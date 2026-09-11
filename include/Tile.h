#pragma once

#include "pch.h"

class Tile
{
public:
	Tile(u8 corner);
	
	void SetConnections(u8 connections);

	void Flush(std::span<u16[SUB_TILE::COUNT_H]> bgTileMap, META_TILE::Type type, Vector2i pos);
private:
	u8 m_Connections;
	Vector2b m_Flips;
};