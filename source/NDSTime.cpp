#include "NDSTime.h"
#include <cmath>

NDSTime::NDSTime(int updatesPerSeconds)
	: m_Fps(0), m_UpdatesPerSeconds(updatesPerSeconds), m_DeltaTime(0.016667f)
{
	timerStart(0, ClockDivider_1024, TIMER_FREQ_1024(m_UpdatesPerSeconds), [](){Get().NewTickCallback();});
}

void NDSTime::NewFrame()
{
	m_Fps = std::ceil(1.f / m_DeltaTime);

	m_DeltaTime = 0.f;
}

void NDSTime::NewTickCallback()
{
	m_DeltaTime += 1.f / static_cast<float>(m_UpdatesPerSeconds);
}

int NDSTime::GetFps()
{
	return m_Fps;
}

float NDSTime::GetDeltaTime()
{
	return m_DeltaTime;
}