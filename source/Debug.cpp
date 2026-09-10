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
	void init()
	{
		consoleDemoInit();
	}

	void clearConsole()
	{
		printf("\x1b[2J\x1b[H");
	}

	void logFps()
	{
		printf("\x1b[23;23HFPS:%5.2d\x1b[0;0H", NDSTime::get().getFps());
	}

	void logEntityInfo(Entity& entity)
	{
		printf("\x1b[10;0HPosition: (%.2f, %.2f)", entity.getPosition().x, entity.getPosition().y);
		printf("\x1b[11;0HSize: (%d, %d)", entity.getSize().x, entity.getSize().y);
		printf("\x1b[12;0HDirections: %s%s%s%s%s",
			entity.getDirection(DIRECTION::TOP)   ? "TOP "   : "",
			entity.getDirection(DIRECTION::BOT)   ? "BOT "   : "",
			entity.getDirection(DIRECTION::LEFT)  ? "LEFT "  : "",
			entity.getDirection(DIRECTION::RIGHT) ? "RIGHT " : "",
			!entity.getDirection(DIRECTION::ALL)  ? "NONE"   : ""
		);
		auto bounds = entity.getHitbox().getBounds();
		printf("\x1b[13;0HHitbox:(%d, %d, %d, %d)", bounds.x, bounds.y, bounds.w, bounds.h);
		printf("\x1b[14;0HSpeed: %.2f\x1b[0;0H", entity.getSpeed());
	}

	void beginProfile()
	{
		cpuStartTiming(2);
	}

	void endProfile()
	{
		s_ticks = cpuEndTiming();
		s_usec = timerTicks2usec(s_ticks);
	}

	void logProfile()
	{
		printf("Profile: %lu ticks (%lu us)\n", s_ticks, s_usec);
	}
}