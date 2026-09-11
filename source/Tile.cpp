#include "Tile.h"

Tile::Tile(u8 corner):
	m_Flips{(bool)(corner&1), (bool)(corner/2)}
{
	m_Connections = 0;
}

void Tile::SetConnections(u8 connections)
{
	m_Connections = connections;
}

void Tile::Flush(std::span<u16[SUB_TILE::COUNT_H]> bgTileMap, META_TILE::Type type, Vector2i pos)
{
	// Layout constants
	constexpr int SUBTILES_PER_AXIS = (TILE::SIZE / SUB_TILE::SIZE);
	constexpr int TILESET_STRIDE    = TILESET::COUNT_W * SUBTILES_PER_AXIS;
	constexpr int TYPE_OFFSET_Y     = 5; // Vertical offset per MetaTile type

	// 1. Calculate source index in VRAM
	const int metaTileIndex     = m_Connections; 
	const int metaTileColumn    = metaTileIndex % TILESET::COUNT_W;
	
	// Calculate linear start index for the row + type offset
	const int metaTileRowOffset = (metaTileIndex - metaTileColumn) + (static_cast<int>(type) * TYPE_OFFSET_Y); 
	
	// Convert to absolute sub-tile index (VRAM address)
	const int sourceSubTileIndex = (metaTileColumn * SUBTILES_PER_AXIS) + (metaTileRowOffset * SUBTILES_PER_AXIS * SUBTILES_PER_AXIS);
	
	// 2. Define sub-tile indices (Row-major)
	//T = Top, L = Left, B = Bottom, R = Right
	u16 subTileTL = sourceSubTileIndex;
	u16 subTileTR = sourceSubTileIndex + 1;
	u16 subTileBL = sourceSubTileIndex + TILESET_STRIDE;
	u16 subTileBR = sourceSubTileIndex + TILESET_STRIDE + 1;

	// 3. Handle Flips (Swap indices geometrically)
	if (m_Flips.x)
	{
		std::swap(subTileTL, subTileTR);
		std::swap(subTileBL, subTileBR);
	}

	if (m_Flips.y)
	{
		std::swap(subTileTL, subTileBL);
		std::swap(subTileTR, subTileBR);
	}

	// 4. Compute Hardware Attributes (Palette + Flips)
	const u16 hwAttributes = (m_Flips.x << TILE::FLIP_H) | (m_Flips.y << TILE::FLIP_V);

	// 5. Write to Map
	const int mapX = pos.x * SUBTILES_PER_AXIS;
	const int mapY = pos.y * SUBTILES_PER_AXIS;

	// Row-Major to match hardware
	bgTileMap[mapY][mapX]         = subTileTL | hwAttributes;
	bgTileMap[mapY][mapX + 1]     = subTileTR | hwAttributes;
	bgTileMap[mapY + 1][mapX]     = subTileBL | hwAttributes;
	bgTileMap[mapY + 1][mapX + 1] = subTileBR | hwAttributes;
}