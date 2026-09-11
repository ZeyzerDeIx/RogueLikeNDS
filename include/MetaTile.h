#pragma once

#include "pch.h"
#include "Tile.h"

class MetaTile
{
public:
	MetaTile(META_TILE::Type type = META_TILE::Type::Path);

	void Flush(std::span<u16[SUB_TILE::COUNT_H]> bgTileMap, Vector2i pos);

	[[nodiscard]] const META_TILE::Type& GetType() const;
	
	void SetType(const META_TILE::Type& type);
	void SetConnections(u8 connections);
private:
	Tile m_Tiles[4];
	u8 m_Connections;
	META_TILE::Type m_Type;
};