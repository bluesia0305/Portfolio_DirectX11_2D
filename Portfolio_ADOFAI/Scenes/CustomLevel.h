#pragma once
#include "Maps/CustomLevelMap.h"

class CustomLevel final : public Scene, public RhythmicalGlow
{
private:
	int									NumCustomLevels;

	Object::APortal*					Portal;

	std::vector<Object::ASimpleImage*>	MainTextures;
	std::vector<Object::ASimpleImage*>	SubTextures;
	std::vector<Object::ASimpleImage*>	TitleEffects;

public:
	CustomLevel(unsigned short bpm, float movement_unit, int num_custom_levels);
	virtual ~CustomLevel();
	virtual void Start() override;
	virtual bool Update() override;
	virtual void End() override;
};