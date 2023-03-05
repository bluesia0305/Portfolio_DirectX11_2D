#pragma once
#include "Engine/Game.h"

class Game final : public Engine::Game
{
	void Start() override;
	bool Update() override;
	void End() override;
};