#include "TileMap.h"

u8 TileMap::m_bgTileMap[SUB_TILE::COUNT_W][SUB_TILE::COUNT_H];

TileMap::TileMap(): 
	m_tileMap(META_TILE::COUNT_W, std::vector<MetaTile>(META_TILE::COUNT_H))
{}

void TileMap::flush()
{
	convertMap();
	swiFastCopy(m_bgTileMap, bgGetMapPtr(BG::ID), sizeof(m_bgTileMap)>>2);
}

void TileMap::convertMap()
{
	for (uint i = 0 ; i < m_tileMap.size() ; ++i)
		for (uint j = 0 ; j < m_tileMap[i].size() ; ++j)
			m_tileMap[i][j].flush(m_bgTileMap, {(int)i, (int)j});
}

std::vector<MetaTile>& TileMap::operator[](int key)
{
	return m_tileMap[key];
}