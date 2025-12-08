#pragma once
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
private:
	Debug();
	~Debug(){};

	u32 m_ticks;
	u32 m_usec;
	
	Debug(const Debug&) = delete;
	Debug& operator=(const Debug&) = delete;
};