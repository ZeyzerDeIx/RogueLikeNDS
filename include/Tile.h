#pragma once

#include "pch.h"

using TileIndicesView = std::span<u16[SUB_TILE::COUNT_H]>;

class Tile
{
public:
	Tile(u8 corner);
	
	void setConnections(u8 connections);

	void flush(TileIndicesView tileIndicesView, META_TILE::Type type, Vector2i pos);
private:
	u16 m_connections    : 7;
	u16 m_metaTileColumn : 7;
	u16 m_flipX : 1;
	u16 m_flipY : 1;
};