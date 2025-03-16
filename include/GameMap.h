#pragma once

#include "PCH.h"
#include "TileMap.h"
#include "GameContext.h"

class GameMap
{
public:
	GameMap(GameContext& context);
	~GameMap();

	void update();

	META_TILE::Type getTile(const Vector2i& tileCoordinate) const;

	bool isCrossable(const Vector2i& tileCoordinate) const;

private:
	std::vector<std::vector<META_TILE::Type>> m_map;
	TileMap m_tileMap;
	GameContext& m_context;

	void loadDisplayableTilesIntoTileMap();
};