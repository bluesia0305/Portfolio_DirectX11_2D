#pragma once
#include <random>

class RhythmicalGlow
{
protected:
	std::random_device	RandomDevice;
	float				EffectTimer;
	bool				isDrawEffect;

public:
	float RhythmicalFluctuation(float minValue, float maxValue);
};