#include "stdafx.h"
#include "UnidirectionalMap.h"

Map::UnidirectionalMap::UnidirectionalMap()
{
	CurrentTile = Tiles.begin();
}

void Map::UnidirectionalMap::Start()
{
	LeadingNumber = 50;

	for (auto elem : Tiles)
	{
		SOUND(dynamic_cast<Object::AUnidirectionalTile*>(elem)->GetHitSound().c_str())->Stop();
		elem->SetAlphaLevel(0.0f);
		elem->SetIsEffectOn(false);
	}

	CurrentTile = Tiles.begin();
	ShowFrontTiles(LeadingNumber);
}

void Map::UnidirectionalMap::Update()
{
	float tempAlpha;
	{
		tempAlpha = (*LeadingTile)->GetAlphaLevel() + 8.0f * DELTA_TIME / SECONDS_PER_BEAT * (*CurrentTile)->GetMagnification();
		(*LeadingTile)->SetAlphaLevel(BoundedFloat(tempAlpha, 0.0f, 1.0f));

		tempAlpha = (*std::prev(LeadingTile, 1))->GetAlphaLevel() + 4.0f * DELTA_TIME / SECONDS_PER_BEAT * (*CurrentTile)->GetMagnification();
		(*std::prev(LeadingTile, 1))->SetAlphaLevel(BoundedFloat(tempAlpha, 0.0f, 1.0f));
	}

	if (CurrentTile != Tiles.begin())
	{
		for (auto tile : Tiles)
		{
			if (tile == *CurrentTile)
				break;

			if (tile->GetAlphaLevel() > 0.0f)
			{
				tempAlpha = tile->GetAlphaLevel() - 2.0f * DELTA_TIME / SECONDS_PER_BEAT * (*CurrentTile)->GetMagnification();
				tile->SetAlphaLevel(BoundedFloat(tempAlpha, 0.0f, 1.0f));
			}
		}
	}

	// Draw in reverse order
	for (std::list<Object::ATile*>::reverse_iterator r_iter = Tiles.rbegin(); r_iter != Tiles.rend(); ++r_iter)
	{
		(*r_iter)->Update();

		if (*r_iter == *Tiles.begin())
			(*r_iter)->PresentEffect();
	}

	(*Tiles.begin())->PresentEffect();
}

void Map::UnidirectionalMap::End() {}

void Map::UnidirectionalMap::Import()
{
	for (auto elem : Tiles)
		delete elem;

	Tiles.clear();
	for (unsigned i = 0; i < INSTANCE(DataManager)->TilesInfo->NumTiles; ++i)
	{
		Tiles.push_back(new Object::AUnidirectionalTile(INSTANCE(DataManager)->TilesInfo->TileTheme[i],
														INSTANCE(DataManager)->TilesInfo->ContentTile[i],
														INSTANCE(DataManager)->TilesInfo->ContentIcon1[i] == "__NULL__" ? "" : INSTANCE(DataManager)->TilesInfo->ContentIcon1[i],
														INSTANCE(DataManager)->TilesInfo->ContentIcon2[i] == "__NULL__" ? "" : INSTANCE(DataManager)->TilesInfo->ContentIcon2[i],
														INSTANCE(DataManager)->TilesInfo->HitSound[i],
														INSTANCE(DataManager)->TilesInfo->MainPosition[i],
														INSTANCE(DataManager)->TilesInfo->Angle[i],
														INSTANCE(DataManager)->TilesInfo->BentAngle[i],
														INSTANCE(DataManager)->TilesInfo->Magnification[i],
														INSTANCE(DataManager)->TilesInfo->isPassingTile[i],
														INSTANCE(DataManager)->TilesInfo->isReversalTile[i]));
	}
	CurrentTile = Tiles.begin();
	INSTANCE(DataManager)->ClearTilesInfo();
}

Object::ATile* Map::UnidirectionalMap::GetStartTile() const { return Tiles.front(); }

Object::ATile* Map::UnidirectionalMap::GetEndTile() const { return Tiles.back(); }

Object::ATile* Map::UnidirectionalMap::GetCurrentTile() const { return *CurrentTile; }

Object::ATile* Map::UnidirectionalMap::GetPrevTile(Object::ATile* pTile) const
{
	Object::ATile* pPrevTile = nullptr;
	for (auto elem : Tiles)
	{
		if (elem == pTile) break;
		else pPrevTile = elem;
	}
	return pPrevTile;
}

Object::ATile* Map::UnidirectionalMap::GetNextTile() const { return *std::next(CurrentTile); }

unsigned Map::UnidirectionalMap::GetNumTiles() const	  { return static_cast<unsigned>(Tiles.size()); }

void Map::UnidirectionalMap::SetDecisionRadius(float radius)
{
	for (auto elem : Tiles)
		dynamic_cast<Object::AUnidirectionalTile*>(elem)->SetDecisionRadius(radius);
}

void Map::UnidirectionalMap::ShowFrontTiles(int leading_number)
{
	for (int i = 0; i < leading_number; ++i)
	{
		LeadingTile = std::next(CurrentTile, i);
		if (LeadingTile != Tiles.end())
			(*LeadingTile)->SetAlphaLevel(1.0f);
		else
		{
			std::advance(LeadingTile, -1);
			break;
		}
	}
}

void Map::UnidirectionalMap::LinkTiles()
{
	std::list<Object::ATile*>::iterator iter = Tiles.begin();
	for (; *iter != Tiles.back(); ++iter)
	{
		if (!dynamic_cast<Object::AUnidirectionalTile*>(*iter)->IsPassingTile())
			(*iter)->Link(*std::next(iter));
		else
			(*iter)->Link(*std::next(iter, 2));
	}
}

vector<2> Map::UnidirectionalMap::FindDestination(Object::APlayer* pPlayer)
{
	if ((*CurrentTile)->Decision(pPlayer))
	{
		std::advance(CurrentTile, 1);
		if (*LeadingTile != Tiles.back())
			std::advance(LeadingTile, 1);

		if (dynamic_cast<Object::AUnidirectionalTile*>(*CurrentTile)->IsPassingTile())
		{
			std::advance(CurrentTile, 1);
			if (*LeadingTile != Tiles.back())
				std::advance(LeadingTile, 1);

			return pPlayer->GetPosition();
		}
		else
			return (*CurrentTile)->GetPosition();
	}
	else
		return pPlayer->GetPosition();
}