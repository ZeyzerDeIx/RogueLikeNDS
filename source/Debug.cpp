#include "pch.h"
#include "NDSTime.h"
#include "Entity.h"

using namespace std;

Debug::Debug()
{
	consoleDemoInit();
	cout << fixed << setprecision(1);
}

void Debug::clearConsole()
{
	cout << "\x1b[2J\x1b[H";
}

void Debug::displayFps()
{
	cout << "\x1b[23;23HFPS:" << setfill(' ') << setw(5) << NDSTime::get().getFps() << "\x1b[0;0H";
}

void Debug::displayEntityInfo(Entity& entity)
{
	cout << "\x1b[10;0HPosition: (" << entity.m_position.x << ", " << entity.m_position.y << ")";

	cout << "\x1b[11;0HSize: (" << entity.m_size.x << ", " << entity.m_size.y << ")";

	cout << "\x1b[12;0HDirections: " 
	 << ((entity.m_directions & DIRECTION::TOP) ? "TOP " : "")
	 << ((entity.m_directions & DIRECTION::BOT) ? "BOT " : "")
	 << ((entity.m_directions & DIRECTION::LEFT) ? "LEFT " : "")
	 << ((entity.m_directions & DIRECTION::RIGHT) ? "RIGHT " : "")
	 << ((entity.m_directions == DIRECTION::NONE) ? "NONE" : "");


	cout << "\x1b[13;0HHitbox:("
	 << entity.m_hitbox.getBounds().x << ","
	 << entity.m_hitbox.getBounds().y << ","
	 << entity.m_hitbox.getBounds().w << ","
	 << entity.m_hitbox.getBounds().h << ")";

	cout << "\x1b[14;0HSpeed: " << entity.m_speed << "\x1b[0;0H";

}