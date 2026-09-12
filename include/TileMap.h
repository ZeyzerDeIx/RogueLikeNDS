#pragma once

#include "pch.h"
#include "MetaTile.h"

class GameMap;

class TileMap
{
public:
	// size in tiles
	TileMap(GameMap* map);

	// Flush the map into background VRAM
	void Flush();

	void UpdateAllMetaTiles(Vector2i const& offset);

	void UpdateMetaTileLine(int direction, Vector2i const& offset);

	void UpdateTileIfVisible(const Vector2i& worldPos, bool updateNeighbors = true);

	u16* operator[](int key);

private:
	GameMap* m_GameMap;

	// Real background accurate tilemap based on sub tiles made by grit
	static u16 m_BgTileMap[SUB_TILE::COUNT_W][SUB_TILE::COUNT_H];
	void UpdateMetaTile(Vector2i const& worldPos) const;
};