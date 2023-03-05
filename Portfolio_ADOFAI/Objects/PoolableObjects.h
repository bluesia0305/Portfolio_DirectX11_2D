#pragma once
#include "AActor.h"
#include "Utilities/ObjectPool.h"
#include <random>

using namespace Rendering;

namespace Object
{
	class APlayer;

	class ACometTail final : public AActor, public IPoolableObject
	{
	public:
		Animation::Component	Effect;
		std::random_device		RandomDevice;
		bool					isRecyclable;

	public:
		ACometTail();
	
		virtual void Start() override;
		virtual void Update() override;
		virtual void End() override;
	
		virtual bool IsRecyclable() override;
		virtual void ForceRecycle() override;
		virtual void OffRecycle() override;
		void Display(APlayer* pPlayer, const char* content);
		void Display(vector<2> position, vector<2> length, float revRadius, float revAngle, const char* content);
	};

	class ADecision final : public AActor, public IPoolableObject
	{
	public:
		Image::Component		DecisionTexts[5];
		Image::Component		Indicator;
		int						DecisionIndex;
		bool					isRecyclable;

	public:
		ADecision();

		virtual void Start() override;
		virtual void Update() override;
		virtual void End() override;

		virtual bool IsRecyclable() override;
		virtual void ForceRecycle() override;
		virtual void OffRecycle() override;
		void Display(vector<2> tile_pos, vector<2> satellite_pos, unsigned char index);
	};


	//class Projectile : public AActor, public IPoolableObject
	//{
	//public:
	//	Projectile();
	//
	//	virtual void Start() override;
	//	virtual void Update() override;
	//	virtual void End() override;
	//
	//	virtual bool IsRecyclable() override;
	//	virtual void OffRecycle() override;
	//
	//	void Shot(vector<2> location, short direction);
	//	void Explode(vector<2> location);
	//
	//public:
	//	Image::Component	Trajectory;
	//	short				Direction;
	//	bool				isRecyclable;
	//};
}