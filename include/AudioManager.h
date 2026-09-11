#pragma once

#include "GameObject.h"

class AudioManager: public GameObject
{
public:
	~AudioManager() override;
	
	void PlayRandomFootstep();

private:
	AudioManager(std::string name);

	friend class GameObject;
};