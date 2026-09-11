#pragma once

#include "pch.h"

class NDSTime
{
public:
	void NewFrame();
	void NewTickCallback();
	int GetFps();
	float GetDeltaTime();
	static NDSTime& Get()
	{
		static NDSTime s_Instance(120);
		return s_Instance;
	}

	NDSTime() = delete;

private:
	int m_Fps;
	int m_UpdatesPerSeconds;
	float m_DeltaTime;

	NDSTime(int updatesPerSeconds);
};