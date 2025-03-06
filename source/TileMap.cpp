#include "TileMap.h"
#include "TileSet.h"

u16 TileMap::m_bgTileMap[SUB_TILE::COUNT_W][SUB_TILE::COUNT_H];

TileMap::TileMap(): 
	m_tileMap(META_TILE::COUNT_W, std::vector<MetaTile>(META_TILE::COUNT_H))
{
	// Load tiles data in memory
	swiFastCopy(TileSetTiles, bgGetGfxPtr(BG::ID), TileSetTilesLen>>2);
	swiFastCopy(TileSetPal, BG_PALETTE, TileSetPalLen>>2);

	calculateConnections();
}

void TileMap::flush()
{
	convertMap();
	swiFastCopy(m_bgTileMap, bgGetMapPtr(BG::ID), sizeof(m_bgTileMap)>>2);
}

std::vector<MetaTile>& TileMap::operator[](int key)
{
	return m_tileMap[key];
}



void TileMap::convertMap()
{
	for (uint i = 0 ; i < m_tileMap.size() ; ++i)
		for (uint j = 0 ; j < m_tileMap[i].size() ; ++j)
			m_tileMap[i][j].flush(m_bgTileMap, {(int)i, (int)j});
}


void TileMap::calculateConnections()
{
	using namespace META_TILE;
	using namespace DIRECTION;

	int rows = m_tileMap.size(), cols = m_tileMap[0].size();

	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < cols; ++x)
		{
			MetaTile& tile = m_tileMap[y][x];
			const Type& type = tile.getType();
			int con = 0;

			// This lambda is here to compress the code, it check if the tile type is the same as the one of the wanted other tile
			auto t = [&](int X, int Y){ return m_tileMap[X][Y].getType() == type; };

			// Precompute boundaries to avoid redundant checks
			bool top = y > 0, bot = y < rows-1, left = x > 0, right = x < cols-1;

			// Cardinal directions
			if (top   && t(y-1, x)) con |= TOP;
			if (bot   && t(y+1, x)) con |= BOT;
			if (left  && t(y, x-1)) con |= LEFT;
			if (right && t(y, x+1)) con |= RIGHT;

			// Diagonal directions
			if (top && left  && t(y-1, x-1)) con |= TOP_LEFT;
			if (top && right && t(y-1, x+1)) con |= TOP_RIGHT;
			if (bot && left  && t(y+1, x-1)) con |= BOT_LEFT;
			if (bot && right && t(y+1, x+1)) con |= BOT_RIGHT;

			tile.setConnections(con);
		}
	}
}
