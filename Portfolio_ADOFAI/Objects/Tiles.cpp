#include "stdafx.h"
#include "Tiles.h"

Object::ATile::ATile(vector<2> main_position, float main_angle)
{
	ContentTile		  = CONTENT_SQUARE_TILE;
	TileComponent	  = Image::Component{ ContentTile.c_str(), main_position, VEC_LENGTH_INTROTILE, vector<3>{ 0, 0, main_angle }, 1.0f };
						  
	isReversalTile	  = false;
	Magnification	  = 1.0f;
	
	isEffectOn		  = false;
	DecisionRadius	  = DECISION_RADIUS_LENIENT_SQUARE;
}
Object::ATile::ATile(vector<2> main_position, vector<3> total_angle)
{
	ContentTile		  = CONTENT_SQUARE_TILE;
	TileComponent	  = Image::Component{ ContentTile.c_str(), main_position, VEC_LENGTH_INTROTILE, total_angle, 1.0f };

	isReversalTile	  = false;
	Magnification	  = 1.0f;

	isEffectOn		  = false;
	DecisionRadius	  = DECISION_RADIUS_LENIENT_SQUARE;
}
std::string Object::ATile::GetContentTile() const			{ return ContentTile; }
vector<2> Object::ATile::GetPosition() const				{ return TileComponent.Position; }
vector<3> Object::ATile::GetAngle() const					{ return TileComponent.Angle; }
float Object::ATile::GetMainAngle() const					{ return TileComponent.Angle[2]; }
float Object::ATile::GetMagnification() const				{ return Magnification; }
float Object::ATile::GetAlphaLevel() const					{ return TileComponent.AlphaLevel; }
bool Object::ATile::IsEffectOn() const						{ return isEffectOn; }
bool Object::ATile::IsReversalTile() const					{ return isReversalTile; }
void Object::ATile::SetPosition(vector<2> position)			{ TileComponent.Position = position; Effect.Position = position; }
void Object::ATile::SetMainAngle(float main_angle)			{ TileComponent.Angle[2] = main_angle; Effect.Angle[2] = main_angle; }
void Object::ATile::SetAlphaLevel(float alphaLevel)			{ TileComponent.AlphaLevel = alphaLevel; Effect.AlphaLevel = alphaLevel; }
void Object::ATile::SetIsEffectOn(bool isEffectOn)			{ this->isEffectOn = isEffectOn; }
void Object::ATile::PresentEffect()							{ if(Effect.Content) Effect.Draw(); }

Object::AUnidirectionalTile::AUnidirectionalTile(std::string tile_theme, vector<2> main_position, float main_angle, float bent_angle) : ATile(main_position, main_angle)
{
	IconComponent[0]	  = Image::Component{ nullptr,				   TileComponent.Position, VEC_LENGTH_LEVELTILE * 0.55f, VEC_ANGLE_NULL, 1.0f };
	IconComponent[1]	  = Image::Component{ nullptr,				   TileComponent.Position, VEC_LENGTH_LEVELTILE * 0.55f, VEC_ANGLE_NULL, 1.0f };
	Effect				  = Image::Component{ "effect_tile_spotlight", TileComponent.Position, VEC_LENGTH_LEVELTILE * 3.0f,  VEC_ANGLE_NULL, 0.5f };
	LinkedTile			  = nullptr;

	TileTheme			  = tile_theme;
	ContentTile			  = "";
	ContentIcon[0]		  = "";
	ContentIcon[1]		  = "";
	HitSound			  = "KickBasic";
	BentAngle			  = bent_angle;
	this->isPassingTile	  = false;

	TileComponent.Length  = VEC_LENGTH_LEVELTILE;
	DecisionRadius		  = DECISION_RADIUS_LENIENT_LEVEL;

	FindContentTile(bent_angle);
}
Object::AUnidirectionalTile::AUnidirectionalTile(std::string tile_theme, std::string content_tile, std::string content_icon1, std::string content_icon2, std::string hit_sound,
												 vector<2> main_position, vector<3> total_angle, float bent_angle, float magnification, bool isPassingTile, bool isReversalTile) : ATile(main_position, total_angle)
{
	IconComponent[0]	  = Image::Component{ nullptr,				   TileComponent.Position, VEC_LENGTH_LEVELTILE * 0.55f, TileComponent.Angle, 1.0f };
	IconComponent[1]	  = Image::Component{ nullptr,				   TileComponent.Position, VEC_LENGTH_LEVELTILE * 0.55f, TileComponent.Angle, 1.0f };
	Effect				  = Image::Component{ "effect_tile_spotlight", TileComponent.Position, VEC_LENGTH_LEVELTILE * 3.0f,  TileComponent.Angle, 0.5f };
	LinkedTile			  = nullptr;

	TileTheme			  = tile_theme;
	ContentTile			  = content_tile;
	ContentIcon[0]		  = content_icon1;
	ContentIcon[1]		  = content_icon2;
	HitSound			  = hit_sound;
	BentAngle			  = bent_angle;
	Magnification		  = magnification;
	this->isPassingTile	  = isPassingTile;
	this->isReversalTile  = isReversalTile;
	
	TileComponent.Content = ContentTile.c_str();
	TileComponent.Length  = VEC_LENGTH_LEVELTILE;
	DecisionRadius		  = DECISION_RADIUS_LENIENT_LEVEL;

	for (int i = 0; i < 2; ++i)
	{
		if (ContentIcon[i] != "")
			IconComponent[i].Content = ContentIcon[i].c_str();
	}
}
void Object::AUnidirectionalTile::Start() {}
void Object::AUnidirectionalTile::Update()
{
	if (ContentIcon[0] == "icon_endpoint")
	{
		IconComponent[0].Angle[2] += 100.0f * DELTA_TIME;
		AdjustAngle(IconComponent[0].Angle[2]);
		IconComponent[1].Angle[2] = IconComponent[0].Angle[2];
	}
	else
	{
		IconComponent[0].Angle[2] = IconComponent[1].Angle[2] = TileComponent.Angle[2];
	}

	if (TileComponent.AlphaLevel)
	{
		TileComponent.Draw();
		if (isEffectOn)
		{
			if (IconComponent[1].Content)
				IconComponent[1].Draw();
			Effect.Draw();
		}
		else
			if (IconComponent[0].Content)
				IconComponent[0].Draw();
	}
}
void Object::AUnidirectionalTile::End() {}
void Object::AUnidirectionalTile::Link(ATile* pTile)										{ if (pTile) LinkedTile = pTile; }
bool Object::AUnidirectionalTile::Decision(APlayer* pPlayer)
{
	CAMERA->Bounce();

	if (!LinkedTile) return false;
	else
	{
		vector<2> displacement1 = LinkedTile->GetPosition() - TileComponent.Position;			// Displacement of tile center to current position
		vector<2> displacement2 = pPlayer->GetSatellitePosition() - TileComponent.Position;		// Displacement of satellite center from current position
		vector<3> disp1_vec3D	= { displacement1[0], displacement1[1], 0 };
		vector<3> disp2_vec3D	= { displacement2[0], displacement2[1], 0 };
		vector<3> cross_product	= cross(disp1_vec3D, disp2_vec3D);

		if (Collision::Collide(Collision::Point{ pPlayer->GetSatellitePosition() },
							   Collision::Circle{ LinkedTile->GetPosition(), DecisionRadius * sqrt(CURRENT_SCENE->GetBPM() / 130.0f * Magnification) }))
		{
			if (Collision::Collide(Collision::Point{ pPlayer->GetSatellitePosition() },
								   Collision::Circle{ LinkedTile->GetPosition(), DecisionRadius * 0.5f * sqrt(CURRENT_SCENE->GetBPM() / 130.0f * Magnification) }))
				pPlayer->DisplayDecision(LinkedTile->GetPosition(), 2);
			else
			{
				if (pPlayer->GetAngleDirection() * cross_product[2] < 0)
					pPlayer->DisplayDecision(LinkedTile->GetPosition(), 1);
				else
					pPlayer->DisplayDecision(LinkedTile->GetPosition(), 3);
			}

			INSTANCE(SoundManager)->GetSound(dynamic_cast<AUnidirectionalTile*>(LinkedTile)->GetHitSound())->Play();

			pPlayer->SetMagnification(LinkedTile->GetMagnification());
			CAMERA->SetMagnification(LinkedTile->GetMagnification());

			if (LinkedTile->IsReversalTile())
				pPlayer->ChangeDirection(LinkedTile->GetMainAngle());

			LinkedTile->SetIsEffectOn(true);

			dynamic_cast<Level*>(CURRENT_SCENE)->CountProgress();
			dynamic_cast<Level*>(CURRENT_SCENE)->CountDecisionMiss(-1);
			return true;
		}
		else
		{
			dynamic_cast<Level*>(CURRENT_SCENE)->CountDecisionMiss(1);
			if (pPlayer->GetAngleDirection() * cross_product[2] < 0)
				pPlayer->DisplayDecision(LinkedTile->GetPosition(), 0);
			else
				pPlayer->DisplayDecision(LinkedTile->GetPosition(), 4);
			return false;
		}
	}
}
Object::ATile* Object::AUnidirectionalTile::GetLinkedTile() const							{ return LinkedTile; }
std::string Object::AUnidirectionalTile::GetTileTheme() const								{ return TileTheme; }
std::string Object::AUnidirectionalTile::GetContentIcon(unsigned char index) const			{ return ContentIcon[index]; }
float Object::AUnidirectionalTile::GetBentAngle() const										{ return BentAngle; }
std::string Object::AUnidirectionalTile::GetHitSound() const								{ return HitSound; }
bool Object::AUnidirectionalTile::IsPassingTile() const										{ return isPassingTile; }
void Object::AUnidirectionalTile::FindContentTile(float bent_angle)
{
	if (bent_angle >= 0.0f && bent_angle <= 180.0f)
	{
		TileComponent.Angle[0] = 0.0f;
		if (isPassingTile)
			ContentTile = "tile_level_" + TileTheme + "_passing";
		else
			ContentTile = "tile_level_" + TileTheme + "_" + std::to_string(static_cast<int>(bent_angle));
	}
	else if (bent_angle > 180.0f)
	{
		TileComponent.Angle[0] = 180.0f;
		BentAngle = bent_angle - 360.0f;
		ContentTile = "tile_level_" + TileTheme + "_" + std::to_string(static_cast<int>(360.0f - bent_angle));
	}
	else
	{
		TileComponent.Angle[0] = 180.0f;
		ContentTile = "tile_level_" + TileTheme + "_" + std::to_string(static_cast<int>(abs(bent_angle)));
	}
	TileComponent.Content = ContentTile.c_str();
}
void Object::AUnidirectionalTile::SetAlphaLevel(float alphaLevel)							{ ATile::SetAlphaLevel(alphaLevel); IconComponent[0].AlphaLevel = alphaLevel; IconComponent[1].AlphaLevel = alphaLevel; }
void Object::AUnidirectionalTile::SetTileTheme(std::string tile_theme)						{ TileTheme = tile_theme; FindContentTile(BentAngle); }
void Object::AUnidirectionalTile::SetContentTile(std::string content)						{ ContentTile = content; TileComponent.Content = ContentTile.c_str(); }
void Object::AUnidirectionalTile::SetContentIcon(unsigned char index, std::string content, unsigned int reversalCount)
{
	ContentIcon[index] = content;
	if (ContentIcon[index] != "")
		IconComponent[index].Content = ContentIcon[index].c_str();
	else
		IconComponent[index].Content = nullptr;

	if (reversalCount)
	{
		if (reversalCount % 2)
			IconComponent[index].Angle[0] = 180.0f;
		else
			IconComponent[index].Angle[0] = 0.0f;
	}
}
void Object::AUnidirectionalTile::SetDecisionRadius(float radius)							{ DecisionRadius = radius; }
void Object::AUnidirectionalTile::SetPosition(vector<2> main_pos)							{ ATile::SetPosition(main_pos); for (auto& elem : IconComponent) elem.Position = main_pos; }
void Object::AUnidirectionalTile::SetBentAngle(float bent_angle)
{
	if (isPassingTile)
		return;

	BentAngle = bent_angle;
	FindContentTile(BentAngle);
}
void Object::AUnidirectionalTile::SetAsPassingTile()
{
	BentAngle	   = 180.0f;
	isPassingTile  = true;
	isReversalTile = false;
	Magnification  = 1.0f;
	ContentTile	   = "tile_level_" + TileTheme + "_passing";
	TileComponent.Content = ContentTile.c_str();
}
void Object::AUnidirectionalTile::SetIsReversalTile(bool isReversalTile)					{ this->isReversalTile = isReversalTile; }
void Object::AUnidirectionalTile::SetMagnification(float magnification)						{ Magnification = magnification; }
void Object::AUnidirectionalTile::SetHitSound(std::string hit_sound)						{ HitSound = hit_sound; }

Object::ASquareTile::ASquareTile(std::string content_tile, vector<2> main_pos) : ATile(main_pos)
{
	ContentTile			  = content_tile;
	TileComponent.Content = ContentTile.c_str();
	Effect				  = Image::Component{ nullptr, TileComponent.Position, VEC_LENGTH_INTROTILE, VEC_ANGLE_NULL, 1.0f };
}
void Object::ASquareTile::Start()								{ isEffectOn = false; }
void Object::ASquareTile::Update()								{ if (TileComponent.AlphaLevel) TileComponent.Draw(); }
void Object::ASquareTile::End()	{}
bool Object::ASquareTile::Decision(APlayer* pPlayer)
{
	if (!dynamic_cast<CustomLevel*>(CURRENT_SCENE))
		CAMERA->Bounce();

	for (auto elem : LinkedTiles)
		if (Collision::Collide(Collision::Point { pPlayer->GetSatellitePosition() },
							   Collision::Circle{ elem->GetPosition(), DecisionRadius }))
		{
			pPlayer->SetMagnification(elem->GetMagnification());

			elem->SetIsEffectOn(true);
			Destination = elem->GetPosition();
			return true;
		}

	return false;
}
void Object::ASquareTile::Link(ATile* pTile)					{ if (pTile) LinkedTiles.push_back(pTile); }
vector<2> Object::ASquareTile::GetDestination() const			{ return Destination; }
void Object::ASquareTile::SetEffectContent(const char* content)	{ Effect.Content = content; }

Object::AEntranceTile::AEntranceTile(std::string content_tile, vector<2> main_pos) : ASquareTile(content_tile, main_pos)
{ Portal = Image::Component{ "icon_endpoint", TileComponent.Position, VEC_LENGTH_INTROTILE, VEC_ANGLE_NULL, 1.0f }; }
void Object::AEntranceTile::Start()							{ Portal.Angle[2] = 0.0f; isEffectOn = false; }
void Object::AEntranceTile::Update()
{
	// Rotating crystal
	Portal.Angle[2] += 100.0f * DELTA_TIME;
	AdjustAngle(Portal.Angle[2]);

	if (TileComponent.AlphaLevel)
	{
		TileComponent.Draw();
		Portal.Draw();
	}
}
void Object::AEntranceTile::End() {}
void Object::AEntranceTile::SetPosition(vector<2> main_pos)	{ ASquareTile::SetPosition(main_pos); Portal.Position = main_pos; }
void Object::AEntranceTile::SetMainAngle(float main_angle)	{ ASquareTile::SetMainAngle(main_angle); Portal.Angle[2] = main_angle; }
void Object::AEntranceTile::SetAlphaLevel(float alphaLevel)	{ ASquareTile::SetAlphaLevel(alphaLevel); Portal.AlphaLevel = alphaLevel; }
void Object::AEntranceTile::SetCustomLevelEntrance()
{
	ContentTile = "tile_square_neonlight_180";
	TileComponent.Content  = ContentTile.c_str();
	TileComponent.Angle[2] = 180.0f;
}

Object::AMuseDashTile::AMuseDashTile(std::string content_tile, vector<2> main_pos) : ASquareTile(content_tile, main_pos) {}
void Object::AMuseDashTile::Start()	{ TileComponent.Angle[2] = 0.0f; isEffectOn = false; }
void Object::AMuseDashTile::Update()
{
	// Rotation of crystal
	TileComponent.Angle[2] += 100.0f * DELTA_TIME;
	AdjustAngle(TileComponent.Angle[2]);

	if (TileComponent.AlphaLevel)
		TileComponent.Draw();
}
void Object::AMuseDashTile::End() {}