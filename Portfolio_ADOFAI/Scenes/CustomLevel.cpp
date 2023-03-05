#include "stdafx.h"
#include "CustomLevel.h"

CustomLevel::CustomLevel(unsigned short bpm, float movement_unit, int num_custom_levels) : Scene(bpm, movement_unit)
{
	ContentBGM	 = "bgm_intro_level";
	ContentBG	 = "bg_intro_level";
	BGM->Content = ContentBGM.c_str();
	BGM->Loop	 = true;

	Map			 = new Map::CustomLevelMap(num_custom_levels);

	isStarted	 = true;
	isClosing	 = false;

	NumCustomLevels = num_custom_levels;
}

CustomLevel::~CustomLevel()
{
	delete Map;
}

void CustomLevel::Start()
{
	CAMERA->Start();
	CAMERA->SetPosition({ -360, StartPos[1] });
	BGM->Play();

	Player->Start();
	Map->Start();

	isStarted = true;
	isClosing = false;
}

bool CustomLevel::Update()
{
	// Move	-> InputComponent 사용으로 변경
	if (!CAMERA->IsClosing() && !CAMERA->IsOpening())
	{
		if (InputProcess())
		{
			Player->Move(Map);
			if (Player->GetPosition()[1] < 72 &&
				Player->GetPosition()[1] > -72 * NumCustomLevels)
				CAMERA->Move({ -360, Player->GetPosition()[1] });
		}
	}

	// ##### Update / Draw #####
	CAMERA->Update();
	Map->Update();
	if (isStarted) Player->Update();

	// Camera closer control / scene change
	CAMERA->CameraOpening();
	if (!L1Distance((Player->GetPosition() - vector<2>{ 0, 72 })))						 { BGM->Stop(); isClosing = true; NextScene = "IntroLevel"; }
	if (!L1Distance((Player->GetPosition() - vector<2>{ 0, 0 - 72 * NumCustomLevels }))) { BGM->Stop(); isClosing = true; NextScene = "IntroLevel"; }
	for (int i = 0; i < NumCustomLevels; ++i)
		if (!L1Distance((Player->GetPosition() - vector<2>{ -72, -72 * i })))
		{ BGM->Stop(); isClosing = true; NextScene = "Custom" + std::to_string(i + 1); StartPos = vector<2>{ 0, -72 * i }; }

	if (isClosing)
		if (!CAMERA->CameraClosing()) INSTANCE(SceneManager)->ChangeScene(NextScene);
	CAMERA->PresentCameraCloser();
	return true;
}

void CustomLevel::End() {}