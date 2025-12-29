#include "Tile.h"

Tile::Tile(u8 corner):
	m_flipX{(bool)(corner&1)},
	m_flipY{(bool)(corner/2)}
{
	m_connections = 0;
}

void Tile::setConnections(u8 connections)
{
	m_connections = connections;
	m_metaTileColumn = connections % TILESET::COUNT_W;
}

void Tile::flush(std::span<u16[SUB_TILE::COUNT_H]> bgTileMap, META_TILE::Type type, Vector2i pos)
{
	// Layout constants
	constexpr int SUBTILES_PER_AXIS = (TILE::SIZE / SUB_TILE::SIZE);
	constexpr int TILESET_STRIDE    = TILESET::COUNT_W * SUBTILES_PER_AXIS;
	constexpr int TYPE_OFFSET_Y     = 5; // Vertical offset per MetaTile type
	
	// Calculate linear start index for the row + type offset
	const int metaTileRowOffset = (m_connections - m_metaTileColumn) + (static_cast<int>(type) * TYPE_OFFSET_Y); 
	
	// Convert to absolute sub-tile index (VRAM address)
	//const int sourceSubTileIndex = (m_metaTileColumn * SUBTILES_PER_AXIS) + (metaTileRowOffset * SUBTILES_PER_AXIS * SUBTILES_PER_AXIS);
	const int sourceSubTileIndex = (m_metaTileColumn + metaTileRowOffset * SUBTILES_PER_AXIS) * SUBTILES_PER_AXIS;
	
	// Define sub-tile indices (Row-major)
	//T = Top, L = Left, B = Bottom, R = Right
	u16 subTileTL = sourceSubTileIndex;
	u16 subTileTR = sourceSubTileIndex + 1;
	u16 subTileBL = sourceSubTileIndex + TILESET_STRIDE;
	u16 subTileBR = sourceSubTileIndex + TILESET_STRIDE + 1;

	// Handle Flips (Swap indices geometrically)
	if (m_flipX)
	{
		std::swap(subTileTL, subTileTR);
		std::swap(subTileBL, subTileBR);
	}

	if (m_flipY)
	{
		std::swap(subTileTL, subTileBL);
		std::swap(subTileTR, subTileBR);
	}

	// Compute Hardware Attributes (Palette + Flips)
	const u16 hwAttributes = (m_flipX << TILE::FLIP_H) | (m_flipY << TILE::FLIP_V);

	// Write to Map
	const int mapX = pos.x * SUBTILES_PER_AXIS;
	const int mapY = pos.y * SUBTILES_PER_AXIS;

	// Note: Mapping assumes specific target layout (TR mapped to Y+1)
	bgTileMap[mapX][mapY]         = subTileTL | hwAttributes;
	bgTileMap[mapX][mapY + 1]     = subTileTR | hwAttributes;
	bgTileMap[mapX + 1][mapY]     = subTileBL | hwAttributes;
	bgTileMap[mapX + 1][mapY + 1] = subTileBR | hwAttributes;
}