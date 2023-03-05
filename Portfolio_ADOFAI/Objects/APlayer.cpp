#include "stdafx.h"
#include "APlayer.h"
#include "Utilities/Transform.h"

#define CENTERED_PLANET					Planet[0 + static_cast<char>(RevolutionMode)]
#define SATELLITE_PLANET				Planet[1 - static_cast<char>(RevolutionMode)]
#define CENTERED_ORBIT					Orbit[0 + static_cast<char>(RevolutionMode)]
#define SATELLITE_ORBIT					Orbit[1 - static_cast<char>(RevolutionMode)]
#define DRAW_CENTERED_ORBIT				isDrawOrbit[0 + static_cast<char>(RevolutionMode)]
#define DRAW_SATELLITE_ORBIT			isDrawOrbit[1 - static_cast<char>(RevolutionMode)]
#define CENTERED_COMET_TAIL				ContentCometTail[0 + static_cast<char>(RevolutionMode)]
#define SATELLITE_COMET_TAIL			ContentCometTail[1 - static_cast<char>(RevolutionMode)]

#define PLANET_ROTATION_SPEED			180.0f
#define SATELLITE_REVOLUTION_SPEED		(RPM * 6.0f) * Magnification	// Conversion formula -> Rev speed (in degree) = RPS * 360 = (RPM / 60) * 360 = RPM * 6
#define ORBIT_ROTATION_SPEED			25.0f
#define ORBIT_EXPANSION_SPEED			1000.0f
#define ORBIT_SHRINKAGE_SPEED			1000.0f
#define ORBIT_SIZE_MIN					0.0f
#define ORBIT_SIZE_MAX					2.1f * SATELLITE_PLANET.RevRadius	// Adjusted to 2.0f * RevRadius -> 2.1f * RevRadius due to image size
#define RELATIVE_SATELLITE_POSITION		(vector<2>{ SATELLITE_PLANET.RevRadius, 0 } * transpose(Transform::Rotation(SATELLITE_PLANET.RevAngle[2])))

Object::APlayer::APlayer(float bpm, float rev_radius)
{
	ContentPlanet[0]	= "planet_mokoko_base";
	ContentPlanet[1]	= "planet_mokoko_blue";
	ContentCometTail[0] = "comet_tail_simple_base";
	ContentCometTail[1] = "comet_tail_simple_blue";
	isRotating			= false;

	AngleDirection	 = ANGLE_DIRECTION::CLOCKWISE;
	RevolutionMode	 = REVOLUTION_MODE::NORMAL_STATE;
	RPM				 = bpm / 2.0f;
	RevRadius		 = rev_radius;
	Magnification	 = 1.0f;
	CometTailTimer	 = 0.0f;

	isDrawOrbit[0]	 = false;
	isDrawOrbit[1]	 = false;
	isDrawSpotlight  = false;
	isReversible	 = true;
	isExplodeable	 = true;
									  // Content					Position				Length					Angle			AlphaLevel
	CENTERED_PLANET	 = Image::Component{ ContentPlanet[0].c_str(),	VEC_POSITION_ORIGIN,	VEC_LENGTH_PLANET,		VEC_ANGLE_NULL,	1.0f};
	SATELLITE_PLANET = Image::Component{ ContentPlanet[1].c_str(),	VEC_POSITION_ORIGIN,	VEC_LENGTH_PLANET,		VEC_ANGLE_NULL,	1.0f};
	CENTERED_ORBIT	 = Image::Component{ "effect_rev_orbit_white",	VEC_POSITION_ORIGIN,	VEC_LENGTH_NULL,		VEC_ANGLE_NULL,	0.7f};
	SATELLITE_ORBIT	 = Image::Component{ "effect_rev_orbit_white",	VEC_POSITION_ORIGIN,	VEC_LENGTH_NULL,		VEC_ANGLE_NULL,	0.7f};
	Spotlight		 = Image::Component{ "effect_planet_spotlight",	VEC_POSITION_ORIGIN,	VEC_LENGTH_INTROTILE,	VEC_ANGLE_NULL,	1.0f};
	CometTail		 = new ObjectPool<ACometTail>;
	DecisionEffect	 = new ObjectPool<ADecision>;
}
Object::APlayer::~APlayer() { delete CometTail; delete DecisionEffect; }
void Object::APlayer::Start()
{
	Load();
	RPM = CURRENT_SCENE->GetBPM() / 2.0f;
	Planet[0].Content	= ContentPlanet[0].c_str();
	Planet[1].Content	= ContentPlanet[1].c_str();

	// 체크포인트 구현하면 수정해야함
	AngleDirection = ANGLE_DIRECTION::CLOCKWISE;		// Scene에서 저장, Checkpoint 도착시 값
	RevolutionMode = REVOLUTION_MODE::NORMAL_STATE;		// Scene에서 저장, Checkpoint 도착시 값
	Magnification  = 1.0f;								// Scene에서 저장, Checkpoint 도착시 값
	
	CometTailTimer = 0.0f;
	ExplosionTimer = 0.0f;

	SetPosition(CURRENT_SCENE->GetStartPos());
	CENTERED_PLANET.Angle[2]	 = CENTERED_ORBIT.Angle[2]	   = 0.0f;
	CENTERED_PLANET.RevAngle[2]  = CENTERED_ORBIT.RevAngle[2]  = 0.0f;
	CENTERED_PLANET.RevRadius	 = CENTERED_ORBIT.RevRadius    = 0.0f;
	SATELLITE_PLANET.Angle[2]	 = SATELLITE_ORBIT.Angle[2]	   = 180.0f;
	SATELLITE_PLANET.RevAngle[2] = SATELLITE_ORBIT.RevAngle[2] = 180.0f;
	SATELLITE_PLANET.RevRadius	 = SATELLITE_ORBIT.RevRadius   = 0.0f;

	isDrawOrbit[0]	= false;
	isDrawOrbit[1]	= false;
	isDrawSpotlight = false;
	isReversible    = true;
	isExplodeable   = true;
}
void Object::APlayer::Update()
{
	if (CURRENT_SCENE->IsStarted())
	{
		if (!isExplodeable && ExplosionTimer >= 0)
		{
			ExplosionTimer += DELTA_TIME;
			if (ExplosionTimer > SECONDS_PER_BEAT)
			{
				ExplosionTimer = -1.0f;
				dynamic_cast<Level*>(CURRENT_SCENE)->SetIsRestartable(true);
				SOUND("PlanetExplosion2")->Play();
			}
		}

		// Revolution radius expansion and shrinkage (start & end)
		if (isExplodeable)
		{
			if (SATELLITE_PLANET.RevRadius < RevRadius)
				SATELLITE_PLANET.RevRadius += RevRadius * DELTA_TIME / SECONDS_PER_BEAT * 2;
			else
				SATELLITE_PLANET.RevRadius = RevRadius;
		}
		else
		{
			if (SATELLITE_PLANET.RevRadius > 0)
				SATELLITE_PLANET.RevRadius -= RevRadius * DELTA_TIME / SECONDS_PER_BEAT * 2;
			else
				SATELLITE_PLANET.RevRadius = 0.0f;
		}

		// Set comet Tails
		DisplayCometTail();

		// Revolution orbit expansion and shrinkage
		if (CENTERED_ORBIT.Length[0]  < ORBIT_SIZE_MAX)	CENTERED_ORBIT.Length  += VEC_UNIT_SQUARE * ORBIT_EXPANSION_SPEED * DELTA_TIME;
		else											CENTERED_ORBIT.Length   = VEC_UNIT_SQUARE * ORBIT_SIZE_MAX;
		if (SATELLITE_ORBIT.Length[0] > ORBIT_SIZE_MIN)	SATELLITE_ORBIT.Length -= VEC_UNIT_SQUARE * ORBIT_SHRINKAGE_SPEED * DELTA_TIME;
		else											SATELLITE_ORBIT.Length  = VEC_UNIT_SQUARE * ORBIT_SIZE_MIN;

		// Rotation and revolution with angle range adjustment to 0 <= angle < 360
		if (isRotating)
		{
			CENTERED_PLANET.Angle[2]  += static_cast<short>(AngleDirection) * PLANET_ROTATION_SPEED		 * DELTA_TIME;
			SATELLITE_PLANET.Angle[2] += static_cast<short>(AngleDirection) * PLANET_ROTATION_SPEED		 * DELTA_TIME;
			AdjustAngle(CENTERED_PLANET.Angle[2]);
			AdjustAngle(SATELLITE_PLANET.Angle[2]);
		}
		else
		{
			CENTERED_PLANET.Angle[2]   = 0.0f;
			SATELLITE_PLANET.Angle[2]  = -SATELLITE_PLANET.RevAngle[2];
		}

		CENTERED_ORBIT.Angle[2]		  += static_cast<short>(AngleDirection) * ORBIT_ROTATION_SPEED		 * DELTA_TIME;
		SATELLITE_PLANET.Angle[2]	  -= static_cast<short>(AngleDirection) * SATELLITE_REVOLUTION_SPEED * DELTA_TIME;
		SATELLITE_PLANET.RevAngle[2]  += static_cast<short>(AngleDirection) * SATELLITE_REVOLUTION_SPEED * DELTA_TIME;
		AdjustAngle(CENTERED_ORBIT.Angle[2]);
		AdjustAngle(SATELLITE_PLANET.RevAngle[2]);
	}

	// Synchronize angles
	SATELLITE_ORBIT.Angle[2]	= CENTERED_ORBIT.Angle[2];
	SATELLITE_ORBIT.RevAngle[2]	= SATELLITE_PLANET.RevAngle[2];

	// Draw
	if (isDrawSpotlight) Spotlight.Draw();
	if (DRAW_CENTERED_ORBIT) CENTERED_ORBIT.Draw();
	if (DRAW_SATELLITE_ORBIT) SATELLITE_ORBIT.Draw();
	CometTail->Update();
	SATELLITE_PLANET.Draw();
	CENTERED_PLANET.Draw();
	DecisionEffect->Update();
}
void Object::APlayer::End()
{
	CometTail->Shutdown();
	DecisionEffect->Shutdown();
}

void Object::APlayer::Save()
{
	for (int i = 0; i < 2; ++i)
	{
		INSTANCE(DataManager)->PlayerInfo->ContentPlanet[i]    = ContentPlanet[i];
		INSTANCE(DataManager)->PlayerInfo->ContentCometTail[i] = ContentCometTail[i];
	}
	INSTANCE(DataManager)->PlayerInfo->isRotating = isRotating;

	INSTANCE(DataManager)->ExportPlayer();
}
void Object::APlayer::Load()
{
	INSTANCE(DataManager)->ImportPlayer();

	for (int i = 0; i < 2; ++i)
	{
		ContentPlanet[i]	= INSTANCE(DataManager)->PlayerInfo->ContentPlanet[i];
		ContentCometTail[i] = INSTANCE(DataManager)->PlayerInfo->ContentCometTail[i];
	}
	isRotating = INSTANCE(DataManager)->PlayerInfo->isRotating;
}

vector<2> Object::APlayer::GetPosition() const										{ return CENTERED_PLANET.Position; }
vector<3> Object::APlayer::GetAngle() const											{ return CENTERED_PLANET.Angle; }
vector<2> Object::APlayer::GetSatellitePosition() const								{ return CENTERED_PLANET.Position + RELATIVE_SATELLITE_POSITION; }
float Object::APlayer::GetRevRadius() const											{ return SATELLITE_PLANET.RevRadius; }
float Object::APlayer::GetRevAngle() const											{ return SATELLITE_PLANET.RevAngle[2]; }
short Object::APlayer::GetAngleDirection() const									{ return static_cast<short>(AngleDirection); }
float Object::APlayer::GetMagnification() const										{ return Magnification; }
std::string Object::APlayer::GetPlanetContent(unsigned char index) const			{ return ContentPlanet[index]; }
std::string Object::APlayer::GetCometTailContent(unsigned char index) const			{ return ContentCometTail[index]; }
bool Object::APlayer::IsRotating() const											{ return isRotating; }
bool Object::APlayer::IsExplodeable() const											{ return isExplodeable; }

void Object::APlayer::SetPosition(vector<2> position)
{
	CENTERED_PLANET.Position = SATELLITE_PLANET.Position =
	CENTERED_ORBIT.Position  = SATELLITE_ORBIT.Position  =
	Spotlight.Position		 = position;
}
void Object::APlayer::SetRevRadius(float radius)									{ SATELLITE_PLANET.RevRadius = SATELLITE_ORBIT.RevRadius = radius; }
void Object::APlayer::SetIsRotating(bool isRotating)								{ this->isRotating = isRotating; }
void Object::APlayer::SetRPM(float bpm)												{ RPM = bpm / 2.0f; }
void Object::APlayer::SetMagnification(float magnification)							{ Magnification = magnification; }
void Object::APlayer::SetPlanetContent(unsigned char index, std::string content)	{ ContentPlanet[index] = content; }
void Object::APlayer::SetCometTailContent(unsigned char index, std::string content)	{ ContentCometTail[index] = content; }
void Object::APlayer::ChangePlanetContent(unsigned char index, std::string content, bool isNeedSync)
{
	ContentPlanet[index] = content;

	if (isNeedSync)
	{
		CometTail->Reset();
		CometTailTimer = 0.0f;

		ContentCometTail[index] = ContentCometTail[index].substr(0, ContentCometTail[index].size() - 4) + ContentPlanet[index].substr(14);
	}
}
void Object::APlayer::ChangeCometTailContent(std::string content, bool isNeedSync)
{
	CometTail->Reset();
	CometTailTimer = 0.0f;

	if (isNeedSync)
	{
		ContentCometTail[0] = content.substr(0, content.size() - 4) + ContentPlanet[0].substr(14);
		ContentCometTail[1] = content.substr(0, content.size() - 4) + ContentPlanet[1].substr(14);
	}
	else
	{
		ContentCometTail[0] = content;
		ContentCometTail[1] = content;
	}
}

void Object::APlayer::Move(Map::TileMap* map)
{
	Map::UnidirectionalMap* downcasted_map = dynamic_cast<Map::UnidirectionalMap*>(map);
	float range_of_revolution = 0.0f;
	if (downcasted_map)
	{
		AUnidirectionalTile* pTile = dynamic_cast<AUnidirectionalTile*>(downcasted_map->GetCurrentTile());

		if (pTile)
		{
			float bent_angle = pTile->GetBentAngle();

			if (bent_angle == 180.0f)
			{
				if (pTile->IsPassingTile())
					range_of_revolution = 180.0f + GetAngleDirection() * bent_angle;
				else
					range_of_revolution = 360.0f;
			}
			else
				range_of_revolution = 180.0f + GetAngleDirection() * bent_angle;
		}
	}

	float magnification		 = Magnification;
	float relative_angle	 = 0.0f;
	vector<2> destination	 = map->FindDestination(this);
	vector<2> move_direction = destination - CENTERED_PLANET.Position;
	
	if (range_of_revolution)
	{
		if (length(move_direction) || dynamic_cast<Object::AUnidirectionalTile*>(downcasted_map->GetPrevTile(downcasted_map->GetCurrentTile()))->IsPassingTile())
			dynamic_cast<Level*>(CURRENT_SCENE)->RewindPlayTimer(range_of_revolution / 180.0f * SECONDS_PER_BEAT / magnification);
	}

	if (length(move_direction))
	{
		float move_angle = move_direction[0] ? ConvertToDegree(atan2(move_direction[1], move_direction[0])) : move_direction[1] > 0 ? 90.0f : -90.0f;

		// Centered planet <-> Satellite planet
		SetPosition(destination);
		SATELLITE_PLANET.Angle[2]	= CENTERED_PLANET.Angle[2];
		CENTERED_PLANET.RevRadius	= CENTERED_ORBIT.RevRadius   = SATELLITE_PLANET.RevRadius;

		relative_angle				= SATELLITE_PLANET.RevAngle[2] - move_angle;
		AdjustAngle(relative_angle);
		CENTERED_PLANET.RevAngle[2]	= CENTERED_ORBIT.RevAngle[2] = (180.0f + move_angle) + relative_angle * (Magnification / magnification);
		CENTERED_PLANET.Angle[2]	= SATELLITE_PLANET.Angle[2] - CENTERED_PLANET.RevAngle[2];
		SATELLITE_PLANET.RevRadius	= SATELLITE_ORBIT.RevRadius  = 0.0f;
		SATELLITE_PLANET.RevAngle	= SATELLITE_ORBIT.RevAngle   = VEC_ANGLE_NULL;

		if (!DRAW_SATELLITE_ORBIT) DRAW_SATELLITE_ORBIT = true;
		if (!isDrawSpotlight)	  isDrawSpotlight = true;
		if (!isReversible)		  isReversible	  = true;
		if (!static_cast<bool>(RevolutionMode))
			RevolutionMode  = REVOLUTION_MODE::REVERSAL_STATE;
		else
			RevolutionMode  = REVOLUTION_MODE::NORMAL_STATE;
	}
}
void Object::APlayer::ChangeDirection(float main_angle)
{
	if (isReversible)
	{
		isReversible   = false;
		AngleDirection = static_cast<ANGLE_DIRECTION>((-1) * static_cast<short>(AngleDirection));
		
		if (main_angle == 90.0f)
		{
			// slope of moving direction = infinity or -infinity
			matrix<2, 2> symmetryToXAxis
			{
				-1,	0,
				 0,	1
			};
			vector<2> adjRevPos = RELATIVE_SATELLITE_POSITION * symmetryToXAxis;
			SATELLITE_PLANET.RevAngle[2] = ConvertToDegree(atan2(adjRevPos[1], adjRevPos[0]));
			SATELLITE_PLANET.Angle[2] = -SATELLITE_PLANET.RevAngle[2];
		}
		else
		{
			// a = slope of moving direction ( 0 <= slope < infinity ) = tan(angle)
			float a = tan(ConvertToRadian(main_angle));
			matrix<2, 2> symmetryToMoveDirection
			{
				(1 - a * a) / (a * a + 1),		  2 * a / (a * a + 1),
					  2 * a / (a * a + 1),	(a * a - 1) / (a * a + 1),
			};
			vector<2> adjRevPos = RELATIVE_SATELLITE_POSITION * symmetryToMoveDirection;
			SATELLITE_PLANET.RevAngle[2] = ConvertToDegree(atan2(adjRevPos[1], adjRevPos[0]));
			SATELLITE_PLANET.Angle[2] = -SATELLITE_PLANET.RevAngle[2];
		}
	}
}

void Object::APlayer::Explode()
{
	if (isExplodeable)
	{
		isExplodeable = false;
		SOUND("PlanetExplosion1")->Play();
	}
}

void Object::APlayer::DisplayCometTail()
{
	{
		CometTailTimer += DELTA_TIME;
		if (SATELLITE_COMET_TAIL.substr(11, 6) == "simple")
		{
			if (CometTailTimer >= 0.0005f / Magnification)
			{
				CometTail->GetRecycledObject()->Display(this, SATELLITE_COMET_TAIL.c_str());
				AdjustFloat(CometTailTimer, 0.0005f / Magnification);
			}
		}
		else
		{
			if (CometTailTimer >= 0.035f / Magnification)
			{
				CometTail->GetRecycledObject()->Display(this, SATELLITE_COMET_TAIL.c_str());
				AdjustFloat(CometTailTimer, 0.035f / Magnification);
			}
		}
	}
}
void Object::APlayer::DisplayDecision(vector<2> tile_pos, unsigned char decision_index)
{ DecisionEffect->GetRecycledObject()->Display(tile_pos, GetSatellitePosition(), decision_index); }

void Object::APlayer::ResetCometTail() const { CometTail->Reset(); }