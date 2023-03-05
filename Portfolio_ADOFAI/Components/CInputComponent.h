#pragma once

class CameraManager;
namespace Object { class APlayer; }

class CInputComponent
{
public:
	CInputComponent(CameraManager* owner);

public:
	void Start();
	void Update();
	void End();
	void BindAction(UINT key, void(CameraManager::* func)(Object::APlayer* pPlayer));

private:
	CameraManager* Owner;
	Object::APlayer* pPlayer;
	std::map<UINT, void(CameraManager::*)(Object::APlayer*)>	PressedList;
};

