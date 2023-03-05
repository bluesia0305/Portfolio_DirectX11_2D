#include "stdafx.h"
#include "Game.h"

void Game::Start()
{
	INSTANCE(CameraManager);
	INSTANCE(SoundManager);
	INSTANCE(SceneManager)->Initialize();
	INSTANCE(ObjectManager);
	INSTANCE(DataManager);
}

bool Game::Update()
{
	if (INSTANCE(SceneManager)->GetCurrentScene()->Update()) return true;
	else return false;
}

void Game::End()
{
	INSTANCE(DataManager)->Release();
	INSTANCE(ObjectManager)->Release();
	INSTANCE(SceneManager)->Release();
	INSTANCE(SoundManager)->Release();
	INSTANCE(CameraManager)->Release();
}

Engine::Game* Engine::Initialize() { return new ::Game(); }