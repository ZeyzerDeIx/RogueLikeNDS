#pragma once

class GameMap;
class Camera;
class Entity;

struct GameContext
{
	GameMap* gameMap = nullptr;
	Camera* camera = nullptr;
	Entity* player = nullptr;
};