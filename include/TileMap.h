#pragma once

#include "pch.h"
#include "MetaTile.h"

class GameMap;

class TileMap
{
public:
	// size in tiles
	TileMap();

	//flush the map into background VRAM
	void flush();

	std::vector<MetaTile>& operator[](int key);
private:
	std::vector<std::vector<MetaTile>> m_tileMap;
	std::span<u16[SUB_TILE::COUNT_H]> m_tileIndicesView;

	//Convert m_tileMap into a background adapted tilemap
	void convertMap();

	void calculateConnections();

	friend class GameMap;
};