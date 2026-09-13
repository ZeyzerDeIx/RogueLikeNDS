#pragma once

#include "pch.h"

class NDSTime
{
public:
	void NewFrame();
	void NewTickCallback();
	[[nodiscard]] int GetFps() const;
	[[nodiscard]] FixedPoint GetDeltaTime() const;
	static NDSTime& Get()
	{
		static NDSTime s_Instance(120);
		return s_Instance;
	}

	NDSTime() = delete;

private:
	int m_Fps;
	int m_UpdatesPerSeconds;
	const int m_DeltaTickDuration;
	int m_DeltaTime;

	NDSTime(int updatesPerSeconds);
};