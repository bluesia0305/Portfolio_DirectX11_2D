#pragma once
#include "Utilities/Singleton.h"

class SoundManager final : public Singleton<SoundManager>
{
private:
	std::map<std::string, Sound::Sound*> Sounds;

public:
	SoundManager();
	~SoundManager();
	Sound::Sound* GetSound(std::string content);
	void Shutdown() const;
};