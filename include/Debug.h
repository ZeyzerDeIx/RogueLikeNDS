#pragma once

#include <cstdio>
#include <cstdarg>

class Entity;
class Debug
{
public:
	static Debug& get()
	{
		static Debug instance;
		return instance;
	}
	void clearConsole();
	void logFps();
	void logEntityInfo(Entity& entity);
	void beginProfile();
	void endProfile();
	void logProfile();
	static void nocashLog(const char* logTxt) { nocashMessage(logTxt); }
	static void nocashLogF(const char* format, ...)
	{
	    char buffer[256];
	    
	    va_list args;
	    va_start(args, format);
	    vsnprintf(buffer, sizeof(buffer), format, args);
	    va_end(args);
	    
	    nocashMessage(buffer);
	}
private:
	Debug();
	~Debug(){};

	u32 m_ticks;
	u32 m_usec;
	
	Debug(const Debug&) = delete;
	Debug& operator=(const Debug&) = delete;
};