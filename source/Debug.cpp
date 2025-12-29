#include "pch.h"
#include "NDSTime.h"
#include "Entity.h"

Debug::Debug()
{
	consoleDemoInit();
}

void Debug::clearConsole()
{
	printf("\x1b[2J\x1b[H");
}

void Debug::logFps()
{
	printf("\x1b[23;23HFPS:%5.2d\x1b[0;0H", NDSTime::get().getFps());
}

void Debug::logEntityInfo(Entity& entity)
{
	// 1. Position
	printf("\x1b[10;0HPosition: (%.2f, %.2f)", entity.m_position.x, entity.m_position.y);

	// 2. Size
	printf("\x1b[11;0HSize: (%d, %d)", entity.m_size.x, entity.m_size.y);

	// 3. Directions (Using string formatters with ternary operators)
	printf("\x1b[12;0HDirections: %s%s%s%s%s",
		(entity.m_directions & DIRECTION::TOP)   ? "TOP " : "",
		(entity.m_directions & DIRECTION::BOT)   ? "BOT " : "",
		(entity.m_directions & DIRECTION::LEFT)  ? "LEFT " : "",
		(entity.m_directions & DIRECTION::RIGHT) ? "RIGHT " : "",
		(entity.m_directions == DIRECTION::NONE) ? "NONE" : ""
	);

	// 4. Hitbox (Assuming integer bounds, use %f if float)
	auto bounds = entity.m_hitbox.getBounds();
	printf("\x1b[13;0HHitbox:(%d, %d, %d, %d)", 
		bounds.x, bounds.y, bounds.w, bounds.h);

	// 5. Speed (Assuming int, use %.2f if float) and reset cursor
	printf("\x1b[14;0HSpeed: %.2f\x1b[0;0H", entity.m_speed);
}

void Debug::beginProfile()
{
	cpuStartTiming(2);
}

void Debug::endProfile()
{
	Debug::get().m_ticks = cpuEndTiming();
	Debug::get().m_usec = timerTicks2usec(Debug::get().m_ticks);
}

void Debug::logProfile()
{
	printf("Profile: %lu ticks (%lu us)\n", m_ticks, m_usec);
}