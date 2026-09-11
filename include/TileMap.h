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
	void Flush();

	std::vector<MetaTile>& operator[](int key);
private:
	std::vector<std::vector<MetaTile>> m_TileMap;
	// real background accurate tilemap based on sub tiles made by grit
	static u16 m_BgTileMap[SUB_TILE::COUNT_W][SUB_TILE::COUNT_H];

	//Convert m_tileMap into a background adapted tilemap
	void ConvertMap();

	void CalculateConnections();

	friend class GameMap;
};