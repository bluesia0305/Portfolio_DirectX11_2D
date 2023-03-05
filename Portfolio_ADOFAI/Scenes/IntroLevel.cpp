#include "stdafx.h"
#include "IntroLevel.h"

IntroLevel::IntroLevel(unsigned short bpm, float movement_unit) : Scene(bpm, movement_unit)
{
	ContentBGM   = "bgm_intro_level";
	ContentBG    = "bg_intro_level";
	BGM->Content = ContentBGM.c_str();
	BGM->Loop	 = true;

	Map			 = new Map::IntroMap();
	Portal1		 = new Object::APortal({  936, -72 }, "text_intro_nametag_level1", "portal_sample1", "(매우 어려움) 16비트 리듬을 즐길 수 있는 빠른 BPM의 노래");
	Portal2		 = new Object::APortal({ 1440, -72 }, "text_intro_nametag_level2", "portal_sample2", "<Level2 임시 텍스트>");
	
	MainTextures.push_back(new Object::ASimpleImage(Image::Component{ "text_intro_title",		  vector<2>{    0.0f,  210.0f }, vector<2>{ 479.808f, 140.04f }, VEC_ANGLE_NULL, 1.0f }));
	MainTextures.push_back(new Object::ASimpleImage(Image::Component{ "text_intro_announcement",  vector<2>{  380.0f,  0.0f   }, vector<2>{ 349.344f, 56.376f }, VEC_ANGLE_NULL, 1.0f }));
	SubTextures.push_back (new Object::ASimpleImage(Image::Component{ "text_intro_calibration",	  vector<2>{ -295.0f, -144.0f }, vector<2>{ 66.312f,  47.52f  }, VEC_ANGLE_NULL, 0.0f }));
	SubTextures.push_back (new Object::ASimpleImage(Image::Component{ "text_intro_leveleditor",	  vector<2>{ -360.0f,  144.0f }, vector<2>{ 184.824f, 47.52f  }, VEC_ANGLE_NULL, 0.0f }));
	SubTextures.push_back (new Object::ASimpleImage(Image::Component{ "text_intro_customlevel",	  vector<2>{  355.0f,  144.0f }, vector<2>{ 184.824f, 47.52f  }, VEC_ANGLE_NULL, 0.0f }));
	SubTextures.push_back (new Object::ASimpleImage(Image::Component{ "text_intro_customsetting", vector<2>{  576.0f,  72.0f  }, vector<2>{ 184.824f, 47.52f  }, VEC_ANGLE_NULL, 0.0f }));
	SubTextures.push_back (new Object::ASimpleImage(Image::Component{ "object_musedash_rin",	  vector<2>{  596.16f, 0.0f   }, vector<2>{ 89.8416f, 122.4f  }, VEC_ANGLE_NULL, 0.0f }));
	SubTextures.push_back (new Object::ASimpleImage(Image::Component{ "object_musedash_marija",	  vector<2>{  522.0f, -48.24f }, vector<2>{ 83.2896f, 115.2f  }, VEC_ANGLE_NULL, 0.0f }));
	SubTextures.push_back (new Object::ASimpleImage(Image::Component{ "object_musedash_buro",	  vector<2>{  637.2f, -54.72f }, vector<2>{ 73.476f,  93.6f   }, VEC_ANGLE_NULL, 0.0f }));
	TitleEffects.push_back(new Object::ASimpleImage(Image::Component{ "effect_title_glow",		  vector<2>{ -172.0f,  177.0f }, vector<2>{ 220.0f,   132.0f  }, VEC_ANGLE_NULL, 1.0f }));
	TitleEffects.push_back(new Object::ASimpleImage(Image::Component{ "effect_title_glow",		  vector<2>{  134.0f,  177.0f }, vector<2>{ 353.0f,   132.0f  }, VEC_ANGLE_NULL, 1.0f }));

	isStarted = true;
	isLoaded  = false;
	isCenter  = true;
}

IntroLevel::~IntroLevel()
{
	delete Map;
	delete Portal1;
	delete Portal2;

	for (auto elem : MainTextures)
		delete elem;

	for (auto elem : SubTextures)
		delete elem;

	for (auto elem : TitleEffects)
		delete elem;
}

void IntroLevel::Start()
{
	isPlayable = true;

	CAMERA->Start();
	Player->Start();
	Map->Start();
	Portal1->Start();
	Portal2->Start();

	for (auto& elem : MainTextures)
		elem->SetAlphaLevel(1.0f);

	for (auto& elem : TitleEffects)
		elem->SetAlphaLevel(0.0f);

	for (auto& elem : SubTextures)
		elem->SetAlphaLevel(0.0f);

	CountdownIndex = 0;
	CountdownTimer = 0.0f;
	BGMTimer	   = 0.0f;
}

bool IntroLevel::Update()
{
	// Ignore Scene initialization delay
	if (DELTA_TIME > 0.1f) { isLoaded = true; return true; }
	
		
	if (BGMTimer >= 0)
	{
		BGMTimer	   += DELTA_TIME;
		CountdownTimer += DELTA_TIME;
	}

	if (CountdownTimer > SECONDS_PER_BEAT &&
		CountdownIndex < CountTicks + 1)
		++CountdownIndex;

	if (CountdownIndex < CountTicks + 1)
	{
		if (CountdownTimer > SECONDS_PER_BEAT)
			SOUND("HatClosed")->Play();
	}

	AdjustFloat(CountdownTimer, SECONDS_PER_BEAT);

	if (BGMTimer > (CountTicks + 1) * SECONDS_PER_BEAT + BGMOffset)
	{
		BGM->Play();
		BGMTimer = -1.0f;
	}
	

	// Move	-> InputComponent 사용으로 변경
	if (!CAMERA->IsClosing() && !CAMERA->IsOpening())
	{
		if (Input::Get::Key::Down(0x30) ||
			Input::Get::Key::Down(0xC0))
		{
			if (L1Distance(Player->GetPosition() - VEC_POSITION_ORIGIN))
			{
				Player->SetPosition(VEC_POSITION_ORIGIN);
				CAMERA->Move(VEC_POSITION_ORIGIN);
			}
			else
			{
				Player->SetPosition({ 576, -144 });
				CAMERA->Move({ 576, -144 });
			}
		}
		if (Input::Get::Key::Down(0x31))	{ Player->SetPosition(vector<2>{  936, -144 }); CAMERA->Move(Player->GetPosition() + vector<2>{ 0, 216 }); }
		if (Input::Get::Key::Down(0x32))	{ Player->SetPosition(vector<2>{ 1440, -144 }); CAMERA->Move(Player->GetPosition() + vector<2>{ 0, 216 }); }
		if (InputProcess())
		{
			Player->Move(Map);
			if (Player->GetPosition()[0] < 216.0f)
			{
				CAMERA->Move(VEC_POSITION_ORIGIN);
				StartPos = VEC_POSITION_ORIGIN;
			}
			else if (Player->GetPosition()[1] < -72.0f)
				CAMERA->Move(Player->GetPosition() + vector<2>{ 0, 216 });
		}
	}

	// Save Starting Position
	if (!L1Distance((Player->GetPosition() - vector<2>{  936, -144 }))) { StartPos = Player->GetPosition(); }
	if (!L1Distance((Player->GetPosition() - vector<2>{ 1440, -144 }))) { StartPos = Player->GetPosition(); }


	// ##### Update / Draw #####
	CAMERA->Update();
	if (!L1Distance(Player->GetPosition())) { isCenter = true;  dynamic_cast<Map::IntroMap*>(Map)->SetShowAll(false); }
	else								    { isCenter = false; dynamic_cast<Map::IntroMap*>(Map)->SetShowAll(true);  }
	
	ControlTexturesAlphaLevel(4.0f * DELTA_TIME);
	for (auto elem : SubTextures)
		elem->Update();

	Map->Update();
	Portal1->Update();
	Portal2->Update();
	Player->Update();

	if (BGMTimer >= 0 && BGMTimer < SECONDS_PER_BEAT)
	{
		for (auto elem : TitleEffects)
			elem->SetLength(vector<2>{ 0, 0 });
	}
	else
		TitleGlow();

	for (auto elem : TitleEffects)
		elem->Update();
	for (auto elem : MainTextures)
		elem->Update();


	// Camera closer control / scene change
	CAMERA->CameraOpening();
	if (!L1Distance((Player->GetPosition() - vector<2>{  576, -72  }))) { BGM->Stop(); isClosing = true; NextScene = "Customizing"; }
	if (!L1Distance((Player->GetPosition() - vector<2>{ -216, -144 }))) { BGM->Stop(); isClosing = true; NextScene = "Calibration"; }
	if (!L1Distance((Player->GetPosition() - vector<2>{ -216,  144 }))) { BGM->Stop(); isClosing = true; NextScene = "LevelEditor"; }
	if (!L1Distance((Player->GetPosition() - vector<2>{  216,  144 }))) { BGM->Stop(); isClosing = true; NextScene = "CustomLevel"; }
	if (isClosing)
		if (!CAMERA->CameraClosing()) INSTANCE(SceneManager)->ChangeScene(NextScene);
	CAMERA->PresentCameraCloser();
	return true;
}

void IntroLevel::End()
{
	isClosing = false;
	Player->End();
}

void IntroLevel::ControlTexturesAlphaLevel(float increment)
{
	float diff = 0.0f;

	for (auto elem : MainTextures)
	{
		if (isCenter)
			diff = elem->GetAlphaLevel() + increment;
		else
			diff = elem->GetAlphaLevel() - increment;
		elem->SetAlphaLevel(BoundedFloat(diff, 0.0f, 1.0f));
	}

	for (auto elem : TitleEffects)
	{
		if (isCenter)
			diff = elem->GetAlphaLevel() + 0.5f * increment;
		else
			diff = elem->GetAlphaLevel() - 0.5f * increment;
		elem->SetAlphaLevel(BoundedFloat(diff, 0.0f, 0.5f));
	}

	for (auto & elem : SubTextures)
	{
		if (isCenter)
			diff = elem->GetAlphaLevel() - increment;
		else
			diff = elem->GetAlphaLevel() + increment;
		elem->SetAlphaLevel(BoundedFloat(diff, 0.0f, 1.0f));
	}
}

void IntroLevel::TitleGlow()
{
	float lengthLevel = RhythmicalFluctuation(0.1f, 1.0f);
	TitleEffects[0]->SetLength({ 200.0f * lengthLevel, 120.0f * lengthLevel });
	TitleEffects[1]->SetLength({ 330.0f * lengthLevel, 120.0f * lengthLevel });
}