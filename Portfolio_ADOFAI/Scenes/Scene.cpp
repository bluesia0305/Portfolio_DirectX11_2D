#include "stdafx.h"
#include "Scene.h"

Scene::Scene(unsigned short bpm, float movement_unit)
{
	BGM = new Sound::Sound{ nullptr, 1.0f, false };
	ContentBGM	   = "";
	ContentBG	   = "";
	BPM			   = bpm;
	BGMOffset	   = 0.0f;
	CountTicks	   = 4;
	
	OperationMode  = OPERATION_MODE::INTRO_MODE;
	NextScene	   = "";
	Player		   = new Object::APlayer(bpm, movement_unit);
	StartPos	   = VEC_POSITION_ORIGIN;

	BGMTimer	   = 0.0f;
	CountdownTimer = 0.0f;
	CountdownIndex = 0;

	isStarted	   = false;
	isPlayable	   = false;
	isEnded		   = false;
	isClosing	   = false;
	isFadingIn	   = false;
}

Scene::~Scene()
{
	delete Player;
	delete BGM;
}

OPERATION_MODE Scene::GetOperationMode() const	{ return OperationMode; }
std::string Scene::GetContentBG() const			{ return ContentBG; }
vector<2> Scene::GetStartPos() const			{ return StartPos; }
unsigned short Scene::GetBPM() const			{ return BPM; }
bool Scene::IsStarted() const					{ return isStarted; }
bool Scene::IsPlayable() const					{ return isPlayable; }

bool Scene::InputProcess()
{
	if (Input::Get::Key::Down(0x01) ||
		Input::Get::Key::Down(0x02) ||
		Input::Get::Key::Down(0x04) ||
		Input::Get::Key::Down(0x05) ||
		Input::Get::Key::Down(0x06) ||
		Input::Get::Key::Down(0x0D) ||
		Input::Get::Key::Down(0x10) ||
		Input::Get::Key::Down(0x2D) ||
		Input::Get::Key::Down(0x2E) ||
		Input::Get::Key::Down(0xA0) ||
		Input::Get::Key::Down(0xA1) ||
		Input::Get::Key::Down(0x90) ||
		Input::Get::Key::Down(0xBA) ||
		Input::Get::Key::Down(0xBC) ||
		Input::Get::Key::Down(0xBE) ||
		Input::Get::Key::Down(0xBF) ||
		Input::Get::Key::Down(0xDB) ||
		Input::Get::Key::Down(0xDD) ||
		Input::Get::Key::Down(0x20) ||
		Input::Get::Key::Down(0x21) ||
		Input::Get::Key::Down(0x22) ||
		Input::Get::Key::Down(0x23) ||
		Input::Get::Key::Down(0x24) ||
		Input::Get::Key::Down(0x25) ||
		Input::Get::Key::Down(0x26) ||
		Input::Get::Key::Down(0x27) ||
		Input::Get::Key::Down(0x28) ||
		Input::Get::Key::Down(0x41) ||
		Input::Get::Key::Down(0x42) ||
		Input::Get::Key::Down(0x43) ||
		Input::Get::Key::Down(0x44) ||
		Input::Get::Key::Down(0x45) ||
		Input::Get::Key::Down(0x46) ||
		Input::Get::Key::Down(0x47) ||
		Input::Get::Key::Down(0x48) ||
		Input::Get::Key::Down(0x49) ||
		Input::Get::Key::Down(0x4A) ||
		Input::Get::Key::Down(0x4B) ||
		Input::Get::Key::Down(0x4C) ||
		Input::Get::Key::Down(0x4D) ||
		Input::Get::Key::Down(0x4E) ||
		Input::Get::Key::Down(0x4F) ||
		Input::Get::Key::Down(0x50) ||
		Input::Get::Key::Down(0x51) ||
		Input::Get::Key::Down(0x52) ||
		Input::Get::Key::Down(0x53) ||
		Input::Get::Key::Down(0x54) ||
		Input::Get::Key::Down(0x55) ||
		Input::Get::Key::Down(0x56) ||
		Input::Get::Key::Down(0x57) ||
		Input::Get::Key::Down(0x58) ||
		Input::Get::Key::Down(0x59) ||
		Input::Get::Key::Down(0x5A) ||
		Input::Get::Key::Down(0x60) ||
		Input::Get::Key::Down(0x61) ||
		Input::Get::Key::Down(0x62) ||
		Input::Get::Key::Down(0x63) ||
		Input::Get::Key::Down(0x64) ||
		Input::Get::Key::Down(0x65) ||
		Input::Get::Key::Down(0x66) ||
		Input::Get::Key::Down(0x67) ||
		Input::Get::Key::Down(0x68) ||
		Input::Get::Key::Down(0x69) ||
		Input::Get::Key::Down(0x6A) ||
		Input::Get::Key::Down(0x6B) ||
		Input::Get::Key::Down(0x6C) ||
		Input::Get::Key::Down(0x6D) ||
		Input::Get::Key::Down(0x6E) ||
		Input::Get::Key::Down(0x6F))
		return true;
	else
		return false;
}