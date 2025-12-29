#include "TileMap.h"
#include "TileSet.h"
#include "GameMap.h"

TileMap::TileMap()
{
	// Load tiles data in memory
	swiFastCopy(TileSetTiles, bgGetGfxPtr(BG::ID), TileSetTilesLen>>2);
	swiFastCopy(TileSetPal, BG_PALETTE, TileSetPalLen>>2);

	for (int i = 0; i < META_TILE::COUNT_W; ++i)
	{
		m_tileMap.pushFront(CircularDeque<MetaTile, META_TILE::COUNT_H>());
		for (int j = 0; j < META_TILE::COUNT_H; ++j)
			m_tileMap.front().pushFront(MetaTile());
	}
    
    // Reinterpret bgGetMapPtr into m_tileIndicesView to allow simple access using [][]
    auto ptr2D = reinterpret_cast<u16(*)[SUB_TILE::COUNT_H]>(bgGetMapPtr(BG::ID));
    m_tileIndicesView = std::span<u16[SUB_TILE::COUNT_H]>(ptr2D, SUB_TILE::COUNT_W);
}

void TileMap::flush()
{
	int rows = META_TILE::COUNT_W;
	int cols = META_TILE::COUNT_H;
	

	for (int i = 0 ; i < rows ; ++i)
		for (int j = 0 ; j < cols ; ++j)
			m_tileMap.at(i).at(j).flush(m_tileIndicesView, {i, j});
}

CircularDeque<MetaTile, META_TILE::COUNT_H>& TileMap::operator[](int key)
{
	return m_tileMap[key];
}


void TileMap::calculateConnections()
{
	int rows = META_TILE::COUNT_H;
	int cols = META_TILE::COUNT_W;

	for (int x = 0; x < rows; ++x)
		for (int y = 0; y < cols; ++y)
			m_tileMap[x][y].setConnections(evaluateTileConnections(x,y));
}

uint32_t TileMap::evaluateTileConnections(int const x, int const y)
{
	constexpr int height = META_TILE::COUNT_H;
	constexpr int width = META_TILE::COUNT_W;

	META_TILE::Type const& type = m_tileMap[x][y].getType();
	uint32_t mask = 0;

	// This lambda is here to compress the code, it check if the tile type is the same as the one of the wanted other tile
	auto isSameAs = [&](int X, int Y){ return m_tileMap[X][Y].getType() == type; };

	// Precompute boundaries to avoid redundant checks
	bool hasLeft = x > 0, hasRight = x < width-1, hasTop = y > 0, hasBot = y < height-1;

	using namespace DIRECTION;

	// Cardinal directions
	if (hasTop   && isSameAs( x , y-1)) mask |= TOP;
	if (hasBot   && isSameAs( x , y+1)) mask |= BOT;
	if (hasLeft  && isSameAs(x-1,  y )) mask |= LEFT;
	if (hasRight && isSameAs(x+1,  y )) mask |= RIGHT;

	// Diagonal directions
	if (hasTop && hasLeft  && isSameAs(x-1, y-1)) mask |= TOP_LEFT;
	if (hasTop && hasRight && isSameAs(x+1, y-1)) mask |= TOP_RIGHT;
	if (hasBot && hasLeft  && isSameAs(x-1, y+1)) mask |= BOT_LEFT;
	if (hasBot && hasRight && isSameAs(x+1, y+1)) mask |= BOT_RIGHT;

	return mask;
}


void TileMap::left(GameMap const& gameMap, Vector2i const& offset)
{
	for (int i = 0; i < META_TILE::COUNT_H; ++i)
	{
		m_tileMap[i].pushBackNoRealloc();
		m_tileMap[i].back().setType(gameMap.getTile(offset + Vector2i{1,i}));
	}
}
void TileMap::right(GameMap const& gameMap, Vector2i const& offset)
{
	for (int i = 0; i < META_TILE::COUNT_H; ++i)
	{
		m_tileMap[i].front().setType(gameMap.getTile(offset + Vector2i{META_TILE::COUNT_W-1,i}));
		m_tileMap[i].pushFrontNoRealloc();
		m_tileMap[i].front().setType(gameMap.getTile(offset + Vector2i{META_TILE::COUNT_W,i}));
	}
}
void TileMap::top(GameMap const& gameMap, Vector2i const& offset)
{
	m_tileMap.pushBackNoRealloc();
	for (int i = 0; i < META_TILE::COUNT_W; ++i)
		m_tileMap.back()[i].setType(gameMap.getTile(offset + Vector2i{i,1}));
}
void TileMap::bottom(GameMap const& gameMap, Vector2i const& offset)
{
	for (int i = 0; i < META_TILE::COUNT_W; ++i)
		m_tileMap.front()[i].setType(gameMap.getTile(offset + Vector2i{i,META_TILE::COUNT_H-1}));
	m_tileMap.pushFrontNoRealloc();
	for (int i = 0; i < META_TILE::COUNT_W; ++i)
		m_tileMap.front()[i].setType(gameMap.getTile(offset + Vector2i{i,META_TILE::COUNT_H}));
}
