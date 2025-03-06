#pragma once

#include "PCH.h"

class Tile
{
public:
	Tile(u8 corner);
	
	void setConnections(u8 connections);

	void flush(std::span<u16[SUB_TILE::COUNT_H]> bgTileMap, Vector2i pos);
private:
	u8 m_connections;
	Vector2b m_flips;
};