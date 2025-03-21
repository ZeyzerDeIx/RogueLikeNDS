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
	void displayFps();
	void displayEntityInfo(Entity& entity);
private:
	Debug();
	~Debug(){};
	
	Debug(const Debug&) = delete;
	Debug& operator=(const Debug&) = delete;
};