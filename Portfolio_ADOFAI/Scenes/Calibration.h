#pragma once
#include "Scene.h"

class Calibration : public Scene
{
private:
	Text::Component		Announcements[4];
	Image::Component	DottedLine;
	Image::Component	Exit[2];

	Text::Component		LevelTitle;
	Text::Component		TextStart;
	Text::Component		TextReady;
	Text::Component		Countdown[8];
	unsigned short		CountdownIndex;

public:
	Calibration(unsigned short bpm, float movement_unit);
	virtual void Start() override;
	virtual bool Update() override;
	virtual void End() override;
};