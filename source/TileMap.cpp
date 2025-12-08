#include "TileMap.h"
#include "TileSet.h"

TileMap::TileMap()
{
	// Load tiles data in memory
	swiFastCopy(TileSetTiles, bgGetGfxPtr(BG::ID), TileSetTilesLen>>2);
	swiFastCopy(TileSetPal, BG_PALETTE, TileSetPalLen>>2);

	for (int i = 0; i < META_TILE::COUNT_W; ++i)
	{
		m_tileMap.push_back(CircularDeque<MetaTile, META_TILE::COUNT_H>());
		for (int j = 0; j < META_TILE::COUNT_H; ++j)
			m_tileMap[i].push_back(MetaTile());
	}

	calculateConnections();
    
    // Reinterpret bgGetMapPtr into m_tileIndicesView to allow simple access using [][]
    auto ptr2D = reinterpret_cast<u16(*)[SUB_TILE::COUNT_H]>(bgGetMapPtr(BG::ID));
    m_tileIndicesView = std::span<u16[SUB_TILE::COUNT_H]>(ptr2D, SUB_TILE::COUNT_W);
}

void TileMap::flush()
{
	convertMap();
}

CircularDeque<MetaTile, META_TILE::COUNT_H>& TileMap::operator[](int key)
{
	return m_tileMap[key];
}



void TileMap::convertMap()
{
	int rows = META_TILE::COUNT_W;
	int cols = META_TILE::COUNT_H;
	

	for (int i = 0 ; i < rows ; ++i)
		for (int j = 0 ; j < cols ; ++j)
			m_tileMap[i][j].flush(m_tileIndicesView, {i, j});
}


void TileMap::calculateConnections()
{
	using namespace META_TILE;
	using namespace DIRECTION;

	int rows = COUNT_H;
	int cols = COUNT_W;

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
