#include "stdafx.h"
#include "Customizing.h"

Customizing::Customizing(unsigned short bpm, float movement_unit) : Scene(bpm, movement_unit)
{
	ContentBGM	  = "bgm_intro_level";
	ContentBG	  = "bg_intro_level";
	BGM->Content  = ContentBGM.c_str();
	BGM->Loop	  = true;

	Map			  = new Map::CustomizingMap();
	StartPos	  = { 0, -216 };
	
	Announcement  = Image::Component{ "text_customizing_announcement", { 0, 290 }, { 679.856f, 79.394f }, VEC_ANGLE_NULL, 1.0f };
	Characters[0] = new Object::ASimpleImage(Image::Component{ "object_musedash_rin",	 { -370,  136 }, { 179.683f, 244.8f  }, VEC_ANGLE_NULL, 1.0f });
	Characters[1] = new Object::ASimpleImage(Image::Component{ "object_musedash_marija", { -450, -179 }, { 166.579f, 230.4f  }, VEC_ANGLE_NULL, 1.0f });
	Characters[2] = new Object::ASimpleImage(Image::Component{ "object_musedash_buro",	 {  425, -69  }, { 146.952,  187.2f  }, VEC_ANGLE_NULL, 1.0f });
	Mokokos[0]	  = new Object::ASimpleImage(Image::Component{ "planet_mokoko_base",	 { -288, -144 }, VEC_LENGTH_PLANET,		VEC_ANGLE_NULL, 0.5f });
	Mokokos[1]	  = new Object::ASimpleImage(Image::Component{ "planet_mokoko_base",	 { -288, -72  }, VEC_LENGTH_PLANET,		VEC_ANGLE_NULL, 0.5f });
	Mokokos[2]	  = new Object::ASimpleImage(Image::Component{ "planet_mokoko_blue",	 { -288,  0   }, VEC_LENGTH_PLANET,		VEC_ANGLE_NULL, 0.5f });
	Mokokos[3]	  = new Object::ASimpleImage(Image::Component{ "planet_mokoko_blue",	 { -288,  72  }, VEC_LENGTH_PLANET,		VEC_ANGLE_NULL, 0.5f });
	Mokokos[4]	  = new Object::ASimpleImage(Image::Component{ "planet_mokoko_pink",	 {  288, -144 }, VEC_LENGTH_PLANET,		VEC_ANGLE_NULL, 0.5f });
	Mokokos[5]	  = new Object::ASimpleImage(Image::Component{ "planet_mokoko_pink",	 {  288, -72  }, VEC_LENGTH_PLANET,		VEC_ANGLE_NULL, 0.5f });
	Mokokos[6]	  = new Object::ASimpleImage(Image::Component{ "planet_mokoko_soda",	 {  288,  0   }, VEC_LENGTH_PLANET,		VEC_ANGLE_NULL, 0.5f });
	Mokokos[7]	  = new Object::ASimpleImage(Image::Component{ "planet_mokoko_soda",	 {  288,  72  }, VEC_LENGTH_PLANET,		VEC_ANGLE_NULL, 0.5f });
	Rotation	  = new Object::ARotatingImage(Image::Component{ "icon_swirl_blue",		 {    0,  144 }, { 74.8f, 74.8f },		VEC_ANGLE_NULL, 1.0f });
	CometTail	  = new ObjectPool<Object::ACometTail>;
}

Customizing::~Customizing()
{
	delete Map;

	for (auto elem : Characters) delete elem;
	for (auto elem : Mokokos)	 delete elem;
	delete Rotation;
	delete CometTail;
}

void Customizing::Start()
{
	CAMERA->Start();
	CAMERA->SetPosition({ 0, 36 });
	BGM->Play();

	Player->Start();
	Map->Start();
	
	CometTailRevAngle = 0.0f;
	CometTailTimer1   = 0.0f;
	CometTailTimer2   = 0.0f;

	isStarted	   = true;
	isClosing	   = false;
	isCustomizable = true;
	isEscapable    = false;
}

bool Customizing::Update()
{
	// Move	-> InputComponent 사용으로 변경
	if (!CAMERA->IsClosing() && !CAMERA->IsOpening())
	{
		if (InputProcess())
		{
			vector<2> tempPos = Player->GetPosition();
			Player->Move(Map);

			if (L1Distance(Player->GetPosition() - tempPos))
			{
				isCustomizable = true;
				isEscapable = true;
			}
		}
	}

	// Comet tail settings
	DisplayCometTail();

	// Customizing - planets / comet tails / rotation
	CustomizePlanet({ -288, -144 }, "planet_mokoko_base");
	CustomizePlanet({ -288, -72  }, "planet_mokoko_base");
	CustomizePlanet({ -288,  0   }, "planet_mokoko_blue");
	CustomizePlanet({ -288,  72  }, "planet_mokoko_blue");
	CustomizePlanet({  288, -144 }, "planet_mokoko_pink");
	CustomizePlanet({  288, -72  }, "planet_mokoko_pink");
	CustomizePlanet({  288,  0   }, "planet_mokoko_soda");
	CustomizePlanet({  288,  72  }, "planet_mokoko_soda");
	CustomizeCometTail({ -144, -216 }, "comet_tail_glittering_yellow", false);
	CustomizeCometTail({ -144,  144 }, "comet_tail_glittering_none", true);
	CustomizeCometTail({  144, -216 }, "comet_tail_simple_base", true);
	CustomizeCometTail({  144,  144 }, "comet_tail_mokoko", false);
	CustomizeRotation();

	// ##### Update / Draw #####
	CAMERA->Update();
	for (auto& elem : Characters) elem->Update();
	Announcement.Draw();
	Map->Update();
	Rotation->Update();
	for (auto& elem : Mokokos)	  elem->Update();
	CometTail->Update();
	if (isStarted) Player->Update();

	// Camera closer control / scene change
	CAMERA->CameraOpening();
	if (!L1Distance((Player->GetPosition() - vector<2>{ 0, -216 })) && isEscapable) { BGM->Stop(); isClosing = true; NextScene = "IntroLevel"; }
	if (isClosing)
		if (!CAMERA->CameraClosing()) INSTANCE(SceneManager)->ChangeScene(NextScene);
	CAMERA->PresentCameraCloser();
	return true;
}

void Customizing::End()
{
	isClosing = false;
	Player->End();
}

void Customizing::DisplayCometTail()
{
	CometTailTimer1 += DELTA_TIME;
	CometTailTimer2 += DELTA_TIME;
	CometTailRevAngle -= GetBPM() * 3.0f * DELTA_TIME;
	if (CometTailTimer1 > 0.10f)
	{
		CometTail->GetRecycledObject()->Display({ -144, -216 }, { 15, 30 }, 20.0f, CometTailRevAngle, "comet_tail_glittering_yellow");
		CometTail->GetRecycledObject()->Display({ -144,  144 }, { 15, 30 }, 20.0f, CometTailRevAngle, "comet_tail_glittering_none");
		CometTail->GetRecycledObject()->Display({  144,  144 }, { 15, 30 }, 20.0f, CometTailRevAngle, "comet_tail_mokoko");
		AdjustFloat(CometTailTimer1, 0.10f);
	}
	if (CometTailTimer2 > 0.0005f)
	{
		CometTail->GetRecycledObject()->Display({  144, -216 }, { 15, 30 }, 20.0f, CometTailRevAngle + 15.0f, "comet_tail_simple_base");
		AdjustFloat(CometTailTimer2, 0.05f);
	}
}

void Customizing::CustomizePlanet(vector<2> position, std::string content)
{
	if (!L1Distance((Player->GetPosition() - position)))
	{
		if (isCustomizable)
		{
			isCustomizable = false;
			bool isNeedSync = Player->GetCometTailContent(0).substr(Player->GetCometTailContent(0).size() - 6) == "yellow" ||
							  Player->GetCometTailContent(0).substr(Player->GetCometTailContent(0).size() - 6) == "mokoko" ? false : true;

			Player->ChangePlanetContent(NormL1Distance(position - StartPos) % 2, content, isNeedSync);
			Player->Save();
		}
	}
}

void Customizing::CustomizeCometTail(vector<2> position, std::string content, bool isNeedSync)
{
	if (!L1Distance((Player->GetPosition() - position)))
	{
		if (isCustomizable)
		{
			isCustomizable = false;
			Player->ChangeCometTailContent(content, isNeedSync);
			Player->Save();
		}
	}
}

void Customizing::CustomizeRotation()
{
	if (!L1Distance((Player->GetPosition() - vector<2>{    0,  144 })))
	{
		if (isCustomizable)
		{
			isCustomizable = false;
			if (Player->IsRotating()) Player->SetIsRotating(false);
			else					  Player->SetIsRotating(true);
			Player->Save();
		}
	}
}