#include "TileMap.h"

#include "GameMap.h"
#include "TileSet.h"

u16 TileMap::m_BgTileMap[SUB_TILE::COUNT_W][SUB_TILE::COUNT_H];

TileMap::TileMap(GameMap* map): m_GameMap(map)
{
	// Load tiles data in memory
	swiFastCopy(TileSetTiles, bgGetGfxPtr(BG::ID), TileSetTilesLen>>2);
	swiFastCopy(TileSetPal, BG_PALETTE, TileSetPalLen>>2);

	UpdateAndFlushAllMetaTiles({0,0});
}

void TileMap::Flush(Vector2i const& offset)
{
	UpdateAndFlushAllMetaTiles(offset);
	dmaCopyAsynch(m_BgTileMap, bgGetMapPtr(BG::ID), sizeof(m_BgTileMap));
}

u16* TileMap::operator[](int key)
{
	return m_BgTileMap[key];
}


void TileMap::UpdateAndFlushAllMetaTiles(Vector2i const& offset)
{
	for (int localX = 0; localX < META_TILE::COUNT_W; ++localX)
		for (int localY = 0; localY < META_TILE::COUNT_H; ++localY)
			UpdateAndFlushMetaTile({offset.x + localX, offset.y + localY}, {localX, localY});
}

void TileMap::UpdateAndFlushMetaTile(Vector2i const& worldPos, Vector2i const& localPos)
{
	MetaTile metaTile(m_GameMap->GetTile(worldPos));

	int con = 0;

	auto SameType = [&](int X, int Y){ return m_GameMap->GetTile({X, Y}) == metaTile.GetType(); };

	// Cardinales
	if (SameType(worldPos.x, worldPos.y - 1)) con |= DIRECTION::TOP;
	if (SameType(worldPos.x, worldPos.y + 1)) con |= DIRECTION::BOT;
	if (SameType(worldPos.x - 1, worldPos.y)) con |= DIRECTION::LEFT;
	if (SameType(worldPos.x + 1, worldPos.y)) con |= DIRECTION::RIGHT;

	// Diagonales
	if (SameType(worldPos.x - 1, worldPos.y - 1)) con |= DIRECTION::TOP_LEFT;
	if (SameType(worldPos.x + 1, worldPos.y - 1)) con |= DIRECTION::TOP_RIGHT;
	if (SameType(worldPos.x - 1, worldPos.y + 1)) con |= DIRECTION::BOT_LEFT;
	if (SameType(worldPos.x + 1, worldPos.y + 1)) con |= DIRECTION::BOT_RIGHT;

	metaTile.SetConnections(con);

	metaTile.Flush(m_BgTileMap, localPos);
}
