#include "NDSTime.h"
#include <cmath>

NDSTime::NDSTime(int updatesPerSeconds)
	: m_fps(0), m_updatesPerSeconds(updatesPerSeconds), m_deltaTime(0.016667f)
{
	timerStart(0, ClockDivider_1024, TIMER_FREQ_1024(m_updatesPerSeconds), [](){get().newTickCallback();});
}

void NDSTime::newFrame()
{
	m_fps = std::ceil(1.f / m_deltaTime);
	
	// Display DeltaTime
	//std::cout << "\x1b[22;12HDeltaTime:" << std::setfill(' ') << std::setw(10) << std::fixed << std::setprecision(6) << m_deltaTime << "\x1b[0;0H";

	m_deltaTime = 0.f;
}

void NDSTime::newTickCallback()
{
	m_deltaTime += 1.f / static_cast<float>(m_updatesPerSeconds);
}

int NDSTime::getFps()
{
	return m_fps;
}

float NDSTime::getDeltaTime()
{
	return m_deltaTime;
}