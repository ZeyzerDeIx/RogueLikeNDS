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

void Tile::flush(std::span<u16[SUB_TILE::COUNT_H]> bgTileMap, Vector2i pos, Vector2b flips)
{
	// Tile to subtile ratio
	constexpr short ratio = TILE::SIZE / SUB_TILE::SIZE;
	Vector2i r = pos * ratio;
	Vector2i axes = {m_id % TILESET::COUNT_W, 0};
	axes.y = m_id - axes.x;
	short baseTile = axes.x * ratio + axes.y * ratio * ratio;

	u16 topL = baseTile;
	u16 topR = baseTile + TILESET::COUNT_W * ratio;
	u16 botL = baseTile + 1;
	u16 botR = baseTile + TILESET::COUNT_W * ratio + 1;
	
	u16 flipBits = (flips.x << TILE::FLIP_H) | (flips.y << TILE::FLIP_V);

	// Returns the appropriate subtile index for (col, row) after applying horizontal and vertical flips.
	auto getSubTileIndex = [&](bool col, bool row) -> u16
	{
		bool adjCol = flips.x ? 1 - col : col;
		bool adjRow = flips.y ? 1 - row : row;
		return (!adjRow) ? (adjCol ? topR : topL)
						 : (adjCol ? botR : botL);
	};

	bgTileMap[r.x    ][r.y    ] = getSubTileIndex(0, 0) | flipBits;
	bgTileMap[r.x + 1][r.y    ] = getSubTileIndex(1, 0) | flipBits;
	bgTileMap[r.x    ][r.y + 1] = getSubTileIndex(0, 1) | flipBits;
	bgTileMap[r.x + 1][r.y + 1] = getSubTileIndex(1, 1) | flipBits;
}

u8 Tile::getId() { return m_id; }


void Tile::processId()
{
	m_id = TILE::get(m_connections, m_corner);
}