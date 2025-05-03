#pragma once

class GameMap;
class Camera;
class Entity;
class AudioManager;

struct GameContext
{
	GameMap* gameMap = nullptr;
	Camera* camera = nullptr;
	Entity* player = nullptr;
	AudioManager* audioManager = nullptr;
};