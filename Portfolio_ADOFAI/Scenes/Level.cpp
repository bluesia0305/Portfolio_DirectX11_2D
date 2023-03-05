#include "stdafx.h"
#include "Level.h"

#define DOWNCASTED_MAP			dynamic_cast<Map::UnidirectionalMap*>(Map)

Level::Level(unsigned short bpm, float movement_unit) : Scene(bpm, movement_unit)
{
	LevelFile		  = "";
	ContentBGM		  = "";
	ContentBG		  = "";
	ContentPortal	  = "";
	BGMArtist		  = "";
	BGMName			  = "";
	TileTheme		  = "neon";
	HitSound		  = "KickBasic";
					 
	Map				  = new Map::UnidirectionalMap();
	OperationMode	  = OPERATION_MODE::PLAY_MODE;

	TextColor		  = { 255, 255, 255 };
	TextLevelTitle	  = Text::Component{ nullptr,				{ 640, 80 },  { "godoMaum", 79 },  { TextColor.R, TextColor.G, TextColor.B } };
	TextReady		  = Text::Component{ "아무 키나 눌러 시작",	{ 640, 270 }, { "godoMaum", 132 }, { TextColor.R, TextColor.G, TextColor.B } };
	TextStandBy		  = Text::Component{ "준비!",				{ 640, 270 }, { "godoMaum", 132 }, { TextColor.R, TextColor.G, TextColor.B } };
	TextCountdown[0]  = Text::Component{ "시작!",				{ 640, 270 }, { "godoMaum", 132 }, { TextColor.R, TextColor.G, TextColor.B } };
	TextCountdown[1]  = Text::Component{ "1!",					{ 640, 270 }, { "godoMaum", 132 }, { TextColor.R, TextColor.G, TextColor.B } };
	TextCountdown[2]  = Text::Component{ "2!",					{ 640, 270 }, { "godoMaum", 132 }, { TextColor.R, TextColor.G, TextColor.B } };
	TextCountdown[3]  = Text::Component{ "3!",					{ 640, 270 }, { "godoMaum", 132 }, { TextColor.R, TextColor.G, TextColor.B } };
	TextCountdown[4]  = Text::Component{ "4!",					{ 640, 270 }, { "godoMaum", 132 }, { TextColor.R, TextColor.G, TextColor.B } };
	TextCountdown[5]  = Text::Component{ "5!",					{ 640, 270 }, { "godoMaum", 132 }, { TextColor.R, TextColor.G, TextColor.B } };
	TextCountdown[6]  = Text::Component{ "6!",					{ 640, 270 }, { "godoMaum", 132 }, { TextColor.R, TextColor.G, TextColor.B } };
	TextCountdown[7]  = Text::Component{ "7!",					{ 640, 270 }, { "godoMaum", 132 }, { TextColor.R, TextColor.G, TextColor.B } };
	TextEnded		  = Text::Component{ "축하합니다!",			{ 640, 270 }, { "godoMaum", 132 }, { TextColor.R, TextColor.G, TextColor.B } };

	TextOverload	  = Text::Component{ "과부하!",				{ 640, 270 }, { "godoMaum", 132 }, { TextColor.R, TextColor.G, TextColor.B } };
	TextCompletion	  = Text::Component{ nullptr,				{ 640, 550 }, { "godoMaum", 132 }, { TextColor.R, TextColor.G, TextColor.B } };

	isAutoPlay		  = false;
	isRestartable	  = false;
	isCleared		  = false;
	
	Percentage		  = "";
	SteppedTileCount  = 1;
	DecisionMissCount = 0;
}

Level::Level(std::string level_file, std::string content_bgm, std::string content_bg, std::string content_portal, std::string tile_theme, std::string hit_sound, std::string bgm_artist, std::string bgm_name,
			 TEXT_COLOR text_color, unsigned short bpm, float bgm_offset, unsigned short count_ticks, bool isCleared) : Scene(bpm, MOVEMENT_UNIT_LEVEL)
{
	LevelFile		  = level_file;
	ContentBGM		  = (content_bgm	   == "__NULL__" ? "" : content_bgm);
	ContentBG		  = (content_bg	   == "__NULL__" ? "" : content_bg);
	ContentPortal	  = (content_portal == "__NULL__" ? "" : content_portal);
	TileTheme		  = tile_theme;
	HitSound		  = hit_sound;
	BGMArtist		  = (bgm_artist	   == "__NULL__" ? "" : bgm_artist);
	BGMName			  = (bgm_name	   == "__NULL__" ? "" : bgm_name);

	BGMOffset		  = bgm_offset;
	CountTicks		  = count_ticks;

	if (ContentBGM != "")
		BGM->Content = ContentBGM.c_str();

	Map				  = new Map::UnidirectionalMap();
	OperationMode	  = OPERATION_MODE::PLAY_MODE;

	TextColor		  = text_color;
	TextLevelTitle	  = Text::Component{ nullptr,				{ 640, 80 },  { "godoMaum", 79 },  { TextColor.R, TextColor.G, TextColor.B } };
	TextReady		  = Text::Component{ "아무 키나 눌러 시작",	{ 640, 270 }, { "godoMaum", 132 }, { TextColor.R, TextColor.G, TextColor.B } };
	TextStandBy		  = Text::Component{ "준비!",				{ 640, 270 }, { "godoMaum", 132 }, { TextColor.R, TextColor.G, TextColor.B } };
	TextCountdown[0]  = Text::Component{ "시작!",				{ 640, 270 }, { "godoMaum", 132 }, { TextColor.R, TextColor.G, TextColor.B } };
	TextCountdown[1]  = Text::Component{ "1!",					{ 640, 270 }, { "godoMaum", 132 }, { TextColor.R, TextColor.G, TextColor.B } };
	TextCountdown[2]  = Text::Component{ "2!",					{ 640, 270 }, { "godoMaum", 132 }, { TextColor.R, TextColor.G, TextColor.B } };
	TextCountdown[3]  = Text::Component{ "3!",					{ 640, 270 }, { "godoMaum", 132 }, { TextColor.R, TextColor.G, TextColor.B } };
	TextCountdown[4]  = Text::Component{ "4!",					{ 640, 270 }, { "godoMaum", 132 }, { TextColor.R, TextColor.G, TextColor.B } };
	TextCountdown[5]  = Text::Component{ "5!",					{ 640, 270 }, { "godoMaum", 132 }, { TextColor.R, TextColor.G, TextColor.B } };
	TextCountdown[6]  = Text::Component{ "6!",					{ 640, 270 }, { "godoMaum", 132 }, { TextColor.R, TextColor.G, TextColor.B } };
	TextCountdown[7]  = Text::Component{ "7!",					{ 640, 270 }, { "godoMaum", 132 }, { TextColor.R, TextColor.G, TextColor.B } };
	TextEnded		  = Text::Component{ "축하합니다!",			{ 640, 270 }, { "godoMaum", 132 }, { TextColor.R, TextColor.G, TextColor.B } };

	TextOverload	  = Text::Component{ "과부하!",				{ 640, 270 }, { "godoMaum", 132 }, { TextColor.R, TextColor.G, TextColor.B } };
	TextCompletion	  = Text::Component{ nullptr,				{ 640, 550 }, { "godoMaum", 132 }, { TextColor.R, TextColor.G, TextColor.B } };
	
	if (BGMArtist == "")
		LevelTitle = BGMName;
	else
		LevelTitle = BGMArtist + " - " + BGMName;

	if (LevelTitle != "")
		TextLevelTitle.String = LevelTitle.c_str();

	PlayTimer		  = 0.0f;

	isAutoPlay		  = false;
	isRestartable	  = false;
	this->isCleared	  = isCleared;

	Percentage		  = "";
	SteppedTileCount  = 1;
	DecisionMissCount = 0;
}

Level::~Level()
{
	delete Map;
}

void Level::Start()
{
	CAMERA->Start();
	Player->Start();
	Map->Start();

	if (BGMArtist == "")
	{
		if (BGMName != "")
			TextLevelTitle.String = BGMName.c_str();
	}
	else if (BGMName != "")
		LevelTitle = BGMArtist + " - " + BGMName;

	if (LevelTitle != "")
		TextLevelTitle.String = LevelTitle.c_str();

	if (BGM->Content) BGM->SetVolume(1.0f);

	CountdownIndex = 0;
	CountdownTimer = 0.0f;
	BGMTimer	   = 0.0f;
	PlayTimer	   = 0.0f;

	isEnded		   = false;
	isClosing	   = false;
	isStarted	   = false;
	isPlayable	   = false;
	isRestartable  = false;
	isAutoPlay	   = false;

	SteppedTileCount  = 1;
	DecisionMissCount = 0;

	NextScene = "";
}

bool Level::Update()
{
	CAMERA->Update();
	Map->Update();
	Player->Update();

	if (!CAMERA->IsClosing() && !CAMERA->IsOpening() && Scene::InputProcess())
		isStarted = true;

	if (isPlayable && !isEnded && !isRestartable)
		PlayTimer += DELTA_TIME;

	// AutoPlay on/off
	if (Input::Get::Key::Down(VK_F11))
		AutoPlaySwitch();

	if (isEnded && InputProcess())
	{
		isClosing = true;
		INSTANCE(SoundManager)->Shutdown();
		if (BGM->Content) BGM->Stop();
		NextScene = "CustomLevel";
	}

	// Player move & explosion
	if (isPlayable)
		PlayLevel();

	Countdown();

	if (isRestartable)
		Restart();

	if (DecisionMissCount > 3)
		TextOverload.Draw();

	if (DecisionMissCount > 3 || !Player->IsExplodeable())
	{
		Percentage = std::to_string(static_cast<int>(round(static_cast<float>(SteppedTileCount) / (DOWNCASTED_MAP->GetNumTiles()) * 100.0f))) + "% 완료";
		TextCompletion.String = Percentage.c_str();
		TextCompletion.Draw();
	}

	// If reached to the end tile -> End
	if (!isEnded)
		if (DOWNCASTED_MAP->GetCurrentTile() == DOWNCASTED_MAP->GetEndTile())
		{
			isEnded	  = true;
			isCleared = true;
			SOUND("LevelClear")->Play();
		}

	if (isEnded)
	{
		if (BGM->Content) BGM->GradualVolumeDown(0.2f);
		TextEnded.Draw();
	}

	if (TextLevelTitle.String)
		TextLevelTitle.Draw();

	// Camera closer control / scene change
	CAMERA->CameraOpening();
	if (CAMERA->IsCloseable() && !CAMERA->IsOpening())
		if (Input::Get::Key::Down(VK_F12))
		{
			isStarted  = false;
			isPlayable = false;
			isClosing  = true;
			INSTANCE(SoundManager)->Shutdown();
			if (BGM->Content) BGM->Stop();
			NextScene = "CustomLevel";
		}

	if (isClosing)
	{
		if (NextScene != "")
		{
			if (!CAMERA->CameraClosing())
			{
				isClosing = false;
				INSTANCE(SceneManager)->ChangeScene(NextScene);
			}
		}
		else if (!CAMERA->CameraClosing())
		{
			isClosing = false;
			Start();
		}
	}

	if (isFadingIn)
		if (!CAMERA->CameraFadeIn()) isFadingIn = false;

	CAMERA->PresentCameraCloser();
	return true;
}

void Level::End()
{
	if (BGM->Content) BGM->Stop();
	SOUND("LevelClear")->Stop();

	if (isCleared)
		INSTANCE(DataManager)->UpdateLevel(LevelFile);
}

bool Level::IsAutoPlay() const						{ return isAutoPlay; }
void Level::SetIsRestartable(bool isRestartable)	{ this->isRestartable = isRestartable; }
void Level::CountProgress()							{ ++SteppedTileCount; }
void Level::CountDecisionMiss(char value)			{ DecisionMissCount += value; if (DecisionMissCount < 0) ++DecisionMissCount; }
void Level::RewindPlayTimer(float rewind)			{ PlayTimer -= rewind; }

void Level::Import()
{
	DOWNCASTED_MAP->Import();
	DOWNCASTED_MAP->LinkTiles();
}

void Level::PlayLevel()
{
	Object::AUnidirectionalTile* pTile = dynamic_cast<Object::AUnidirectionalTile*>(DOWNCASTED_MAP->GetCurrentTile());
	float bent_angle = pTile->GetBentAngle();
	float range_of_revolution = 0.0f;

	if (bent_angle == 180.0f && !pTile->IsPassingTile())
		range_of_revolution = 360.0f;
	else
		range_of_revolution = 180.0f + Player->GetAngleDirection() * bent_angle;

	if (!isEnded)
	{
		if (isAutoPlay)
		{
			if (PlayTimer > range_of_revolution / 180.0f * SECONDS_PER_BEAT / Player->GetMagnification())
			{
				Player->Move(Map);
				if (!dynamic_cast<Object::AUnidirectionalTile*>(DOWNCASTED_MAP->GetPrevTile(DOWNCASTED_MAP->GetCurrentTile()))->IsPassingTile())
					CAMERA->Move(Player->GetPosition());
			}
		}
		else if (Scene::InputProcess())
		{
			Player->Move(Map);
			if (!dynamic_cast<Object::AUnidirectionalTile*>(DOWNCASTED_MAP->GetPrevTile(DOWNCASTED_MAP->GetCurrentTile()))->IsPassingTile())
				CAMERA->Move(Player->GetPosition());
		}
	}

	if (DecisionMissCount > 3 ||
		PlayTimer > (range_of_revolution + 180.0f) / 180.0f * SECONDS_PER_BEAT / Player->GetMagnification())
	{
		Player->Explode();
		isPlayable = false;
	}
}

void Level::Countdown()
{
	if (!isStarted)
		TextReady.Draw();
	else
	{
		if (BGMTimer >= 0)
		{
			BGMTimer += DELTA_TIME;
			CountdownTimer += DELTA_TIME;
		}
		else return;

		if (CountdownTimer > SECONDS_PER_BEAT &&
			CountdownIndex < CountTicks + 1)
			++CountdownIndex;

		if (CountdownIndex == 0)
			TextStandBy.Draw();
		else if (CountdownIndex < CountTicks + 1)
		{
			if (CountdownTimer > SECONDS_PER_BEAT)
				SOUND("HatClosed")->Play();
			TextCountdown[CountTicks - CountdownIndex].Draw();
		}
		if (CountdownIndex == CountTicks)
			isPlayable = true;

		AdjustFloat(CountdownTimer, SECONDS_PER_BEAT);

		if (BGMTimer > (CountTicks + 1) * SECONDS_PER_BEAT + BGMOffset)
		{
			if (BGM->Content) BGM->Play();
			BGMTimer = -1.0f;
		}
	}
}

void Level::CompletionPercentage()
{
	Percentage = std::to_string(static_cast<int>(round(static_cast<float>(SteppedTileCount) / (DOWNCASTED_MAP->GetNumTiles()) * 100.0f))) + "% 완료";
	TextCompletion.String = Percentage.c_str();
	TextCompletion.Draw();
}

void Level::AutoPlaySwitch()
{
	if (isAutoPlay)
	{
		SOUND("OttoDeactivate")->Play();
		isAutoPlay = false;
	}
	else
	{
		SOUND("OttoActivate")->Play();
		isAutoPlay = true;
	}
}

void Level::Restart()
{
	if (BGM->Content)
		BGM->GradualVolumeDown(1.0f);

	if (InputProcess())
	{
		if (BGM->Content) BGM->Stop();
		SOUND("PlanetExplosion1")->Stop();
		SOUND("PlanetExplosion2")->Stop();
		isRestartable = false;
		
		Map->End();

		if (dynamic_cast<LevelEditor*>(this))
		{
			isFadingIn = true;
			CAMERA->CameraFadeIn();
			Start();
		}
		else
		{
			isClosing = true;
			CAMERA->CameraClosing();
		}
	}
}