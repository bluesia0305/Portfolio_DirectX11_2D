#include "stdafx.h"
#include "EditableMap.h"

Map::EditableMap::EditableMap() : UnidirectionalMap()
{
	LeadingNumber	= 32;
	OperationMode	= OPERATION_MODE::EDIT_MODE;
	SelectionEffect = Image::Component{ "effect_tile_select", VEC_POSITION_ORIGIN, VEC_LENGTH_LEVELTILE * 1.5f, VEC_ANGLE_NULL, 0.9f };

	for (int i = 0; i < 9; ++i)
		Tiles.push_back(new Object::AUnidirectionalTile("neon", vector<2>{ MOVEMENT_UNIT_LEVEL* i, 0 }, 0.0f, 0.0f));

	SelectedTile = Tiles.begin();
}

void Map::EditableMap::Start()
{
	Synchronize();
	OperationMode = dynamic_cast<LevelEditor*>(CURRENT_SCENE)->GetOperationMode();

	for (auto elem : Tiles)
	{
		SOUND(dynamic_cast<Object::AUnidirectionalTile*>(elem)->GetHitSound().c_str())->Stop();
		if (OperationMode == OPERATION_MODE::EDIT_MODE)
			elem->SetAlphaLevel(1.0f);
		else
			elem->SetAlphaLevel(0.0f);
		elem->SetIsEffectOn(false);
	}

	CurrentTile	= Tiles.begin();
	ShowFrontTiles(LeadingNumber);
}

void Map::EditableMap::Update()
{
	SelectionEffect.Position = (*SelectedTile)->GetPosition();
	Synchronize();

	(*Tiles.begin())->PresentEffect();

	if (OperationMode == OPERATION_MODE::TEST_MODE)
	{
		float tempAlpha;
		{
			tempAlpha = (*LeadingTile)->GetAlphaLevel() + 8.0f * DELTA_TIME / SECONDS_PER_BEAT * (*CurrentTile)->GetMagnification();
			(*LeadingTile)->SetAlphaLevel(BoundedFloat(tempAlpha, 0.0f, 1.0f));

			tempAlpha = (*std::prev(LeadingTile, 1))->GetAlphaLevel() + 8.0f * DELTA_TIME / SECONDS_PER_BEAT * (*CurrentTile)->GetMagnification();
			(*std::prev(LeadingTile, 1))->SetAlphaLevel(BoundedFloat(tempAlpha, 0.0f, 1.0f));
		}

		if (CurrentTile != Tiles.begin())
		{
			tempAlpha = (*std::prev(CurrentTile, 1))->GetAlphaLevel() - 2.0f * DELTA_TIME / SECONDS_PER_BEAT * (*CurrentTile)->GetMagnification();
			(*std::prev(CurrentTile, 1))->SetAlphaLevel(tempAlpha);

			if (std::prev(CurrentTile, 1) != Tiles.begin())
				(*std::prev(CurrentTile, 2))->SetAlphaLevel(0.0f);
		}
	}
	else
		for (auto elem : Tiles)
			elem->SetAlphaLevel(1.0f);

	// Draw in reverse order
	for (std::list<Object::ATile*>::reverse_iterator r_iter = Tiles.rbegin(); r_iter != Tiles.rend(); ++r_iter)
	{
		if ((*r_iter)->GetAlphaLevel())
			(*r_iter)->Update();

		if (*r_iter == *SelectedTile)
			if (OperationMode == OPERATION_MODE::EDIT_MODE)
				SelectionEffect.Draw();
	}
}

void Map::EditableMap::End() {}

void Map::EditableMap::Export() const
{
	INSTANCE(DataManager)->ClearTilesInfo();

	for (auto elem : Tiles)
	{
		auto result = dynamic_cast<Object::AUnidirectionalTile*>(elem);
		INSTANCE(DataManager)->TilesInfo->TileTheme.push_back(result->GetTileTheme());
		INSTANCE(DataManager)->TilesInfo->ContentTile.push_back(result->GetContentTile());
		INSTANCE(DataManager)->TilesInfo->ContentIcon1.push_back((result->GetContentIcon(0) == "" ? "__NULL__" : result->GetContentIcon(0)));
		INSTANCE(DataManager)->TilesInfo->ContentIcon2.push_back((result->GetContentIcon(1) == "" ? "__NULL__" : result->GetContentIcon(1)));
		INSTANCE(DataManager)->TilesInfo->MainPosition.push_back(result->GetPosition());
		INSTANCE(DataManager)->TilesInfo->Angle.push_back(result->GetAngle());
		INSTANCE(DataManager)->TilesInfo->BentAngle.push_back(result->GetBentAngle());
		INSTANCE(DataManager)->TilesInfo->isPassingTile.push_back(result->IsPassingTile());
		INSTANCE(DataManager)->TilesInfo->isReversalTile.push_back(result->IsReversalTile());
		INSTANCE(DataManager)->TilesInfo->Magnification.push_back(result->GetMagnification());
		INSTANCE(DataManager)->TilesInfo->HitSound.push_back(result->GetHitSound());
		++INSTANCE(DataManager)->TilesInfo->NumTiles;
	}
}

void Map::EditableMap::Import()
{
	Map::UnidirectionalMap::Import();
	SelectedTile = Tiles.begin();
}

Object::ATile* Map::EditableMap::GetSelectedTile() const { return (*SelectedTile); }

void Map::EditableMap::SelectTile(SELECT_TILE type)
{
	switch (type)
	{
	case SELECT_TILE::BEGIN_TILE:	SelectedTile = Tiles.begin(); break;
	case SELECT_TILE::PREV_TILE:	if (SelectedTile != Tiles.begin()) std::advance(SelectedTile, -1); break;
	case SELECT_TILE::NEXT_TILE:	if (*SelectedTile != Tiles.back()) std::advance(SelectedTile, 1); break;
	case SELECT_TILE::END_TILE:		SelectedTile = std::prev(Tiles.end()); break;
	}
}

void Map::EditableMap::PrintTileInfo() const
{
	Object::AUnidirectionalTile* result = dynamic_cast<Object::AUnidirectionalTile*>(*SelectedTile);
	system("cls");
	printf("현재 선택된 타일 정보\n");
	printf("- main_pos       : %12.7f\t%12.7f\n", result->GetPosition()[0], result->GetPosition()[1]);
	printf("- main_angle	 : %12.7f\n",		  result->GetMainAngle());
	printf("- bent_angle     : %12.7f\n",		  result->GetBentAngle());
	printf("- x-axis flipped : %s\n",			  result->GetAngle()[0] == 180.0f ? "true" : "false");
	printf("- isPassingTile  : %s\n",			  result->IsPassingTile() ? "true" : "false");
	printf("- isReversalTile : %s\n",			  result->IsReversalTile() ? "true" : "false");
	printf("- magnification  : %12.7f\n",		  result->GetMagnification());
	printf("- hit_sound      : %s\n",			  result->GetHitSound().c_str());
}

void Map::EditableMap::AddTile(float created_main_angle, bool generatePassingTile)
{
	float magnification = (*SelectedTile)->GetMagnification();
	AdjustAngle(created_main_angle);

	// Push main_pos of forward tiles (insertion_displacement = created_main_pos - current_main_pos)
	vector<2> insertion_displacement = vector<2>{ MOVEMENT_UNIT_LEVEL, 0 } * transpose(Transform::Rotation(created_main_angle));
	
	// Recursive formula: main_angle(n + 1) = main angle(n) + bent_angle(n)
	float current_bent_angle = created_main_angle - (*SelectedTile)->GetMainAngle();
	AdjustAngle(current_bent_angle);

	// If island tile (bent_angle == 180.0f)
	if (current_bent_angle == 180.0f && generatePassingTile)
	{
		if (SelectedTile == Tiles.begin())	// First tile cannot be passing tile
			return;
		else
			dynamic_cast<Object::AUnidirectionalTile*>(*SelectedTile)->SetAsPassingTile();
	}
	
	dynamic_cast<Object::AUnidirectionalTile*>(*SelectedTile)->SetBentAngle(current_bent_angle);

	vector<2> created_main_pos = (*SelectedTile)->GetPosition() + insertion_displacement;

	if (*SelectedTile == Tiles.back())
	{
		// Insert tile at the end
		Tiles.push_back(new Object::AUnidirectionalTile(dynamic_cast<LevelEditor*>(CURRENT_SCENE)->GetTileTheme(),
														created_main_pos, created_main_angle, 0.0f));
		std::advance(SelectedTile, 1);
	}
	else
	{
		float created_bent_angle = (*std::next(SelectedTile))->GetMainAngle() - created_main_angle;

		// Insert tile in front of next tile
		std::advance(SelectedTile, 1);
		Tiles.insert(SelectedTile, new Object::AUnidirectionalTile(dynamic_cast<LevelEditor*>(CURRENT_SCENE)->GetTileTheme(),
																   created_main_pos, created_main_angle, AdjustAngle(created_bent_angle)));
		std::advance(SelectedTile, -1);

		// Push main_pos of forward tiles
		for (unsigned i = 1; std::next(SelectedTile, i) != Tiles.end(); ++i)
			(*std::next(SelectedTile, i))->SetPosition((*std::next(SelectedTile, i))->GetPosition() + insertion_displacement);
	}
	dynamic_cast<Object::AUnidirectionalTile*>(*SelectedTile)->SetMagnification(magnification);
}

void Map::EditableMap::RemoveTile()
{
	if (Tiles.size() < 3)
		return;

	if (*SelectedTile == Tiles.back())
	{
		SelectTile(SELECT_TILE::PREV_TILE);
		Tiles.pop_back();
		dynamic_cast<Object::AUnidirectionalTile*>(*SelectedTile)->SetBentAngle(0.0f);
		dynamic_cast<Object::AUnidirectionalTile*>(*SelectedTile)->SetMagnification(1.0f);
		dynamic_cast<Object::AUnidirectionalTile*>(*SelectedTile)->SetIsReversalTile(false);
	}
	else
	{
		vector<2> deleted_main_pos	    = (*SelectedTile)->GetPosition();
		vector<2> deletion_displacement = deleted_main_pos - (*std::next(SelectedTile))->GetPosition();
		float	  deleted_main_angle	= (*SelectedTile)->GetMainAngle();
		float	  deleted_bent_angle    = dynamic_cast<Object::AUnidirectionalTile*>(*SelectedTile)->GetBentAngle();

		// Remove current tile
		delete *SelectedTile;
		SelectedTile = Tiles.erase(SelectedTile);

		// Pull main_pos of forward tiles
		for (unsigned i = 0; std::next(SelectedTile, i) != Tiles.end(); ++i)
			(*std::next(SelectedTile, i))->SetPosition((*std::next(SelectedTile, i))->GetPosition() + deletion_displacement);

		// Adjust main_angle & bent_angle of the next tile					  should be next_main_angle 		  should be next_bent_angle
		// 1) next_main_angle  = deleted_main_angle										 |<-------------->|	  |<-------------------------------->|
		// 2) next_bent_angle += deleted_bent_angle			since next_next_main_angle = deleted_main_angle + deleted_bent_angle + next_bent_angle
		(*SelectedTile)->SetMainAngle(deleted_main_angle);
		if ((*SelectedTile) != Tiles.back())
		{
			float bent_angle = dynamic_cast<Object::AUnidirectionalTile*>(*SelectedTile)->GetBentAngle() + deleted_bent_angle;
			dynamic_cast<Object::AUnidirectionalTile*>(*SelectedTile)->SetBentAngle(AdjustAngle(bent_angle));
		}
		else
			dynamic_cast<Object::AUnidirectionalTile*>(*SelectedTile)->SetBentAngle(0.0f);
	}
}

void Map::EditableMap::AdjustedRemoveTile()
{
	if (*SelectedTile == Tiles.back())
	{
		SelectTile(SELECT_TILE::PREV_TILE);
		Tiles.pop_back();
		dynamic_cast<Object::AUnidirectionalTile*>(*SelectedTile)->SetBentAngle(0.0f);
	}
	else
	{
		vector<2> current_main_pos	    = (*SelectedTile)->GetPosition();
		float	  current_bent_angle    = dynamic_cast<Object::AUnidirectionalTile*>(*SelectedTile)->GetBentAngle();
		vector<2> deletion_displacement = current_main_pos - (*std::next(SelectedTile))->GetPosition();

		for (unsigned i = 1; std::next(SelectedTile, i) != Tiles.end(); ++i)
		{
			// pulling main_pos
			(*std::next(SelectedTile, i))->SetPosition((*std::next(SelectedTile, i))->GetPosition() + deletion_displacement);
			// rotating main_pos
			(*std::next(SelectedTile, i))->SetPosition(current_main_pos + ((*std::next(SelectedTile, i))->GetPosition() - current_main_pos) * transpose(Transform::Rotation(-current_bent_angle)));
			// adjust main_angle
			float main_angle = (*std::next(SelectedTile, i))->GetMainAngle() - current_bent_angle;
			(*std::next(SelectedTile, i))->SetMainAngle(AdjustAngle(main_angle));
		}

		// Remove current tile
		SelectedTile = Tiles.erase(SelectedTile);
	}
}

void Map::EditableMap::ReadjustTile()
{

}

void Map::EditableMap::SetTileTheme(std::string tile_theme)
{
	for (auto elem : Tiles)
	{
		Object::AUnidirectionalTile* result = dynamic_cast<Object::AUnidirectionalTile*>(elem);
		result->SetTileTheme(tile_theme);

		float bent_angle = result->GetBentAngle();
		if (result->IsPassingTile())
			result->SetAsPassingTile();
		else
			dynamic_cast<Object::AUnidirectionalTile*>(elem)->FindContentTile(bent_angle);
	}
}

void Map::EditableMap::SetHitSound(std::string hit_sound)
{
	for (auto elem : Tiles)
	{
		Object::AUnidirectionalTile* result = dynamic_cast<Object::AUnidirectionalTile*>(elem);
		result->SetHitSound(hit_sound);
	}
}

void Map::EditableMap::SetMagnification(float magnification)
{
	bool shouldUpdate = false;
	for (auto elem : Tiles)
	{
		if (elem == *SelectedTile)
			shouldUpdate = true;

		if (shouldUpdate)
			dynamic_cast<Object::AUnidirectionalTile*>(elem)->SetMagnification(magnification);
	}
}

void Map::EditableMap::Synchronize()
{
	unsigned int reversalCount = 0;
	float magnification = 1.0f;

	for (auto iter = Tiles.begin(); iter != Tiles.end(); ++iter)
	{
		auto curr_tile = dynamic_cast<Object::AUnidirectionalTile*>(*iter);

		if (curr_tile == Tiles.back())
		{
			curr_tile->SetContentIcon(0, "icon_endpoint", reversalCount);
			curr_tile->SetContentIcon(1, "icon_endpoint", reversalCount);
			return;
		}

		// Speed change icons
		else if (curr_tile->GetMagnification() > magnification)
		{
			curr_tile->SetContentIcon(0, "icon_rabbit", reversalCount);
			curr_tile->SetContentIcon(1, "icon_rabbit_gray", reversalCount);
			magnification = curr_tile->GetMagnification();
		}
		else if (curr_tile->GetMagnification() < magnification)
		{
			curr_tile->SetContentIcon(0, "icon_snail", reversalCount);
			curr_tile->SetContentIcon(1, "icon_snail_gray", reversalCount);
			magnification = curr_tile->GetMagnification();
		}

		// Direction change icons
		else if (curr_tile->IsReversalTile())
		{
			++reversalCount;
			curr_tile->SetContentIcon(0, "icon_swirl_red", reversalCount);
			curr_tile->SetContentIcon(1, "icon_swirl_red", reversalCount);
		}
		else
		{
			curr_tile->SetContentIcon(0, "", reversalCount);
			curr_tile->SetContentIcon(1, "", reversalCount);
		}
	}
}