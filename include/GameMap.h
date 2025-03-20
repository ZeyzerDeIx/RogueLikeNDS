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
	// Speed to expand but also to access + do not store useless data
	std::unordered_map<Vector2i, META_TILE::Type, NDSMath::HashVector2i> m_map;
	TileMap m_tileMap;
	GameContext& m_context;

	void loadDisplayableTilesIntoTileMap();
};