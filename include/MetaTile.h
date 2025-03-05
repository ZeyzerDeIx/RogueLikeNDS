#pragma once

#include "PCH.h"
#include "Tile.h"

class MetaTile
{
public:
	MetaTile(META_TILE::Type type = META_TILE::Type::Path);

	void flush(std::span<u8[SUB_TILE::COUNT_H]> bgTileMap, Vector2i pos);

	const META_TILE::Type& getType() const;
	
	void setConnections(u8 connections);
private:
	Tile m_tiles[4];
	u8 m_connections;
	META_TILE::Type m_type;
};