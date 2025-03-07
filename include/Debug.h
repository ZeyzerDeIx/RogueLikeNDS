#pragma once

class Debug
{
public:
	static Debug& get()
	{
		static Debug instance;
		return instance;
	}
private:
	Debug(){consoleDemoInit();};
	~Debug(){};
	
	Debug(const Debug&) = delete;
	Debug& operator=(const Debug&) = delete;
};