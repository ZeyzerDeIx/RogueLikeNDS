#pragma once

#include "PCH.h"
#include "TileMap.h"
#include "Camera.h"

class GameMap
{
public:
	GameMap(TileMap& tileMap, Camera& camera);
	~GameMap();

	void update();

private:
	std::vector<std::vector<META_TILE::Type>> m_map;
	TileMap& m_tileMap;
	Camera& m_camera;

	void loadDisplayableTilesIntoTileMap();
};