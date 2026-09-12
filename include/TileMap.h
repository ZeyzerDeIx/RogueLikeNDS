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
	void Flush(Vector2i const& offset);

	u16* operator[](int key);

private:
	GameMap* m_GameMap;

	// Real background accurate tilemap based on sub tiles made by grit
	static u16 m_BgTileMap[SUB_TILE::COUNT_W][SUB_TILE::COUNT_H];

	void UpdateAndFlushAllMetaTiles(Vector2i const& offset);

	void UpdateAndFlushMetaTile(Vector2i const& worldPos, Vector2i const& localPos);

	friend class GameMap;
};