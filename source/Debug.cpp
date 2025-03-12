#include "PCH.h"
#include "NDSTime.h"

void Debug::displayFps()
{
	std::cout << "\x1b[23;23HFPS:" << std::setfill(' ') << std::setw(5) << NDSTime::get().getFps() << "\x1b[0;0H";
}