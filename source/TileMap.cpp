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

	int rows = META_TILE::COUNT_W - WORD_BORDER_SIZE;
	int cols = META_TILE::COUNT_H - WORD_BORDER_SIZE;

	for (int i = WORD_BORDER_SIZE; i < rows; ++i)
	{
		for (int j = WORD_BORDER_SIZE; j < cols; ++j)
		{
			int x = offset.x + j;
			int y = offset.y + i;
			MetaTile metaTile(m_GameMap->GetTile({y,x}));

			int con = 0;

			// This lambda is here to compress the code, it check if the tile type is the same as the one of the wanted other tile
			auto SameType = [&](int X, int Y){ return m_GameMap->GetTile({X,Y}) == metaTile.GetType(); };

			// Cardinal directions
			if (SameType(y-1, x)) con |= TOP;
			if (SameType(y+1, x)) con |= BOT;
			if (SameType(y, x-1)) con |= LEFT;
			if (SameType(y, x+1)) con |= RIGHT;

			// Diagonal directions
			if (SameType(y-1, x-1)) con |= TOP_LEFT;
			if (SameType(y-1, x+1)) con |= TOP_RIGHT;
			if (SameType(y+1, x-1)) con |= BOT_LEFT;
			if (SameType(y+1, x+1)) con |= BOT_RIGHT;

			metaTile.SetConnections(con);
			metaTile.Flush(m_BgTileMap, {i, j});
		}
	}
}
