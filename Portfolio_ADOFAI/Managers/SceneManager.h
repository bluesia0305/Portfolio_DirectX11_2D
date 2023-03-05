#pragma once
#include "Utilities/Singleton.h"
#include "Scenes/Scene.h"
#include "Scenes/IntroLevel.h"
#include "Scenes/Calibration.h"
#include "Scenes/LevelEditor.h"
#include "Scenes/Customizing.h"
#include "Scenes/CustomLevel.h"
#include "Scenes/Level.h"

class SceneManager final : public Singleton<SceneManager>
{
private:
	std::map<std::string, Scene*>	Scenes;
	std::string						CurrentScene;
	int								NumCustomLevels;

public:
	SceneManager();
	virtual ~SceneManager();
	void Initialize();
	void GenerateCustomLevel();
	Scene* GetCurrentScene() const;
	void ChangeScene(std::string scene);
};