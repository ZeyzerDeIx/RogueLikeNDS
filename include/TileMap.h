#pragma once

#include "pch.h"
#include "MetaTile.h"
#include "CircularDeque.h"

class GameMap;

using TileMapData = CircularDeque<CircularDeque<MetaTile, META_TILE::COUNT_H>, META_TILE::COUNT_W>;

class TileMap
{
public:
	// size in tiles
	TileMap();

	//flush the map into background VRAM
	void flush();

	CircularDeque<MetaTile, META_TILE::COUNT_H>& operator[](int key);
private:
	TileMapData m_tileMap;
	TileIndicesView m_tileIndicesView;

	void calculateConnections();
	uint32_t evaluateTileConnections(int x, int y);
	void left(GameMap const& gameMap, Vector2i const& offset);
	void right(GameMap const& gameMap, Vector2i const& offset);
	void top(GameMap const& gameMap, Vector2i const& offset);
	void bottom(GameMap const& gameMap, Vector2i const& offset);

	friend class GameMap;
};