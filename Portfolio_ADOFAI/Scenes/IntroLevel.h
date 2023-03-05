#pragma once
#include "Scene.h"
#include "Objects/APortal.h"
#include "Objects/SimpleObjects.h"
#include "Maps/IntroMap.h"
#include "Utilities/RhythmicalGlow.h"

class IntroLevel final : public Scene, public RhythmicalGlow
{
private:
	Object::APortal*					Portal1;
	Object::APortal*					Portal2;

	std::vector<Object::ASimpleImage*>	MainTextures;
	std::vector<Object::ASimpleImage*>	SubTextures;
	std::vector<Object::ASimpleImage*>	TitleEffects;

	bool								isLoaded;
	bool								isCenter;

public:
	IntroLevel(unsigned short bpm, float movement_unit);
	virtual ~IntroLevel();
	virtual void Start() override;
	virtual bool Update() override;
	virtual void End() override;

private:
	void ControlTexturesAlphaLevel(float increment);
	void TitleGlow();
};