#include "stdafx.h"
#include "CInputComponent.h"

CInputComponent::CInputComponent(CameraManager* owner) : Owner(owner)
{
	PressedList.try_emplace(0x01, &CameraManager::Move);
	PressedList.try_emplace(0x02, &CameraManager::Move);
	PressedList.try_emplace(0x04, &CameraManager::Move);
	PressedList.try_emplace(0x05, &CameraManager::Move);
	PressedList.try_emplace(0x06, &CameraManager::Move);
	PressedList.try_emplace(0x0D, &CameraManager::Move);
	PressedList.try_emplace(0x10, &CameraManager::Move);
	PressedList.try_emplace(0x2D, &CameraManager::Move);
	PressedList.try_emplace(0x2E, &CameraManager::Move);
	PressedList.try_emplace(0xA0, &CameraManager::Move);
	PressedList.try_emplace(0xA1, &CameraManager::Move);
	PressedList.try_emplace(0x90, &CameraManager::Move);
	PressedList.try_emplace(0xBA, &CameraManager::Move);
	PressedList.try_emplace(0xBC, &CameraManager::Move);
	PressedList.try_emplace(0xBE, &CameraManager::Move);
	PressedList.try_emplace(0xBF, &CameraManager::Move);
	PressedList.try_emplace(0xC0, &CameraManager::Move);
	PressedList.try_emplace(0xDB, &CameraManager::Move);
	PressedList.try_emplace(0xDD, &CameraManager::Move);
	PressedList.try_emplace(0xDE, &CameraManager::Move);
	PressedList.try_emplace(0xDF, &CameraManager::Move);

	for (unsigned i = 0x20; i <= 0x28; ++i)
		PressedList.try_emplace(i, &CameraManager::Move);

	for (unsigned i = 0x41; i <= 0x5A; ++i)
		PressedList.try_emplace(i, &CameraManager::Move);

	for (unsigned i = 0x60; i <= 0x6F; ++i)
		PressedList.try_emplace(i, &CameraManager::Move);
}
void CInputComponent::Start() {}
void CInputComponent::Update()
{
    for (std::pair<UINT, void(CameraManager::*)(Object::APlayer*)> elem : PressedList)
    {
		if (Input::Get::Key::Down(elem.first))
			(Owner->*(elem.second))(PLAYER);
    }
}
void CInputComponent::End() {}
void CInputComponent::BindAction(UINT key, void(CameraManager::* func)(Object::APlayer* pPlayer))
{
	PressedList.try_emplace(key, func);
}