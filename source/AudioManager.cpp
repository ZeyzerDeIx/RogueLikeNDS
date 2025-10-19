#include "pch.h"
#include "AudioManager.h"

using namespace std;

AudioManager::~AudioManager()
{
    soundDisable();
}

void AudioManager::playRandomFootstep()
{
	std::mt19937 rng(time(0));
	std::uniform_int_distribution<> footstep(1, 6);

	switch(footstep(rng))
	{
		case 1: mmEffect(SFX_FOOTSTEP1); break;
		case 2: mmEffect(SFX_FOOTSTEP2); break;
		case 3: mmEffect(SFX_FOOTSTEP3); break;
		case 4: mmEffect(SFX_FOOTSTEP4); break;
		case 5: mmEffect(SFX_FOOTSTEP5); break;
		case 6: mmEffect(SFX_FOOTSTEP6); break;
	}
}


AudioManager::AudioManager(string name): GameObject(name)
{
    mmInitDefaultMem((mm_addr)soundbank_bin);
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