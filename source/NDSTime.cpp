#include "NDSTime.h"

NDSTime::NDSTime(int updatesPerSeconds)
	: m_Fps(0),
	  m_UpdatesPerSeconds(updatesPerSeconds),
	  m_DeltaTickDuration((1 << NDSMath::TIME_FIXED_POINT_SHIFT) / updatesPerSeconds),
	  m_DeltaTime(m_DeltaTickDuration)
{
	timerStart(0, ClockDivider_1024, TIMER_FREQ_1024(m_UpdatesPerSeconds), [](){Get().NewTickCallback();});
}

void NDSTime::NewFrame()
{
	if (m_DeltaTime > 0)
	{
		m_Fps = ((1 << 16) + m_DeltaTime - 1) / m_DeltaTime;
	}
	else
	{
		m_Fps = 0;
	}

	m_DeltaTime = 0;
}

void NDSTime::NewTickCallback()
{
	m_DeltaTime += m_DeltaTickDuration;
}

int NDSTime::GetFps() const
{
	return m_Fps;
}

FixedPoint NDSTime::GetDeltaTime() const
{
	// Conversion of internal precision (shift-16) to project standard (shift-8)
	return static_cast<FixedPoint>(m_DeltaTime >> NDSMath::TO_STANDARD_FIXED_POINT_SHIFT);
}