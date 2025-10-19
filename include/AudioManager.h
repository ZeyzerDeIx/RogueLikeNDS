#pragma once

#include "GameObject.h"

class AudioManager: public GameObject
{
public:
	~AudioManager();
	
	void playRandomFootstep();

private:
	AudioManager(std::string name);

	friend class GameObject;
};