#include "stdafx.h"
#include "SoundManager.h"

SoundManager::SoundManager()
{
	//				   Key										  Content							Volume	Loop
	Sounds.try_emplace("KickBasic",				new Sound::Sound{ "hitsound_kick_basic",			0.8f,	false });
	Sounds.try_emplace("KickHouse",				new Sound::Sound{ "hitsound_kick_house",			0.8f,	false });
	Sounds.try_emplace("SnareBasic",			new Sound::Sound{ "hitsound_snare_basic",			0.8f,	false });
	Sounds.try_emplace("SnareHouse",			new Sound::Sound{ "hitsound_snare_house",			0.8f,	false });
	Sounds.try_emplace("SnareStick",			new Sound::Sound{ "hitsound_snare_stick",			0.8f,	false });
	Sounds.try_emplace("HatClosed",				new Sound::Sound{ "hitsound_hat_closed",			0.8f,	false });
	Sounds.try_emplace("Hammer",				new Sound::Sound{ "hitsound_hammer",				0.8f,	false });
	Sounds.try_emplace("Shaker",				new Sound::Sound{ "hitsound_shaker",				0.8f,	false });
	Sounds.try_emplace("Clap",					new Sound::Sound{ "hitsound_clap",					0.8f,	false });
	Sounds.try_emplace("ClapReverb",			new Sound::Sound{ "hitsound_clap_reverb",			0.8f,	false });

	Sounds.try_emplace("LevelClear",			new Sound::Sound{ "fxsound_cymbal_crash",			1.0f,	false });
	Sounds.try_emplace("OttoActivate",			new Sound::Sound{ "fxsound_otto_activate",			0.8f,	false });
	Sounds.try_emplace("OttoDeactivate",		new Sound::Sound{ "fxsound_otto_deactivate",		0.8f,	false });
	Sounds.try_emplace("NotificationIn",		new Sound::Sound{ "fxsound_menu_notification_in",	0.8f,	false });
	Sounds.try_emplace("NotificationOut",		new Sound::Sound{ "fxsound_menu_notification_out",	0.8f,	false });
	Sounds.try_emplace("CameraOpening",			new Sound::Sound{ "fxsound_screen_wipe_in",			0.8f,	false });
	Sounds.try_emplace("CameraClosing",			new Sound::Sound{ "fxsound_screen_wipe_out",		0.8f,	false });
	Sounds.try_emplace("PlanetExplosion1",		new Sound::Sound{ "fxsound_planet_explosion1",		0.8f,	false });
	Sounds.try_emplace("PlanetExplosion2",		new Sound::Sound{ "fxsound_planet_explosion2",		0.8f,	false });
}

SoundManager::~SoundManager()
{
	for (auto elem : Sounds)
		delete elem.second;

	Sounds.clear();
}

Sound::Sound* SoundManager::GetSound(std::string content) { return Sounds.at(content); }

void SoundManager::Shutdown() const
{
	for (auto elem : Sounds)
		elem.second->Stop();
}
