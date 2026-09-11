#pragma once
#include "pch.h"
#include "MetaTile.h"

class TileMap
{
public:
	TileMap();
	void Flush();

	// Seul le buffer matériel 64x64 subsiste !
	static u16 m_BgTileMap[SUB_TILE::COUNT_W][SUB_TILE::COUNT_H];
};