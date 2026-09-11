#include "pch.h"
#include "Debug.h"
#include "NDSTime.h"
#include "Entity.h"

namespace
{
	// Internal profiling state
	u32 s_ticks = 0;
	u32 s_usec = 0;
}

namespace Debug
{
	void Init()
	{
		consoleDemoInit();
	}

	void ClearConsole()
	{
		printf("\x1b[2J\x1b[H");
	}

	void LogFps()
	{
		printf("\x1b[23;23HFPS:%5.2d\x1b[0;0H", NDSTime::Get().GetFps());
	}

	void LogEntityInfo(Entity& entity)
	{
		printf("\x1b[10;0HPosition: (%.2f, %.2f)", entity.GetPosition().x, entity.GetPosition().y);
		printf("\x1b[11;0HSize: (%d, %d)", entity.GetSize().x, entity.GetSize().y);
		printf("\x1b[12;0HDirections: %s%s%s%s%s",
			entity.GetDirection(DIRECTION::TOP)   ? "TOP "   : "",
			entity.GetDirection(DIRECTION::BOT)   ? "BOT "   : "",
			entity.GetDirection(DIRECTION::LEFT)  ? "LEFT "  : "",
			entity.GetDirection(DIRECTION::RIGHT) ? "RIGHT " : "",
			!entity.GetDirection(DIRECTION::ALL)  ? "NONE"   : ""
		);
		auto bounds = entity.GetHitbox().GetBounds();
		printf("\x1b[13;0HHitbox:(%d, %d, %d, %d)", bounds.x, bounds.y, bounds.w, bounds.h);
		printf("\x1b[14;0HSpeed: %.2f\x1b[0;0H", entity.GetSpeed());
	}

	void BeginProfile()
	{
		cpuStartTiming(2);
	}

	void EndProfile()
	{
		s_ticks = cpuEndTiming();
		s_usec = timerTicks2usec(s_ticks);
	}

	void LogProfile()
	{
		printf("Profile: %lu ticks (%lu us)\n", s_ticks, s_usec);
	}
}