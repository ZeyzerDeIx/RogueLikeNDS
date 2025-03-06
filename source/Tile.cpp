#include "Tile.h"

Tile::Tile(u8 corner):
	m_flips{(bool)(corner&1), (bool)(corner/2)}
{
	m_connections = 0;
}

void Tile::setConnections(u8 connections)
{
	m_connections = connections;
}

void Tile::flush(std::span<u16[SUB_TILE::COUNT_H]> bgTileMap, Vector2i pos)
{
	// Tile/subtile ratio
	constexpr int r = (TILE::SIZE / SUB_TILE::SIZE);

	// Compute base tile indices
	short xAxe = m_connections % TILESET::COUNT_W;
	short yAxe = m_connections - xAxe;
	short tileBase = xAxe * r + yAxe * r * r;

	short topL = tileBase;
	short topR = tileBase + TILESET::COUNT_W * r;
	short botL = tileBase + 1;
	short botR = tileBase + TILESET::COUNT_W * r + 1;

	const Vector2b& f = m_flips;

	// Compute flip flags once
	u16 flipFlags = (f.x << TILE::FLIP_H) | (f.y << TILE::FLIP_V);

	// Array representing the correct tile order
	short tileMap[2][2] =
	{
		{
			f.x ? (f.y ? botR : botL) : (f.y ? topR : topL),
			f.x ? (f.y ? topR : topL) : (f.y ? botR : botL)
		},
		{
			f.x ? (f.y ? botL : botR) : (f.y ? topL : topR),
			f.x ? (f.y ? topL : topR) : (f.y ? botL : botR)
		}
	};
	
	short xr = pos.x * r;
	short yr = pos.y * r;

	// Assign values to bgTileMap
	bgTileMap[xr][yr]     = tileMap[0][0] | flipFlags;
	bgTileMap[xr][yr+1]   = tileMap[0][1] | flipFlags;
	bgTileMap[xr+1][yr]   = tileMap[1][0] | flipFlags;
	bgTileMap[xr+1][yr+1] = tileMap[1][1] | flipFlags;
}
