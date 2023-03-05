#pragma once
#include "Objects/SimpleObjects.h"
#include "Maps/CustomizingMap.h"

class Customizing final : public Scene
{
private:
	Image::Component				Announcement;
	Object::ASimpleImage*			Characters[3];
	Object::ASimpleImage*			Mokokos[8];
	Object::ARotatingImage*			Rotation;
	ObjectPool<Object::ACometTail>*	CometTail;
	float							CometTailRevAngle;
	float							CometTailTimer1;
	float							CometTailTimer2;
	bool							isCustomizable;
	bool							isEscapable;

public:
	Customizing(unsigned short bpm, float movement_unit);
	virtual ~Customizing();
	virtual void Start() override;
	virtual bool Update() override;
	virtual void End() override;

private:
	void DisplayCometTail();
	void CustomizePlanet(vector<2> position, std::string content);
	void CustomizeCometTail(vector<2> position, std::string content, bool isNeedSync);
	void CustomizeRotation();
};