#pragma once

class GameMap;
class Camera;
class Entity;
class AudioManager;

struct GameContext
{
	static GameContext& Get()
	{
		static GameContext s_Instance;
		return s_Instance;
	}

	GameMap* m_GameMap = nullptr;
	Camera* m_Camera = nullptr;
	Entity* m_Player = nullptr;
	AudioManager* m_AudioManager = nullptr;

	GameContext(const GameContext&) = delete;
	GameContext& operator=(const GameContext&) = delete;

private:
	GameContext() = default;
	~GameContext() = default;
};