#include "Tile.h"

Tile::Tile(u8 corner):
	m_corner(corner)
{
	m_connections = 0;
	processId();
}

void Tile::setConnections(u8 connections)
{
	m_connections = connections;
	processId();
}

void Tile::flush(std::span<u8[SUB_TILE::COUNT_H]> bgTileMap, Vector2i pos)
{
	// tile/subtile ratio
	constexpr int r = (TILE::SIZE / SUB_TILE::SIZE);
	u8 xr = pos.x*r;
	u8 yr = pos.y*r;
	u8 xAxe = m_id%TILESET::COUNT_W;
	u8 yAxe = m_id-xAxe;
	u8 tile = xAxe * r + yAxe * r*r ;
	bgTileMap[xr  ][yr  ] = tile;
	bgTileMap[xr+1][yr  ] = tile + TILESET::COUNT_W * r;
	bgTileMap[xr  ][yr+1] = tile + 1;
	bgTileMap[xr+1][yr+1] = tile + TILESET::COUNT_W * r + 1;
}

u8 Tile::getId() { return m_id; }


void Tile::processId()
{
	m_id = TILE::get(m_connections, m_corner);
}