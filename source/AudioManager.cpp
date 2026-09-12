#include "pch.h"
#include "AudioManager.h"

using namespace std;

AudioManager::~AudioManager()
{
    soundDisable();
}

void AudioManager::PlayRandomFootstep()
{
	int step = (std::rand() % 6) + 1; // NOLINT(*-msc50-cpp)

	switch(step)
	{
		case 1: mmEffect(SFX_FOOTSTEP1); break;
		case 2: mmEffect(SFX_FOOTSTEP2); break;
		case 3: mmEffect(SFX_FOOTSTEP3); break;
		case 4: mmEffect(SFX_FOOTSTEP4); break;
		case 5: mmEffect(SFX_FOOTSTEP5); break;
		case 6: mmEffect(SFX_FOOTSTEP6); break;
		default:;
	}
}


AudioManager::AudioManager(string name): GameObject(name)
{
    mmInitDefault("nitro:/soundbank.bin");
    mmLoad(MOD_CORRIDORSUIT);
    mmLoadEffect(SFX_FOOTSTEP1);
    mmLoadEffect(SFX_FOOTSTEP2);
    mmLoadEffect(SFX_FOOTSTEP3);
    mmLoadEffect(SFX_FOOTSTEP4);
    mmLoadEffect(SFX_FOOTSTEP5);
    mmLoadEffect(SFX_FOOTSTEP6);
    soundEnable();
    mmStart(MOD_CORRIDORSUIT, MM_PLAY_LOOP);

    mmSetEffectsVolume(700);
}