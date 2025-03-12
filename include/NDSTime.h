#pragma once

#include "PCH.h"
#include <cstdlib>

//#ifdef FPS_ENABLE

class NDSTime
{
public:
	void newFrame();
	void newTickCallback();
	int getFps();
	float getDeltaTime();
	static NDSTime& get()
	{
		static NDSTime instance(120);
		return instance;
	}
private:
	int m_fps;
	int m_updatesPerSeconds;
	float m_deltaTime;

	NDSTime() = delete;
	NDSTime(int updatesPerSeconds);
};
//#endif