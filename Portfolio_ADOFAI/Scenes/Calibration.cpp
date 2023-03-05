#include "stdafx.h"
#include "Calibration.h"

Calibration::Calibration(unsigned short bpm, float movement_unit) : Scene(bpm, movement_unit)
{
	ContentBGM		 = "bgm_intro_level";
	ContentBG		 = "bg_calibration";
	BGM->Content	 = ContentBGM.c_str();

	DottedLine		 = Image::Component{ "effect_calibration_line",	{ 0, 0 }, { 1280.0f, 5.34f }, VEC_ANGLE_NULL, 1.0f };
	Announcements[0] = Text::Component{ "기기 보정을 시작합니다. 키를 눌러 시작하세요!",	{ 640, 90 },  { "godoMaum", 70 }, COLOR_PURPLE };
	Announcements[1] = Text::Component{ "선 기울기 상관없이 드럼 박자에 맞춰 입력하세요.",	{ 640, 90 },  { "godoMaum", 70 }, COLOR_PURPLE };
	Announcements[2] = Text::Component{ "아무 때에 시작하셔도 됩니다.",					{ 640, 155 }, { "godoMaum", 70 }, COLOR_PURPLE };
	Announcements[3] = Text::Component{ "(현재 Calibration 미구현 상태입니다)",			{ 640, 565 }, { "godoMaum", 70 }, COLOR_PURPLE };
}

void Calibration::Start()
{
	isPlayable = true;

	CAMERA->Start();
	Player->Start();
	BGMOffset = SECONDS_PER_BEAT;
	CountdownIndex = 0;
}

bool Calibration::Update()
{
	CAMERA->Update();

	// ##### Update / Draw #####
	DottedLine.Draw();
	Player->Update();
	
	if (!CAMERA->IsClosing() && !CAMERA->IsOpening() && Scene::InputProcess())
		isStarted = true;

	if (!isStarted)
		Announcements[0].Draw();
	else
	{
		Announcements[1].Draw();
		Announcements[2].Draw();

		if (BGMTimer >= 0)
			BGMTimer += DELTA_TIME;

		if (BGMTimer > BGMOffset)
		{
			BGM->Play();
			BGMTimer = -1.0f;
		}
	}
	Announcements[3].Draw();


	// Camera closer control / scene change
	CAMERA->CameraOpening();
	if (CAMERA->IsCloseable() && !CAMERA->IsOpening())
		if (Input::Get::Key::Down(VK_F12)) { BGM->Stop(); isClosing = true; NextScene = "IntroLevel"; }
	
	if (isClosing)
		if (!CAMERA->CameraClosing()) INSTANCE(SceneManager)->ChangeScene(NextScene);
	CAMERA->PresentCameraCloser();
	return true;
}

void Calibration::End()
{
	isClosing = false;
	Player->End();
}