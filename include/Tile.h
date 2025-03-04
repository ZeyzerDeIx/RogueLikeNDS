#pragma once

#include "PCH.h"

class Tile
{
public:
	Tile(u8 corner);
	
	void setConnections(u8 connections);

	void flush(std::span<u8[SUB_TILE::COUNT_H]> bgTileMap, Vector2i pos);

	u8 getId();
private:
	u8 m_corner;
	u8 m_id;
	u8 m_connections;

	void processId();
};