#pragma once
#include "AActor.h"
#include "Utilities/RhythmicalGlow.h"

using namespace Rendering;

namespace Object
{
	class APortal final : public AActor, public RhythmicalGlow
	{
	private:
		vector<2>			DescPos;
		Image::Component	NameTag;
		Image::Component	LevelName;
		Image::Component	Preview;
		Image::Component	PreviewFX1[6];
		Image::Component	PreviewFX2[6];
		Image::Component	Lanterns[3];
		Image::Component	Lights[3];
		Text::Component		Text;

		float				PendulumTimer;
		float				PendulumAmplitude;
		float				VibrationTimer;
		float				VibrationAmplitude;

	public:
		APortal(vector<2>, const char*, const char*, const char*);
		virtual void Start() override;
		virtual void Update() override;
		virtual void End() override;
		
	private:
		void PendulumMovements();
		void SimpleVibrations();
		void EllipticMovements();
		void Shrink();
		void Expand();
	};
}