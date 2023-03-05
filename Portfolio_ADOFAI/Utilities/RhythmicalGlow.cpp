#include "stdafx.h"
#include "RhythmicalGlow.h"

float RhythmicalGlow::RhythmicalFluctuation(float minValue, float maxValue)
{
	std::mt19937						gen(RandomDevice());
	std::uniform_int_distribution<int>	dis(0, 100);

	EffectTimer += DELTA_TIME;
	if (!isDrawEffect && EffectTimer >= SECONDS_PER_BEAT)
		isDrawEffect = true;
	
	AdjustFloat(EffectTimer, SECONDS_PER_BEAT);

	int randNum = dis(gen);
	float level = maxValue * static_cast<float>(pow(5.0f, -EffectTimer)) + static_cast<float>(pow(-1.0f, randNum)) * randNum * 0.25f * DELTA_TIME;
	return BoundedFloat(level, minValue, maxValue);
}