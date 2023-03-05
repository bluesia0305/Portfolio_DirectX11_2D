#pragma once
#include "AActor.h"
#include "Objects/PoolableObjects.h"
#include <random>

using namespace Rendering;

// Forward declaration
namespace Map { class TileMap; }

enum class REVOLUTION_MODE
{
	NORMAL_STATE,
	REVERSAL_STATE,
};

enum class ANGLE_DIRECTION
{
	CLOCKWISE = -1,
	COUNTER_CLOCKWISE = 1
};

enum class PLAYER_MODE
{
	INTRO_MODE,
	LEVEL_MODE
};

namespace Object
{
	class APlayer final : public AActor
	{
	private:
		// Members to be saved
		std::string						ContentPlanet[2];
		std::string						ContentCometTail[2];
		bool							isRotating;
		
		// Members to be initialized once instace is generated
		Image::Component				Planet[2];
		Image::Component				Orbit[2];
		Image::Component				Spotlight;
		ObjectPool<Object::ACometTail>* CometTail;
		ObjectPool<Object::ADecision>*	DecisionEffect;
		float							RPM;
		float							RevRadius;

		// Members to be initialized respectively for each scene start
		ANGLE_DIRECTION					AngleDirection;
		REVOLUTION_MODE					RevolutionMode;
		float							Magnification;
		float							CometTailTimer;
		float							PreExplosionTimer;
		float							ExplosionTimer;
		//								+ Position, Angle, RevAngle

		bool							isDrawOrbit[2];
		bool							isDrawSpotlight;
		bool							isReversible;
		bool							isExplodeable;

	public:
		APlayer(float bpm, float revRadius);
		virtual ~APlayer();
		virtual void Start() override;
		virtual void Update() override;
		virtual void End() override;

		void Save();
		void Load();

		vector<2> GetPosition() const;
		vector<3> GetAngle() const;
		vector<2> GetSatellitePosition() const;
		float GetRevRadius() const;
		float GetRevAngle() const;
		short GetAngleDirection() const;
		float GetMagnification() const;
		std::string GetPlanetContent(unsigned char index) const;
		std::string GetCometTailContent(unsigned char index) const;
		bool IsRotating() const;
		bool IsExplodeable() const;

		void SetPosition(vector<2> position);
		void SetRevRadius(float radius);
		void SetIsRotating(bool isRotating);
		void SetRPM(float bpm);
		void SetMagnification(float magnification);
		void SetPlanetContent(unsigned char index, std::string content);
		void SetCometTailContent(unsigned char index, std::string content);
		void ChangePlanetContent(unsigned char index, std::string content, bool isNeedSync);
		void ChangeCometTailContent(std::string content, bool isNeedSync);

		void Move(Map::TileMap* map);
		void ChangeDirection(float main_angle);
		void Explode();

	private:
		void DisplayCometTail();

	public:
		void DisplayDecision(vector<2> position, unsigned char decision_index);
		
	public:
		void ResetCometTail() const;
	};
}