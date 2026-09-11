#include "TileMap.h"

#include "GameMap.h"
#include "TileSet.h"

u16 TileMap::m_BgTileMap[SUB_TILE::COUNT_W][SUB_TILE::COUNT_H];

TileMap::TileMap(GameMap* map): m_GameMap(map)
{
	// Load tiles data in memory
	swiFastCopy(TileSetTiles, bgGetGfxPtr(BG::ID), TileSetTilesLen>>2);
	swiFastCopy(TileSetPal, BG_PALETTE, TileSetPalLen>>2);

	CalculateConnections({0,0});
}

void TileMap::Flush(Vector2i const& offset)
{
	CalculateConnections(offset);
	dmaCopyAsynch(m_BgTileMap, bgGetMapPtr(BG::ID), sizeof(m_BgTileMap));
}

u16* TileMap::operator[](int key)
{
	return m_BgTileMap[key];
}


void TileMap::CalculateConnections(Vector2i const& offset)
{
	using namespace DIRECTION;

	int boundX = META_TILE::COUNT_W - WORD_BORDER_SIZE;
	int boundY = META_TILE::COUNT_H - WORD_BORDER_SIZE;

	for (int localX = WORD_BORDER_SIZE; localX < boundX; ++localX)
	{
		for (int localY = WORD_BORDER_SIZE; localY < boundY; ++localY)
		{
			int worldX = offset.x + localX;
			int worldY = offset.y + localY;

			MetaTile metaTile(m_GameMap->GetTile({worldX, worldY}));

			int con = 0;

			auto SameType = [&](int X, int Y){ return m_GameMap->GetTile({X, Y}) == metaTile.GetType(); };

			// Cardinales
			if (SameType(worldX, worldY - 1)) con |= TOP;
			if (SameType(worldX, worldY + 1)) con |= BOT;
			if (SameType(worldX - 1, worldY)) con |= LEFT;
			if (SameType(worldX + 1, worldY)) con |= RIGHT;

			// Diagonales
			if (SameType(worldX - 1, worldY - 1)) con |= TOP_LEFT;
			if (SameType(worldX + 1, worldY - 1)) con |= TOP_RIGHT;
			if (SameType(worldX - 1, worldY + 1)) con |= BOT_LEFT;
			if (SameType(worldX + 1, worldY + 1)) con |= BOT_RIGHT;

			metaTile.SetConnections(con);

			metaTile.Flush(m_BgTileMap, {localX, localY});
		}
	}
}