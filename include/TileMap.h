#pragma once

#include "pch.h"
#include "MetaTile.h"
#include "CircularDeque.h"

class GameMap;

class TileMap
{
public:
	// size in tiles
	TileMap();

	//flush the map into background VRAM
	void flush();

	CircularDeque<MetaTile, META_TILE::COUNT_H>& operator[](int key);
private:
	CircularDeque<CircularDeque<MetaTile, META_TILE::COUNT_H>, META_TILE::COUNT_W> m_tileMap;
	std::span<u16[SUB_TILE::COUNT_H]> m_tileIndicesView;

	//Convert m_tileMap into a background adapted tilemap
	void convertMap();

	void calculateConnections();
	void left(GameMap const& gameMap, Vector2i const& offset);
	void right(GameMap const& gameMap, Vector2i const& offset);
	void top(GameMap const& gameMap, Vector2i const& offset);
	void bottom(GameMap const& gameMap, Vector2i const& offset);

	friend class GameMap;
};