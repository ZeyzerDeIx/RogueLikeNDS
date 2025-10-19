#pragma once

class GameMap;
class Camera;
class Entity;
class AudioManager;

struct GameContext
{
	static GameContext& get()
	{
		static GameContext instance;
		return instance;
	}

	GameMap* gameMap = nullptr;
	Camera* camera = nullptr;
	Entity* player = nullptr;
	AudioManager* audioManager = nullptr;


private:
	GameContext(){};
	~GameContext(){};
	
	GameContext(const GameContext&) = delete;
	GameContext& operator=(const GameContext&) = delete;
};