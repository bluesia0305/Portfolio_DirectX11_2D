#include "stdafx.h"
#include "SceneManager.h"
#include <filesystem>

SceneManager::SceneManager()
{
	Scenes.try_emplace("IntroLevel",  new IntroLevel (130, MOVEMENT_UNIT));
	//Scenes.try_emplace("Calibration", new Calibration(130, MOVEMENT_UNIT));
	Scenes.try_emplace("LevelEditor", new LevelEditor(100, MOVEMENT_UNIT_LEVEL));
	Scenes.try_emplace("Customizing", new Customizing(130, MOVEMENT_UNIT));
	CurrentScene	= "IntroLevel";
	NumMainLevels = 0;
	NumCustomLevels = 0;

	GenerateMainLevels();
	GenerateCustomLevels();
}

SceneManager::~SceneManager()
{
	for (auto scene : Scenes)
		delete scene.second;

	Scenes.clear();
}

void SceneManager::Initialize() { Scenes.at(CurrentScene)->Start(); }

void SceneManager::GenerateMainLevels()
{
	std::string directory = "Data/Levels";
	if (std::filesystem::exists(directory))
	{
		for (std::filesystem::path const& path : std::filesystem::recursive_directory_iterator(directory))
		{
			if (std::filesystem::is_regular_file(path))
			{
				std::string file = path.string();
				std::replace(file.begin(), file.end(), '\\', '/');

				size_t const start = file.find_last_of('/') + sizeof(char);
				size_t const end = file.find_first_of('.');
				file = file.substr(start, end - start);

				INSTANCE(DataManager)->ImportLevel(file.c_str(), "Levels");

				std::string scene_name = "Level" + std::to_string(++NumMainLevels);
				Scenes.try_emplace(scene_name, new Level(file,
														 INSTANCE(DataManager)->LevelInfo->ContentBGM,
														 INSTANCE(DataManager)->LevelInfo->ContentBG,
														 INSTANCE(DataManager)->LevelInfo->ContentPortal,
														 INSTANCE(DataManager)->LevelInfo->TileTheme,
														 INSTANCE(DataManager)->LevelInfo->HitSound,
														 INSTANCE(DataManager)->LevelInfo->BGMArtist,
														 INSTANCE(DataManager)->LevelInfo->BGMName,
														 INSTANCE(DataManager)->LevelInfo->TextColor,
														 INSTANCE(DataManager)->LevelInfo->BPM,
														 INSTANCE(DataManager)->LevelInfo->BGMOffset,
														 INSTANCE(DataManager)->LevelInfo->CountTicks,
														 INSTANCE(DataManager)->LevelInfo->isCleared));

				dynamic_cast<Level*>(Scenes.at(scene_name))->Import();
				INSTANCE(DataManager)->ClearTilesInfo();
			}
		}
	}
}

void SceneManager::GenerateCustomLevels()
{
	for (int i = 0; i < NumCustomLevels; ++i)
	{
		std::string custom_scene = "Custom" + std::to_string(i + 1);
		if (Scenes.find(custom_scene) != Scenes.end())
		{
			delete Scenes.at(custom_scene);
			Scenes.erase(custom_scene);
		}
	}

	if (Scenes.find("CustomLevel") != Scenes.end())
	{
		delete Scenes.at("CustomLevel");
		Scenes.erase("CustomLevel");
	}
	
	NumCustomLevels = 0;
	std::string directory = "Data/CustomLevels";
	if (std::filesystem::exists(directory))
	{
		for (std::filesystem::path const& path : std::filesystem::recursive_directory_iterator(directory))
		{
			if (std::filesystem::is_regular_file(path))
			{
				std::string file = path.string();
				std::replace(file.begin(), file.end(), '\\', '/');

				size_t const start = file.find_last_of('/') + sizeof(char);
				size_t const end = file.find_first_of('.');
				file = file.substr(start, end - start);

				INSTANCE(DataManager)->ImportLevel(file.c_str(), "CustomLevels");

				std::string scene_name = "Custom" + std::to_string(++NumCustomLevels);
				Scenes.try_emplace(scene_name, new Level(file,
														 INSTANCE(DataManager)->LevelInfo->ContentBGM,
														 INSTANCE(DataManager)->LevelInfo->ContentBG,
														 INSTANCE(DataManager)->LevelInfo->ContentPortal,
														 INSTANCE(DataManager)->LevelInfo->TileTheme,
														 INSTANCE(DataManager)->LevelInfo->HitSound,
														 INSTANCE(DataManager)->LevelInfo->BGMArtist,
														 INSTANCE(DataManager)->LevelInfo->BGMName,
														 INSTANCE(DataManager)->LevelInfo->TextColor,
														 INSTANCE(DataManager)->LevelInfo->BPM,
														 INSTANCE(DataManager)->LevelInfo->BGMOffset,
														 INSTANCE(DataManager)->LevelInfo->CountTicks,
														 INSTANCE(DataManager)->LevelInfo->isCleared));
				
				dynamic_cast<Level*>(Scenes.at(scene_name))->Import();
				INSTANCE(DataManager)->ClearTilesInfo();
			}
		}
	}

	Scenes.try_emplace("CustomLevel", new CustomLevel(130, MOVEMENT_UNIT, NumCustomLevels));
}

Scene* SceneManager::GetCurrentScene() const { return Scenes.at(CurrentScene); }
std::string SceneManager::GetCurrentSceneName() const { return CurrentScene; }

void SceneManager::ChangeScene(std::string scene)
{
	Scenes.at(CurrentScene)->End();
	CurrentScene = scene;
	Scenes.at(scene)->Start();
}