#pragma once
#include "Scene.h"

struct TEXT_COLOR
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
};

class Level : public Scene
{
protected:
	std::string			TileTheme;
	std::string			ContentPortal;
	std::string			BGMArtist;
	std::string			BGMName;
	std::string			HitSound;
	
	TEXT_COLOR			TextColor;
	std::string			LevelTitle;
	Text::Component		TextLevelTitle;
	Text::Component		TextReady;
	Text::Component		TextStandBy;
	Text::Component		TextCountdown[8];
	Text::Component		TextEnded;
	
	unsigned int		SteppedTileCount;
	char				DecisionMissCount;
	std::string			Percentage;
	Text::Component		TextOverload;
	Text::Component		TextCompletion;

	float				PlayTimer;

	bool				isAutoPlay;
	bool				isRestartable;
	bool				isCleared;

private:
	std::string			LevelFile;

public:
	Level(unsigned short bpm, float movement_unit);
	Level(std::string level_file, std::string content_bgm, std::string content_bg, std::string content_portal, std::string tile_theme, std::string hit_sound, std::string bgm_artist, std::string bgm_name,
		  TEXT_COLOR text_color, unsigned short bpm, float bgm_offset, unsigned short count_ticks, bool isCleared);
	virtual ~Level();
	virtual void Start() override;
	virtual bool Update() override;
	virtual void End() override;

	virtual void Import();

public:
	virtual bool IsAutoPlay() const final;
	virtual void SetIsRestartable(bool isRestartable);
	virtual void CountProgress();
	virtual void CountDecisionMiss(char value);
	virtual void RewindPlayTimer(float rewind);

protected:
	virtual void PlayLevel() final;
	virtual void Countdown() final;
	virtual void CompletionPercentage() final;
	virtual void AutoPlaySwitch() final;
	virtual void Restart() final;
};