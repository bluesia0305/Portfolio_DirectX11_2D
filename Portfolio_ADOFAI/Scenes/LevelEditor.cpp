#include "stdafx.h"
#include "LevelEditor.h"
#include <iostream>
#include <filesystem>

#define	VEC_LENGTH_CREATE_DIRECTION		vector<2>{ 96.2686f, 62.651f }
#define DOWNCASTED_MAP					dynamic_cast<Map::EditableMap*>(Map)

LevelEditor::LevelEditor(unsigned short bpm, float movement_unit) : Level(bpm, movement_unit)
{
	Map = new Map::EditableMap();

	OperationMode	= OPERATION_MODE::EDIT_MODE;
	CameraMoveBasis = { 0, 0 };
	CameraPosBasis	= { 0, 0 };
	isCameraFollow	= false;

	SteppedTileCount  = 1;
	DecisionMissCount = 0;

	CreateDirectionOpt[0]	= Image::Component{ "text_leveleditor_create_opt1",		VEC_POSITION_NULL, { 165.504f, 25 }, VEC_ANGLE_NULL, 1.0f };
	CreateDirectionOpt[1]	= Image::Component{ "text_leveleditor_create_opt2",		VEC_POSITION_NULL, { 222.416f, 25 }, VEC_ANGLE_NULL, 1.0f };
	CreateDirectionOpt[2]	= Image::Component{ "text_leveleditor_create_opt3",		VEC_POSITION_NULL, { 257.429f, 25 }, VEC_ANGLE_NULL, 1.0f };
	CreateDirectionOpt[3]	= Image::Component{ "text_leveleditor_create_opt4",		VEC_POSITION_NULL, { 303.811f, 25 }, VEC_ANGLE_NULL, 1.0f };
	CreateDirectionOpt[4]	= Image::Component{ "text_leveleditor_create_opt5",		VEC_POSITION_NULL, { 218.540f, 25 }, VEC_ANGLE_NULL, 1.0f };
	CreateDirectionOpt[5]	= Image::Component{ "text_leveleditor_create_opt6",		VEC_POSITION_NULL, { 342.054f, 25 }, VEC_ANGLE_NULL, 1.0f };
	CreateDirectionOpt[0].RevRadius = length(vector<2>{ -228, 90 });
	CreateDirectionOpt[1].RevRadius = length(vector<2>{ -256.3f, 90 });
	CreateDirectionOpt[2].RevRadius = length(vector<2>{ 275, 90 });
	CreateDirectionOpt[3].RevRadius = length(vector<2>{ 299, 90 });
	CreateDirectionOpt[4].RevRadius = length(vector<2>{ 255, -90 });
	CreateDirectionOpt[5].RevRadius = length(vector<2>{ 319, -90 });
	CreateDirectionOpt[0].RevAngle[2] = ConvertToDegree(static_cast<float>(atan2(90, -228)));
	CreateDirectionOpt[1].RevAngle[2] = ConvertToDegree(static_cast<float>(atan2(90, -256.3f)));
	CreateDirectionOpt[2].RevAngle[2] = ConvertToDegree(static_cast<float>(atan2(90, 275)));
	CreateDirectionOpt[3].RevAngle[2] = ConvertToDegree(static_cast<float>(atan2(90, 299)));
	CreateDirectionOpt[4].RevAngle[2] = ConvertToDegree(static_cast<float>(atan2(-90, 255)));
	CreateDirectionOpt[5].RevAngle[2] = ConvertToDegree(static_cast<float>(atan2(-90, 319)));
	for (auto& elem : CreateDirectionOpt)
		elem.Angle = -elem.RevAngle;

	CreateDirection			= Image::Component{ "button_create_tile_direction",		VEC_POSITION_NULL, VEC_LENGTH_NULL, VEC_ANGLE_NULL, 1.0f, 100.0f };
	CreateDirectionKeys[0]  = Image::Component{ "button_create_tile_direction_d",	VEC_POSITION_NULL, VEC_LENGTH_NULL, VEC_ANGLE_NULL, 1.0f, 95.0f };
	CreateDirectionKeys[1]  = Image::Component{ "button_create_tile_direction_e",	VEC_POSITION_NULL, VEC_LENGTH_NULL, VEC_ANGLE_NULL, 1.0f, 95.0f };
	CreateDirectionKeys[2]  = Image::Component{ "button_create_tile_direction_w",	VEC_POSITION_NULL, VEC_LENGTH_NULL, VEC_ANGLE_NULL, 1.0f, 95.0f };
	CreateDirectionKeys[3]  = Image::Component{ "button_create_tile_direction_q",	VEC_POSITION_NULL, VEC_LENGTH_NULL, VEC_ANGLE_NULL, 1.0f, 95.0f };
	CreateDirectionKeys[4]  = Image::Component{ "button_create_tile_direction_a",	VEC_POSITION_NULL, VEC_LENGTH_NULL, VEC_ANGLE_NULL, 1.0f, 95.0f };
	CreateDirectionKeys[5]  = Image::Component{ "button_create_tile_direction_z",	VEC_POSITION_NULL, VEC_LENGTH_NULL, VEC_ANGLE_NULL, 1.0f, 95.0f };
	CreateDirectionKeys[6]  = Image::Component{ "button_create_tile_direction_x",	VEC_POSITION_NULL, VEC_LENGTH_NULL, VEC_ANGLE_NULL, 1.0f, 95.0f };
	CreateDirectionKeys[7]  = Image::Component{ "button_create_tile_direction_c",	VEC_POSITION_NULL, VEC_LENGTH_NULL, VEC_ANGLE_NULL, 1.0f, 95.0f };

	CreateDirectionKeys[8]   = Image::Component{ "button_create_tile_direction_e",	VEC_POSITION_NULL, VEC_LENGTH_NULL, VEC_ANGLE_NULL, 1.0f, 95.0f };
	CreateDirectionKeys[9]   = Image::Component{ "button_create_tile_direction_r",	VEC_POSITION_NULL, VEC_LENGTH_NULL, VEC_ANGLE_NULL, 1.0f, 95.0f };
	CreateDirectionKeys[10]  = Image::Component{ "button_create_tile_direction_q",	VEC_POSITION_NULL, VEC_LENGTH_NULL, VEC_ANGLE_NULL, 1.0f, 95.0f };
	CreateDirectionKeys[11]  = Image::Component{ "button_create_tile_direction_w",	VEC_POSITION_NULL, VEC_LENGTH_NULL, VEC_ANGLE_NULL, 1.0f, 95.0f };
	CreateDirectionKeys[12]  = Image::Component{ "button_create_tile_direction_x",	VEC_POSITION_NULL, VEC_LENGTH_NULL, VEC_ANGLE_NULL, 1.0f, 95.0f };
	CreateDirectionKeys[13]  = Image::Component{ "button_create_tile_direction_z",	VEC_POSITION_NULL, VEC_LENGTH_NULL, VEC_ANGLE_NULL, 1.0f, 95.0f };
	CreateDirectionKeys[14]  = Image::Component{ "button_create_tile_direction_v",	VEC_POSITION_NULL, VEC_LENGTH_NULL, VEC_ANGLE_NULL, 1.0f, 95.0f };
	CreateDirectionKeys[15]  = Image::Component{ "button_create_tile_direction_c",	VEC_POSITION_NULL, VEC_LENGTH_NULL, VEC_ANGLE_NULL, 1.0f, 95.0f };

	CreateDirectionKeys[16]  = Image::Component{ "button_create_tile_direction_r",	VEC_POSITION_NULL, VEC_LENGTH_NULL, VEC_ANGLE_NULL, 1.0f, 95.0f };
	CreateDirectionKeys[17]  = Image::Component{ "button_create_tile_direction_v",	VEC_POSITION_NULL, VEC_LENGTH_NULL, VEC_ANGLE_NULL, 1.0f, 95.0f };
	CreateDirectionKeys[18]  = Image::Component{ "button_create_tile_direction_w",	VEC_POSITION_NULL, VEC_LENGTH_NULL, VEC_ANGLE_NULL, 1.0f, 95.0f };
	CreateDirectionKeys[19]  = Image::Component{ "button_create_tile_direction_e",	VEC_POSITION_NULL, VEC_LENGTH_NULL, VEC_ANGLE_NULL, 1.0f, 95.0f };
	CreateDirectionKeys[20]  = Image::Component{ "button_create_tile_direction_z",	VEC_POSITION_NULL, VEC_LENGTH_NULL, VEC_ANGLE_NULL, 1.0f, 95.0f };
	CreateDirectionKeys[21]  = Image::Component{ "button_create_tile_direction_q",	VEC_POSITION_NULL, VEC_LENGTH_NULL, VEC_ANGLE_NULL, 1.0f, 95.0f };
	CreateDirectionKeys[22]  = Image::Component{ "button_create_tile_direction_c",	VEC_POSITION_NULL, VEC_LENGTH_NULL, VEC_ANGLE_NULL, 1.0f, 95.0f };
	CreateDirectionKeys[23]  = Image::Component{ "button_create_tile_direction_x",	VEC_POSITION_NULL, VEC_LENGTH_NULL, VEC_ANGLE_NULL, 1.0f, 95.0f };
}

LevelEditor::~LevelEditor() { delete Map; }

void LevelEditor::Start()
{
	CAMERA->Start();
	Map->Start();
	Player->Start();

	if (BGMArtist == "")
		LevelTitle = BGMName;
	else
		LevelTitle = BGMArtist + " - " + BGMName;

	if (LevelTitle != "")
		TextLevelTitle.String = LevelTitle.c_str();

	if (ContentBGM != "")
		BGM->Content = ContentBGM.c_str();

	if (BGM->Content)
		BGM->SetVolume(1.0f);

	for (auto& elem : TextCountdown)
	elem.Color = { TextColor.R, TextColor.G, TextColor.B };

	TextLevelTitle.Color = { TextColor.R, TextColor.G, TextColor.B };
	TextReady.Color		 = { TextColor.R, TextColor.G, TextColor.B };
	TextStandBy.Color	 = { TextColor.R, TextColor.G, TextColor.B };
	TextEnded.Color		 = { TextColor.R, TextColor.G, TextColor.B };
	TextOverload.Color	 = { TextColor.R, TextColor.G, TextColor.B };
	TextCompletion.Color = { TextColor.R, TextColor.G, TextColor.B };

	CountdownIndex = 0;
	CountdownTimer = 0.0f;
	BGMTimer	   = 0.0f;
	PlayTimer	   = 0.0f;

	isStarted	   = false;
	isRestartable  = false;

	SteppedTileCount  = 1;
	DecisionMissCount = 0;
}

bool LevelEditor::Update()
{
	CAMERA->Update();
	Map->Update();

	Operation();

	if (isPlayable && !isEnded && !isRestartable)
		PlayTimer += DELTA_TIME;

	if (OperationMode == OPERATION_MODE::TEST_MODE)
	{
		Player->Update();

		Countdown();

		if (isRestartable)
			Restart();

		if (DecisionMissCount > 3)
			TextOverload.Draw();

		if (DecisionMissCount > 3 || !Player->IsExplodeable())
			CompletionPercentage();

		// If reached to the end tile -> End
		if (!isEnded)
			if (DOWNCASTED_MAP->GetCurrentTile() == DOWNCASTED_MAP->GetEndTile())
			{
				isEnded = true;
				SOUND("LevelClear")->Play();
			}

		if (isEnded)
		{
			if (BGM->Content) BGM->GradualVolumeDown(0.2f);
			TextEnded.Draw();
		}
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
			isEnded = false;

			if (BGM->Content) BGM->Stop();
			SOUND("LevelClear")->Stop();
			OperationMode = OPERATION_MODE::EDIT_MODE;
			INSTANCE(SoundManager)->Shutdown();
			NextScene = "IntroLevel";

			Start();
		}

	if (isClosing)
	{
		isCameraFollow = false;
		if (NextScene != "")
		{
			if (!CAMERA->CameraClosing())
				INSTANCE(SceneManager)->ChangeScene(NextScene);
		}
		else
			if (!CAMERA->CameraClosing())
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

void LevelEditor::End()
{
	OperationMode = OPERATION_MODE::EDIT_MODE;
	isClosing = false;
	Player->End();
	INSTANCE(SceneManager)->GenerateCustomLevels();

	ResetLevel();
}

std::string LevelEditor::GetTileTheme() const { return TileTheme; }

void LevelEditor::Operation()
{
	// Background announcement on/off
	if (Input::Get::Key::Down(VK_CONTROL))
	{
		CAMERA->SetIsNotifying(!CAMERA->IsNotifying());
		if (CAMERA->IsNotifying())
			SOUND("NotificationIn")->Play();
		else
			SOUND("NotificationOut")->Play();
	}

#pragma region EDIT_MODE
	if (OperationMode == OPERATION_MODE::EDIT_MODE)
	{
		if (Input::Get::Key::Down(VK_F1))
			EditLevelInfo();
		if (Input::Get::Key::Down(VK_F2))
			EditTileInfo();
		if (Input::Get::Key::Down(VK_F3))
			Export();
		if (Input::Get::Key::Down(VK_F4))
			Import();
		if (Input::Get::Key::Down(VK_F5))
			PrintFloat(DOWNCASTED_MAP->GetSelectedTile()->GetMagnification());
		if (Input::Get::Key::Pressed(VK_MENU) && Input::Get::Key::Down(VK_DELETE))
			ResetMap();

		// Camera movements
		if (Input::Get::Key::Down(0x30))
			ResetCameraZoom();
		if (Input::Get::Key::Down(0x01))
		{
			isCameraFollow = false;
			CAMERA->SetDragBasis(VEC_POSITION_CURSOR);
		}
		if (Input::Get::Key::Pressed(0x01))
			CAMERA->Drag(VEC_POSITION_CURSOR);
		if (isCameraFollow)
			CAMERA->Move(DOWNCASTED_MAP->GetSelectedTile()->GetPosition());
		if (Input::Get::Wheel::V())
			CAMERA->Zoom(Input::Get::Wheel::V());
		
		// Select tile
		if (Input::Get::Key::Down(VK_HOME))   { isCameraFollow = true; DOWNCASTED_MAP->SelectTile(SELECT_TILE::BEGIN_TILE); }
		if (Input::Get::Key::Down(VK_END))	  { isCameraFollow = true; DOWNCASTED_MAP->SelectTile(SELECT_TILE::END_TILE); }
		if (Input::Get::Key::Down(VK_LEFT))   { isCameraFollow = true; DOWNCASTED_MAP->SelectTile(SELECT_TILE::PREV_TILE); }
		if (Input::Get::Key::Down(VK_RIGHT))  { isCameraFollow = true; DOWNCASTED_MAP->SelectTile(SELECT_TILE::NEXT_TILE); }

		// Add / remove tiles
		CreateDirection.Position = DOWNCASTED_MAP->GetSelectedTile()->GetPosition();
		for (auto& elem : CreateDirectionOpt)
			elem.Position = CreateDirection.Position;
		if (!Input::Get::Key::Pressed(VK_SHIFT))
		{
			if (Input::Get::Key::Down('D'))			{ isCameraFollow = true; DOWNCASTED_MAP->AddTile(0.0f); }
			if (Input::Get::Key::Down('E'))			{ isCameraFollow = true; DOWNCASTED_MAP->AddTile(45.0f); }
			if (Input::Get::Key::Down('W'))			{ isCameraFollow = true; DOWNCASTED_MAP->AddTile(90.0f); }
			if (Input::Get::Key::Down('Q'))			{ isCameraFollow = true; DOWNCASTED_MAP->AddTile(135.0f); }
			if (Input::Get::Key::Down('A'))			{ isCameraFollow = true; DOWNCASTED_MAP->AddTile(180.0f); }
			if (Input::Get::Key::Down('Z'))			{ isCameraFollow = true; DOWNCASTED_MAP->AddTile(-135.0f); }
			if (Input::Get::Key::Down('X'))			{ isCameraFollow = true; DOWNCASTED_MAP->AddTile(-90.0f); }
			if (Input::Get::Key::Down('C'))			{ isCameraFollow = true; DOWNCASTED_MAP->AddTile(-45.0f); }
			if (Input::Get::Key::Down(VK_TAB))		{ isCameraFollow = true; DOWNCASTED_MAP->AddTile(DOWNCASTED_MAP->GetSelectedTile()->GetMainAngle() + 180.0f, true); }
			if (Input::Get::Key::Down(VK_DELETE))	{ isCameraFollow = true; DOWNCASTED_MAP->RemoveTile(); }
			for (int i = 0; i < 8; ++i)
			{
				CreateDirection.Length			=  CreateDirectionKeys[i].Length	  = VEC_LENGTH_CREATE_DIRECTION;
				CreateDirection.RevAngle[2]		=  CreateDirectionKeys[i].RevAngle[2] = 45.0f * i;
				CreateDirectionKeys[i].Angle	= -CreateDirectionKeys[i].RevAngle;
				CreateDirectionKeys[i].Position	=  CreateDirection.Position;

				CreateDirection.Draw();
				CreateDirectionKeys[i].Draw();
			}
			CreateDirectionOpt[0].Draw();
			CreateDirectionOpt[2].Draw();
			CreateDirectionOpt[4].Draw();
		}
		else										  //	^^^		if Shift key is *not* pressed		^^^
		{
			if (!Input::Get::Key::Pressed(0xC0))	  //	vvv			if Shift key is pressed			vvv
			{
				if (Input::Get::Key::Down('D'))			{ isCameraFollow = true; DOWNCASTED_MAP->AddTile(0.0f); }
				if (Input::Get::Key::Down('R'))			{ isCameraFollow = true; DOWNCASTED_MAP->AddTile(30.0f); }
				if (Input::Get::Key::Down('E'))			{ isCameraFollow = true; DOWNCASTED_MAP->AddTile(60.0f); }
				if (Input::Get::Key::Down('W'))			{ isCameraFollow = true; DOWNCASTED_MAP->AddTile(120.0f); }
				if (Input::Get::Key::Down('Q'))			{ isCameraFollow = true; DOWNCASTED_MAP->AddTile(150.0f); }
				if (Input::Get::Key::Down('A'))			{ isCameraFollow = true; DOWNCASTED_MAP->AddTile(180.0f); }
				if (Input::Get::Key::Down('Z'))			{ isCameraFollow = true; DOWNCASTED_MAP->AddTile(-150.0f); }
				if (Input::Get::Key::Down('X'))			{ isCameraFollow = true; DOWNCASTED_MAP->AddTile(-120.0f); }
				if (Input::Get::Key::Down('C'))			{ isCameraFollow = true; DOWNCASTED_MAP->AddTile(-60.0f); }
				if (Input::Get::Key::Down('V'))			{ isCameraFollow = true; DOWNCASTED_MAP->AddTile(-30.0f); }
				if (Input::Get::Key::Down(VK_SPACE))	{ isCameraFollow = true; DOWNCASTED_MAP->AddTile(DOWNCASTED_MAP->GetSelectedTile()->GetMainAngle() + 180.0f); }
				if (Input::Get::Key::Down(VK_DELETE))	{ isCameraFollow = true; DOWNCASTED_MAP->AdjustedRemoveTile(); }
				for (int i = 0; i < 8; ++i)
				{
					CreateDirection.Length				=  CreateDirectionKeys[8 + i].Length	  = VEC_LENGTH_CREATE_DIRECTION * 0.823f;
					CreateDirection.RevAngle[2]			=  CreateDirectionKeys[8 + i].RevAngle[2] = 45.0f + 90.0f * (i / 2) + 15.0f * static_cast<float>(pow(-1, i));
					CreateDirectionKeys[8 + i].Angle	= -CreateDirectionKeys[8 + i].RevAngle;
					CreateDirectionKeys[8 + i].Position =  CreateDirection.Position;
					CreateDirection.Draw();
					CreateDirectionKeys[8 + i].Draw();
				}
				for (int i = 0; i < 2; ++i)
				{
					CreateDirection.Length				=  CreateDirectionKeys[4 * i].Length	  = VEC_LENGTH_CREATE_DIRECTION;
					CreateDirection.RevAngle[2]			=  CreateDirectionKeys[4 * i].RevAngle[2] = 180.0f * i;
					CreateDirectionKeys[4 * i].Angle	= -CreateDirectionKeys[4 * i].RevAngle;
					CreateDirectionKeys[4 * i].Position =  CreateDirection.Position;

					CreateDirection.Draw();
					CreateDirectionKeys[4 * i].Draw();
				}
				CreateDirectionOpt[1].Draw();
				CreateDirectionOpt[3].Draw();
				CreateDirectionOpt[5].Draw();
			}										  //	^^^			if Shift key is pressed			^^^
			else
			{										  //	vvv	   if both Shift + `  keys are pressed	vvv
				if (Input::Get::Key::Down('R'))			{ isCameraFollow = true; DOWNCASTED_MAP->AddTile(15.0f); }
				if (Input::Get::Key::Down('E'))			{ isCameraFollow = true; DOWNCASTED_MAP->AddTile(75.0f); }
				if (Input::Get::Key::Down('W'))			{ isCameraFollow = true; DOWNCASTED_MAP->AddTile(105.0f); }
				if (Input::Get::Key::Down('Q'))			{ isCameraFollow = true; DOWNCASTED_MAP->AddTile(165.0f); }
				if (Input::Get::Key::Down('Z'))			{ isCameraFollow = true; DOWNCASTED_MAP->AddTile(-165.0f); }
				if (Input::Get::Key::Down('X'))			{ isCameraFollow = true; DOWNCASTED_MAP->AddTile(-105.0f); }
				if (Input::Get::Key::Down('C'))			{ isCameraFollow = true; DOWNCASTED_MAP->AddTile(-75.0f); }
				if (Input::Get::Key::Down('V'))			{ isCameraFollow = true; DOWNCASTED_MAP->AddTile(-15.0f); }
				if (Input::Get::Key::Down(VK_SPACE))	{ isCameraFollow = true; DOWNCASTED_MAP->AddTile(DOWNCASTED_MAP->GetSelectedTile()->GetMainAngle() + 180.0f); }
				if (Input::Get::Key::Down(VK_DELETE))	{ isCameraFollow = true; DOWNCASTED_MAP->AdjustedRemoveTile(); }
				for (int i = 0; i < 8; ++i)
				{
					CreateDirection.Length				 =  CreateDirectionKeys[16 + i].Length		= VEC_LENGTH_CREATE_DIRECTION * 0.823f;
					CreateDirection.RevAngle[2]			 =  CreateDirectionKeys[16 + i].RevAngle[2] = 90.0f * (i / 2) + 15.0f * static_cast<float>(pow(-1, i));
					CreateDirectionKeys[16 + i].Angle	 = -CreateDirectionKeys[16 + i].RevAngle;
					CreateDirectionKeys[16 + i].Position =  CreateDirection.Position;

					CreateDirection.Draw();
					CreateDirectionKeys[16 + i].Draw();
				}
				CreateDirectionOpt[3].Draw();
				CreateDirectionOpt[5].Draw();
			}
		}

		// Change operation mode
		if (!Input::Get::Key::Pressed(VK_SHIFT) && Input::Get::Key::Down(VK_SPACE))
		{
			isCameraFollow = false;
			OperationMode  = OPERATION_MODE::TEST_MODE;
			DOWNCASTED_MAP->LinkTiles();
			DOWNCASTED_MAP->SetDecisionRadius(DECISION_RADIUS_LENIENT_LEVEL);

			if (ContentBGM != "")
			{
				BGM->Content = ContentBGM.c_str();
				BGM->SetVolume(1.0f);
			}

			Start();
			isFadingIn = true;
			return;
		}
	}																//						^^^^^^^^^^^^		EDIT_MODE		^^^^^^^^^^^^
#pragma endregion

	else if (OperationMode == OPERATION_MODE::TEST_MODE)			//						vvvvvvvvvvvv		TEST_MODE		vvvvvvvvvvvv
	{
		// Stand-by -> Start
		if (!CAMERA->IsFadingIn() && !CAMERA->IsClosing() && !CAMERA->IsOpening() && !isStarted &&Scene::InputProcess())
			isStarted = true;

		// Player move & explosion
		if (isPlayable)
			PlayLevel();

		// AutoPlay on/off
		if (Input::Get::Key::Down(VK_F11))
			AutoPlaySwitch();

		// Change operation mode
		if (Input::Get::Key::Down(VK_ESCAPE))
		{
			if (BGM->Content) BGM->Stop();
			SOUND("LevelClear")->Stop();
			OperationMode  = OPERATION_MODE::EDIT_MODE;
			isPlayable	   = false;
			isCameraFollow = true;
			isEnded		   = false;

			Start();
			return;
		}
	}
}

void LevelEditor::EditLevelInfo()
{
	while (true)
	{
		system("cls");
		printf("### F1 : 파일 import 및 레벨 정보 설정 ###.\n");
		PrintLevelInfo();
		printf("수행할 기능을 선택해주세요.\n");
		printf("[ 1. 파일 Import (BGM 파일, 배경 이미지, 포탈 이미지) ]\n");
		printf("[ 2. 레벨 정보 설정 ]\n");
		std::string input1 = InputSelectNumber(1, 2);
		if (input1 == "1")
		{
			while (true)
			{
				system("cls");
				printf("### F1 : 파일 import 및 레벨 정보 설정 ###.\n");
				PrintLevelInfo();
				printf("Import할 항목을 선택해주세요.\n");
				printf("[ 1. BGM 파일 import ]\n");
				printf("[ 2. 배경 이미지 import ]\n");
				printf("[ 3. 포탈 이미지 import ]\n");
				printf("[ 0. 상위 단계로 이동 ]\n");
				std::string input2 = InputSelectNumber(1, 3, "0");
					 if (input2 == "1") ImportResource(IMPORT_RESOURCE_TYPE::SOUND);
				else if (input2 == "2") ImportResource(IMPORT_RESOURCE_TYPE::BACKGROUND);
				else if (input2 == "3") ImportResource(IMPORT_RESOURCE_TYPE::IMAGE);
				else if (input2 == "0") { input1 = "0"; break; }
				else if (input2 == "")	{ PrintTermination(); return; }
				else continue;
				system("pause");
			}
			if (input1 == "0") continue;
		}
		else if (input1 == "2")
		{
			while (true)
			{
				system("cls");
				printf("### F1 : 파일 import 및 레벨 정보 설정 ###.\n");
				PrintLevelInfo();
				printf("설정할 항목을 선택해주세요.\n");
				printf("[ 1. 기본 타일 테마 ]\n");
				printf("[ 2. 기본 히트 사운드 ]\n");
				printf("[ 3. 곡 이름 ]\n");
				printf("[ 4. 작곡가 이름 ]\n");
				printf("[ 5. 텍스트 컬러 ]\n");
				printf("[ 6. BPM ]\n");
				printf("[ 7. BGM Offset ]\n");
				printf("[ 8. 카운트다운 틱 ]\n");
				printf("[ 0. 상위 단계로 이동 ]\n");
				std::string input2 = InputSelectNumber(1, 7, "0");

				system("cls");
				printf("### F1 : 파일 import 및 레벨 정보 설정 ###.\n");
				PrintLevelInfo();
				if (input2 == "1")
				{
					printf("설정할 기본 타일 테마를 선택하세요.\n");
					printf("* (주의) 해당 설정 시 모든 타일의 테마가 설정값으로 변경됩니다.\n");
					printf("[ 1. 네온 ]\n");
					printf("[ 2. 네온라이트 ]\n");
					printf("[ 3. 화이트 ]\n");
					printf("[ 4. 그레이스케일 ]\n");
					std::string input_num = InputSelectNumber(1, 4);
					if (input_num == "1") TileTheme = "neon";
					else if (input_num == "2") TileTheme = "neonlight";
					else if (input_num == "3") TileTheme = "white";
					else if (input_num == "4") TileTheme = "greyscale";
					else if (input_num == "")  { PrintTermination(); return; }
					else { PrintInappropriateInput(); continue; }
					DOWNCASTED_MAP->SetTileTheme(TileTheme);
				}
				else if (input2 == "2")
				{
					printf("설정할 기본 히트 사운드를 선택해주세요\n");
					printf("* (주의) 해당 설정 시 모든 타일의 히트 사운드가 설정값으로 변경됩니다.\n");
					printf("  [ 1. 킥 ]\n");
					printf("  [ 2. 킥 하우스 ]\n");
					printf("  [ 3. 스네어 ]\n");
					printf("  [ 4. 스네어 하우스 ]\n");
					printf("  [ 5. 사이드 스틱 ]\n");
					printf("  [ 6. 햇 ]\n");
					printf("  [ 7. 해머 ]\n");
					printf("  [ 8. 셰이커 ]\n");
					printf("  [ 9. 박수 ]\n");
					printf("  [ 10. 박수 리버브 ]\n");
					std::string input_num = InputSelectNumber(1, 10);
						 if (input_num == "1")	HitSound = "KickBasic";
					else if (input_num == "2")	HitSound = "KickHouse";
					else if (input_num == "3")	HitSound = "SnareBasic";
					else if (input_num == "4")	HitSound = "SnareHouse";
					else if (input_num == "5")	HitSound = "SnareStick";
					else if (input_num == "6")	HitSound = "HatClosed";
					else if (input_num == "7")	HitSound = "Hammer";
					else if (input_num == "8")	HitSound = "Shaker";
					else if (input_num == "9")	HitSound = "Clap";
					else if (input_num == "10")	HitSound = "ClapReverb";
					else if (input_num == "")	{ PrintTermination(); return; }
					else { PrintInappropriateInput(); continue; }
					DOWNCASTED_MAP->SetHitSound(HitSound);
				}
				else if (input2 == "3")
				{
					printf("설정할 곡 이름을 입력하세요 (띄어쓰기 가능).\n");
					std::string name = InputSpacedString();
					if (name == "") { PrintTermination(); return; }
					else
						BGMName = name;
					if (BGMArtist == "")
						LevelTitle = BGMName;
					else
						LevelTitle = BGMArtist + " - " + BGMName;

					if (LevelTitle != "")
						TextLevelTitle.String = LevelTitle.c_str();
				}
				else if (input2 == "4")
				{
					printf("설정할 작곡가 이름을 입력하세요 (띄어쓰기 가능).\n");
					std::string name = InputSpacedString();
					if (name == "") { PrintTermination(); return; }
					else
						BGMArtist = name;
					if (BGMArtist == "")
						LevelTitle = BGMName;
					else
						LevelTitle = BGMArtist + " - " + BGMName;

					if (LevelTitle != "")
						TextLevelTitle.String = LevelTitle.c_str();
				}		
				else if (input2 == "5")
				{
					int input_color1;
					int input_color2;
					int input_color3;
					printf("설정할 텍스트 컬러의 RGB 값(각각 0 ~ 255)을 띄어쓰기로 구분하여 입력해주세요.\n");
					printf("(입력 예시) 0 0 0: 검정색 / 255 255 255: 흰색\n");
					printf("입력 (회) >> ");
					std::cin >> input_color1;
					std::cin >> input_color2;
					std::cin >> input_color3;
					std::cin.ignore();

					if (input_color1 < 0 || input_color1 > 255 ||
						input_color2 < 0 || input_color2 > 255 ||
						input_color3 < 0 || input_color3 > 255)
					{
						PrintInappropriateInput();
						continue;
					}

					TextColor =
					{
						static_cast<unsigned char>(input_color1),
						static_cast<unsigned char>(input_color2),
						static_cast<unsigned char>(input_color3)
					};

					for (auto& elem : TextCountdown)
						elem.Color = { TextColor.R, TextColor.G, TextColor.B };

					TextLevelTitle.Color = { TextColor.R, TextColor.G, TextColor.B };
					TextReady.Color		 = { TextColor.R, TextColor.G, TextColor.B };
					TextStandBy.Color	 = { TextColor.R, TextColor.G, TextColor.B };
					TextEnded.Color		 = { TextColor.R, TextColor.G, TextColor.B };
					TextOverload.Color	 = { TextColor.R, TextColor.G, TextColor.B };
					TextCompletion.Color = { TextColor.R, TextColor.G, TextColor.B };
				}
				else if (input2 == "6")
				{
					printf("설정할 BPM 값을 정수로 입력하세요.\n");
					printf("입력 >> ");
					std::cin >> BPM;
					std::cin.ignore();
				}
				else if (input2 == "7")
				{
					printf("설정할 BGM Offset 값(초 단위)을 소수로 입력하세요.\n");
					printf("입력 >> ");
					std::cin >> BGMOffset;
					std::cin.ignore();
				}
				else if (input2 == "8")
				{
					printf("설정할 카운트다운 틱 수 (0 ~ 8 사이의 정수)를 입력해주세요.\n");
					printf("입력 (회) >> ");
					std::cin >> CountTicks;
					std::cin.ignore();
				}
				else if (input2 == "0") { input1 = "0"; break; }
				else if (input2 == "")  { PrintTermination(); return; }
				else continue;
			}
			if (input1 == "0") continue;
		}
		else if (input1 == "") { PrintTermination(); return; }
		else continue;
	}
}

void LevelEditor::EditTileInfo()
{
	Object::ATile*				 pSelectedTile	 = DOWNCASTED_MAP->GetSelectedTile();
	Object::AUnidirectionalTile* pDowncastedTile = dynamic_cast<Object::AUnidirectionalTile*>(pSelectedTile);

	while (true)
	{
		system("cls");
		printf("### F2 : 개별 타일 정보 설정 ###.\n");
		PrintTileInfo(pDowncastedTile);
		printf("설정할 타일 정보를 선택해주세요.\n");
		printf("[ 1. 타일 특수 기능 ]\n");
		printf("[ 2. 개별 타일 테마 ]\n");
		printf("[ 3. 개별 히트 사운드 ]\n");
		std::string input1 = InputSelectNumber(1, 3);
		if (input1 == "1")
		{
			while (true)
			{
				if (pSelectedTile == DOWNCASTED_MAP->GetStartTile() ||
					pSelectedTile == DOWNCASTED_MAP->GetEndTile())
				{ system("cls"); printf("시작 타일과 끝 타일은 특수 기능을 설정할 수 없습니다.\n"); return; }

				system("cls");
				printf("### F2 : 개별 타일 정보 설정 ###.\n");
				PrintTileInfo(pDowncastedTile);
				printf("수행할 항목을 선택하세요.\n");
				printf("* 방향 전환 타일: 타일로 이동했을 때, 현재 공전 방향을 반대로 전환시키는 타일.\n");
				printf("* 배속 변경 타일: 타일로 이동했을 때, 현재 진행 속도를 배속으로 변경시키는 타일.\n");
				printf("* 특수 기능을 중복해서 설정할 경우, 아이콘이 정상적으로 보이지 않으므로 타일별로 하나의 기능만 설정하는 것을 권장합니다.\n");
				printf("[ 1. 방향 전환 기능 추가/제거 ]\n");
				printf("[ 2. 배속 변경 기능 추가/제거 ]\n");
				printf("[ 0. 상위 단계로 이동 ]\n");
				std::string input2 = InputSelectNumber(1, 2, "0");
				if (input2 == "1")
				{
					if (pDowncastedTile->IsReversalTile())
						pDowncastedTile->SetIsReversalTile(false);
					else
						pDowncastedTile->SetIsReversalTile(true);
				}
				else if (input2 == "2")
				{
					float input_float;
					system("cls");
					printf("### F2 : 개별 타일 정보 설정 ###.\n");
					PrintTileInfo(pDowncastedTile);
					printf("설정할 배속을 소수(x >= 1.0 또는 x < -1.0)로 입력해주세요.\n");
					printf("* 배속 변경 타일: 타일을 밟는 순간부터 그 이후의 진행 속도를 배속으로 변경시키는 타일.\n");
					printf("(입력 예시) 양수를 입력하면 해당 타일부터 빠른 속도로 진행, 음수를 입력하면 해당 타일부터 느린 속도로 진행합니다.\n");
					printf("   1.0 = 해당 타일 이후부터 기본 BPM 속도로 변경.\n");
					printf("   1.5 = 해당 타일 이후부터 BPM의 1.5배속 빠른 속도로 진행.\n");
					printf("  -2.0 = 해당 타일 이후부터 BPM의 2배속 느린 속도로 진행.\n");
					printf("입력 >> ");
					std::cin >> input_float;
					std::cin.ignore();
					if (input_float >= 1.0)
						DOWNCASTED_MAP->SetMagnification(input_float);
					else if (input_float < -1.0)
						DOWNCASTED_MAP->SetMagnification(1 / abs(input_float));
				}
				else if (input2 == "0") { input1 = "0"; break; }
				else if (input2 == "")	{ PrintTermination(); return; }
				else continue;
			}
		}
		else if (input1 == "2")
		{
			std::string indivTileTheme;
			system("cls");
			printf("### F2 : 개별 타일 정보 설정 ###.\n");
			PrintTileInfo(pDowncastedTile);
			printf("설정할 개별 타일 테마를 선택해주세요.\n");
			printf("[ 1. 네온 ]\n");
			printf("[ 2. 네온라이트 ]\n");
			printf("[ 3. 화이트 ]\n");
			printf("[ 4. 그레이스케일 ]\n");
			std::string input_num = InputSelectNumber(1, 4);
				 if (input_num == "1") indivTileTheme = "neon";
			else if (input_num == "2") indivTileTheme = "neonlight";
			else if (input_num == "3") indivTileTheme = "white";
			else if (input_num == "4") indivTileTheme = "greyscale";
			else if (input_num == "")  { PrintTermination(); return; }
			else { PrintInappropriateInput(); continue; }
			pDowncastedTile->SetTileTheme(indivTileTheme);
		}
		else if (input1 == "3")
		{
			std::string indivHitSound;
			system("cls");
			printf("### F2 : 개별 타일 정보 설정 ###.\n");
			PrintTileInfo(pDowncastedTile);
			printf("설정할 개별 히트 사운드를 선택해주세요\n");
			printf("[ 1. 킥 ]\n");
			printf("[ 2. 킥 하우스 ]\n");
			printf("[ 3. 스네어 ]\n");
			printf("[ 4. 스네어 하우스 ]\n");
			printf("[ 5. 사이드 스틱 ]\n");
			printf("[ 6. 햇 ]\n");
			printf("[ 7. 해머 ]\n");
			printf("[ 8. 셰이커 ]\n");
			printf("[ 9. 박수 ]\n");
			printf("[ 10. 박수 리버브 ]\n");
			std::string input_num = InputSelectNumber(1, 10);
			if (input_num == "1")		indivHitSound = "KickBasic";
			else if (input_num == "2")	indivHitSound = "KickHouse";
			else if (input_num == "3")	indivHitSound = "SnareBasic";
			else if (input_num == "4")	indivHitSound = "SnareHouse";
			else if (input_num == "5")	indivHitSound = "SnareStick";
			else if (input_num == "6")	indivHitSound = "HatClosed";
			else if (input_num == "7")	indivHitSound = "Hammer";
			else if (input_num == "8")	indivHitSound = "Shaker";
			else if (input_num == "9")	indivHitSound = "Clap";
			else if (input_num == "10")	indivHitSound = "ClapReverb";
			else if (input_num == "")	{ PrintTermination(); return; }
			else { PrintInappropriateInput(); continue; }
			pDowncastedTile->SetHitSound(indivHitSound);
		}
		else if (input1 == "") { PrintTermination(); return; }
		else continue;
	}
}

void LevelEditor::Export() const
{
	system("cls");
	printf("현재까지 설정한 레벨을 저장합니다.\n");
	PrintLevelInfo();
	printf("저장할 파일의 이름을 설정해주세요 (확장자 제외).\n");
	printf("* 파일은 Data/Levels 폴더에 저장되며, 커스텀 레벨에서 플레이할 수 있습니다.\n");
	std::string saveFile = InputSpacedString();
	
	if (saveFile != "")
	{
		INSTANCE(DataManager)->LevelInfo->ContentBGM	= (ContentBGM	 == "" ? "__NULL__" : ContentBGM);
		INSTANCE(DataManager)->LevelInfo->ContentBG		= (ContentBG	 == "" ? "__NULL__" : ContentBG);
		INSTANCE(DataManager)->LevelInfo->ContentPortal = (ContentPortal == "" ? "__NULL__" : ContentPortal);
		INSTANCE(DataManager)->LevelInfo->BGMArtist		= (BGMArtist	 == "" ? "__NULL__" : BGMArtist);
		INSTANCE(DataManager)->LevelInfo->BGMName		= (BGMName		 == "" ? "__NULL__" : BGMName);
		INSTANCE(DataManager)->LevelInfo->TileTheme		= TileTheme;
		INSTANCE(DataManager)->LevelInfo->HitSound		= HitSound;
		INSTANCE(DataManager)->LevelInfo->TextColor		= TextColor;
		INSTANCE(DataManager)->LevelInfo->BPM			= BPM;
		INSTANCE(DataManager)->LevelInfo->BGMOffset		= BGMOffset;
		INSTANCE(DataManager)->LevelInfo->CountTicks	= CountTicks;
		INSTANCE(DataManager)->LevelInfo->isCleared		= isCleared;

		DOWNCASTED_MAP->Export();

		if (INSTANCE(DataManager)->ExportLevel(saveFile))
			printf("\n레벨이 정상적으로 저장되었습니다 (Data/CustomLevels/%s.Level).\n", saveFile.c_str());
		else
			printf("\n(오류) 레벨이 저장되지 않았습니다.\n");
		system("pause");
		system("cls");
	}
	else { PrintTermination(); return; }
}

void LevelEditor::Import()
{
	system("cls");
	printf("\'Data/CustomLevels\' 폴더에 저장되어있는 레벨 정보 파일의 이름을 입력하세요 (확장자 제외).\n");
	std::string filename = InputSpacedString();

	if (filename != "")
	{
		if (INSTANCE(DataManager)->ImportLevel(filename, "CustomLevels"))
		{
			ContentBGM	  = INSTANCE(DataManager)->LevelInfo->ContentBGM	== "__NULL__" ? "" : INSTANCE(DataManager)->LevelInfo->ContentBGM;
			ContentBG	  = INSTANCE(DataManager)->LevelInfo->ContentBG		== "__NULL__" ? "" : INSTANCE(DataManager)->LevelInfo->ContentBG;
			ContentPortal = INSTANCE(DataManager)->LevelInfo->ContentPortal == "__NULL__" ? "" : INSTANCE(DataManager)->LevelInfo->ContentPortal;
			TileTheme	  = INSTANCE(DataManager)->LevelInfo->TileTheme;
			HitSound	  = INSTANCE(DataManager)->LevelInfo->HitSound;
			BGMArtist	  = INSTANCE(DataManager)->LevelInfo->BGMArtist		== "__NULL__" ? "" : INSTANCE(DataManager)->LevelInfo->BGMArtist;
			BGMName		  = INSTANCE(DataManager)->LevelInfo->BGMName		== "__NULL__" ? "" : INSTANCE(DataManager)->LevelInfo->BGMName;
			TextColor	  = INSTANCE(DataManager)->LevelInfo->TextColor;
			BPM			  = INSTANCE(DataManager)->LevelInfo->BPM;
			BGMOffset	  = INSTANCE(DataManager)->LevelInfo->BGMOffset;
			CountTicks	  = INSTANCE(DataManager)->LevelInfo->CountTicks;
			isCleared	  = INSTANCE(DataManager)->LevelInfo->isCleared;

			DOWNCASTED_MAP->Import();
			Start();

			printf("\n레벨 정보가 정상적으로 import 되었습니다.\n");		
		}
		else
			printf("\n(오류) 레벨 정보가 정상적으로 import 되지 않았습니다.\n");
		system("pause");
		system("cls");
	}
	else { PrintTermination(); return; }
}

void LevelEditor::ImportResource(IMPORT_RESOURCE_TYPE resource_type)
{
	std::string origFile;
	std::string origFile_basename;
	std::string origFile_extension;
	std::string copyFile;

	while (true)
	{
		system("cls");

		if (resource_type == IMPORT_RESOURCE_TYPE::BACKGROUND)
			printf("Import할 배경 이미지(PNG 파일)가 존재하는 경로를 절대경로로 입력해주세요 (권장 이미지 크기: 1280 x 720).\n");
		else if (resource_type == IMPORT_RESOURCE_TYPE::IMAGE)
			printf("Import할 포탈 이미지(PNG 파일)가 존재하는 경로를 절대경로로 입력해주세요 (권장 이미지 크기: 424 x 512, 타원형 이미지).\n");
		else
			printf("Import할 BGM 파일(WAV 파일)이 존재하는 경로를 절대경로로 입력해주세요.\n");
		printf("* 하위 폴더는 '/' 로만 구분하여 입력해주세요 ('\\' 사용 불가능).\n");

		if (resource_type != IMPORT_RESOURCE_TYPE::SOUND)
			printf("* (입력 예시) C:/Users/blues/Desktop/abc.png\n\n");
		else
			printf("* (입력 예시) C:/Users/blues/Desktop/abc.wav\n\n");

		origFile = InputSpacedString();
		if (origFile == "") { printf("\nResource import를 취소했습니다.\n"); return; }

		if (resource_type == IMPORT_RESOURCE_TYPE::BACKGROUND)
		{
			if (std::filesystem::exists(std::string("Resources/Backgrounds/" + origFile + ".png")))
			{ ContentBG = origFile; CAMERA->SetBackground(ContentBG); printf("\nResource를 성공적으로 import 했습니다.\n"); return; }
		}
		else if (resource_type == IMPORT_RESOURCE_TYPE::IMAGE)
		{
			if (std::filesystem::exists(std::string("Resources/Images/" + origFile + ".png")))
			{ ContentPortal = origFile; printf("\nResource를 성공적으로 import 했습니다.\n"); return; }
		}
		else if (resource_type == IMPORT_RESOURCE_TYPE::SOUND)
		{
			if (std::filesystem::exists(std::string("Resources/Sounds/" + origFile + ".wav")))
			{ ContentBGM = origFile; printf("\nResource를 성공적으로 import 했습니다.\n"); return; }
		}

		size_t const str_start = origFile.find_last_of('/') + sizeof(char);
		size_t const str_end = origFile.find_first_of('.');
		origFile_basename = origFile.substr(str_start, str_end - str_start);
		origFile_extension = origFile.substr(str_end + sizeof(char));

		if (std::filesystem::exists(origFile))
			if (resource_type != IMPORT_RESOURCE_TYPE::SOUND)
			{
				if (origFile_extension == "png")
					break;
				else
					printf("\n(오류) PNG 파일만 import할 수 있습니다.\n");
			}
			else
			{
				if (origFile_extension == "wav")
					break;
				else
					printf("\n(오류) WAV 파일만 import할 수 있습니다.\n");
			}
		else
			printf("\n(오류) 해당 경로에 파일이 존재하지 않습니다.\n");
		system("pause");
	}

	if (resource_type == IMPORT_RESOURCE_TYPE::BACKGROUND)
		copyFile = "Resources/Backgrounds/Import/" + origFile_basename + ".png";
	else if (resource_type == IMPORT_RESOURCE_TYPE::IMAGE)
		copyFile = "Resources/Images/Import/" + origFile_basename + ".png";
	else
		copyFile = "Resources/Sounds/Import/" + origFile_basename + ".wav";

	std::filesystem::path fspathFile = origFile;
	std::filesystem::path fspathCopy = copyFile;

	if (origFile_extension == "wav")
	{
		if (!std::filesystem::exists(fspathCopy))
			std::filesystem::copy(fspathFile, fspathCopy);
	}
	else
		std::filesystem::copy(fspathFile, fspathCopy, std::filesystem::copy_options::overwrite_existing);

	if (resource_type == IMPORT_RESOURCE_TYPE::BACKGROUND)
	{
		Resource::Import(copyFile, Background::Import);
		ContentBG = origFile_basename;
		CAMERA->SetBackground(ContentBG);
	}
	else if (resource_type == IMPORT_RESOURCE_TYPE::IMAGE)
	{
		Resource::Import(copyFile, Image::Import);
		ContentPortal = origFile_basename;
	}
	else if (resource_type == IMPORT_RESOURCE_TYPE::SOUND)
	{
		Resource::Import("Resources/Sounds", Sound::Import);
		ContentBGM = origFile_basename;
	}
	printf("\nResource를 성공적으로 import 했습니다.\n");
}

void LevelEditor::PrintLevelInfo() const
{
	printf("\n[ 현재 레벨 정보 ]\n");
	printf("---------------------------------------\n");
	printf(" - BGM 파일\t    : %s\n",		ContentBGM	  == "" ? "(없음)" : ContentBGM.c_str());
	printf(" - 배경 이미지\t    : %s\n",		ContentBG	  == "" ? "(없음)" : ContentBG.c_str());
	printf(" - 포탈 이미지\t    : %s\n",		ContentPortal == "" ? "(없음)" : ContentPortal.c_str());
	printf(" - 기본 타일 테마   : %s\n",		TileTheme	  == "neon" ? "네온" :
											TileTheme	  == "neonlight" ? "네온라이트" :
											TileTheme	  == "white" ? "화이트" : "그레이스케일");
	printf(" - 기본 히트 사운드 : %s\n",		HitSound	  == "KickBasic" ? "킥" :
											HitSound	  == "KickHouse" ? "킥 하우스" :
											HitSound	  == "SnareBasic" ? "스네어" :
											HitSound	  == "SnareHouse" ? "스네어 하우스" :
											HitSound	  == "SnareStick" ? "사이드 스틱" :
											HitSound	  == "HatClosed" ? "햇" :
											HitSound	  == "Hammer" ? "해머" :
											HitSound	  == "Shaker" ? "셰이커" :
											HitSound	  == "Clap" ? "박수" : "박수 리버브");
	printf(" - 곡 이름\t    : %s\n",			BGMName		  == "" ? "(없음)" : BGMName.c_str());
	printf(" - 작곡가 이름\t    : %s\n",		BGMArtist	  == "" ? "(없음)" : BGMArtist.c_str());
	printf(" - 텍스트 컬러\t    : %d %d %d\n", TextColor.R, TextColor.G, TextColor.B);
	printf(" - BPM      \t    : %d\n",		BPM);
	printf(" - BGM 오프셋\t    : %.3f 초\n",	BGMOffset);
	printf(" - 카운트다운 틱    : %d 회\n",	CountTicks);
	printf("---------------------------------------\n\n");
}
void LevelEditor::PrintTileInfo(Object::AUnidirectionalTile* pSelectedTile) const
{
	Object::ATile* pPrevTile = DOWNCASTED_MAP->GetPrevTile(pSelectedTile);
	printf("\n[ 현재 타일 정보 ]\n");
	printf("---------------------------------------\n");
	printf(" - 타일 특수 기능   : %s\n", pSelectedTile->IsReversalTile() ? "방향 전환" : pPrevTile ? (pSelectedTile->GetMagnification() != pPrevTile->GetMagnification() ? "배속 변경" : "(없음)") : "(없음)");
	printf(" - 개별 타일 테마   : %s\n", pSelectedTile->GetTileTheme() == "neon" ? "네온" :
										pSelectedTile->GetTileTheme() == "neonlight" ? "네온라이트" :
										pSelectedTile->GetTileTheme() == "white" ? "화이트" : "그레이스케일");
	printf(" - 개별 히트 사운드 : %s\n",	pSelectedTile->GetHitSound()  == "KickBasic" ? "킥" :
										pSelectedTile->GetHitSound()  == "KickHouse" ? "킥 하우스" :
										pSelectedTile->GetHitSound()  == "SnareBasic" ? "스네어" :
										pSelectedTile->GetHitSound()  == "SnareHouse" ? "스네어 하우스" :
										pSelectedTile->GetHitSound()  == "SnareStick" ? "스틱" :
										pSelectedTile->GetHitSound()  == "HatClosed" ? "햇" :
										pSelectedTile->GetHitSound()  == "Hammer" ? "해머" :
										pSelectedTile->GetHitSound()  == "Shaker" ? "셰이커" :
										pSelectedTile->GetHitSound()  == "Clap" ? "박수" : "박수 리버브");
	printf("---------------------------------------\n\n");
}
void LevelEditor::PrintTermination() const
{
	system("cls");
	printf("설정을 종료했습니다.\n");
}
void LevelEditor::PrintInappropriateInput() const
{
	printf("\n잘못된 값을 입력하여 정상적으로 설정되지 않았습니다.\n");
	system("pause");
}

std::string LevelEditor::InputSpacedString() const
{
	char spacedStr[100];

	printf("\n* 입력을 하지 않은 상태에서 Enter 키를 누르면 설정을 종료합니다.\n");
	printf("입력 (공백 포함 가능) >> ");
	std::cin.getline(spacedStr, sizeof(spacedStr), '\n');
	return std::string(spacedStr);
}
std::string LevelEditor::InputSelectNumber(unsigned start, unsigned end, std::string turn_back) const
{
	char spacedStr[100];

	printf("\n* 입력을 하지 않은 상태에서 Enter 키를 누르면 설정을 종료합니다.\n");
	printf("입력 [%d-%d", start, end);
	if (turn_back != "")
		printf(", %s", turn_back.c_str());
	printf("] >> ");
	std::cin.getline(spacedStr, sizeof(spacedStr), '\n');
	return std::string(spacedStr);
}

void LevelEditor::ResetCameraZoom()
{
	vector<2> currentPos = DOWNCASTED_MAP->GetSelectedTile()->GetPosition();
	CAMERA->Start();
	CAMERA->SetPosition(currentPos);
	isCameraFollow = false;
}
void LevelEditor::ResetMap()
{
	TileTheme = "neon";

	delete Map;
	Map = new Map::EditableMap();

	Start();
}
void LevelEditor::ResetLevel()
{
	ResetMap();

	OperationMode  = OPERATION_MODE::EDIT_MODE;

	BGM->Content   = nullptr;
	ContentBGM	   = "";
	ContentBG	   = "";
	ContentPortal  = "";
	TileTheme	   = "neon";
	HitSound	   = "KickBasic";
	BGMName		   = "";
	BGMArtist	   = "";
	TextColor	   = { 255, 255, 255 };
	BPM			   = 100;
	BGMOffset	   = 0.0f;
	CountTicks	   = 4;

	CountdownIndex = 0;
	CountdownTimer = 0.0f;
	BGMTimer	   = 0.0f;
	PlayTimer	   = 0.0f;

	isStarted	   = false;
	isRestartable  = false;

	SteppedTileCount  = 1;
	DecisionMissCount = 0;
}