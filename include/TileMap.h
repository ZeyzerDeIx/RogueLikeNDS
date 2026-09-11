#pragma once

#include "pch.h"
#include "MetaTile.h"

class GameMap;

class TileMap
{
public:
	// size in tiles
	TileMap(GameMap* map);

	//flush the map into background VRAM
	void Flush(Vector2i const& offset);

	u16* operator[](int key);

private:
	GameMap* m_GameMap;

	// real background accurate tilemap based on sub tiles made by grit
	static u16 m_BgTileMap[SUB_TILE::COUNT_W][SUB_TILE::COUNT_H];

	void CalculateConnections(Vector2i const& offset);

	friend class GameMap;
};