#include "TileMap.h"
#include "TileSet.h"

u16 TileMap::m_BgTileMap[SUB_TILE::COUNT_W][SUB_TILE::COUNT_H];

TileMap::TileMap()
{
	swiFastCopy(TileSetTiles, bgGetGfxPtr(BG::ID), TileSetTilesLen>>2);
	swiFastCopy(TileSetPal, BG_PALETTE, TileSetPalLen>>2);
}

void TileMap::Flush()
{
	// Copie SYNCHRONE (dmaCopy et non Asynch) : garantit l'absence de clipping si appelé en VBlank !
	dmaCopy(m_BgTileMap, bgGetMapPtr(BG::ID), sizeof(m_BgTileMap));
}