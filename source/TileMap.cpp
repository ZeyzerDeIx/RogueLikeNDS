#include "TileMap.h"

//std::vector<std::vector<u8>> TileMap::m_bgTileMap(SUB_TILE::COUNT_W, std::vector<u8>(SUB_TILE::COUNT_H, 0));
u8 TileMap::m_bgTileMap[SUB_TILE::COUNT_W][SUB_TILE::COUNT_H];

TileMap::TileMap(Vector2i size): 
	m_tileMap(TILE::COUNT_W, std::vector<u8>(TILE::COUNT_H, 0))
{}

void TileMap::flush()
{
	convertMap();
	dmaCopy(m_bgTileMap, bgGetMapPtr(BG::ID), sizeof(m_bgTileMap));
}

void TileMap::convertMap()
{
	constexpr int ratio = (TILE::SIZE / SUB_TILE::SIZE);
	for (uint i = 0 ; i < m_tileMap.size() ; ++i)
	{
		for (uint j = 0 ; j < m_tileMap[i].size() ; ++j)
		{
			uint ir = i*ratio;
			uint jr = j*ratio;
			uint tile = m_tileMap[i][j] * ratio;
			m_bgTileMap[ir  ][jr  ] = tile;
			m_bgTileMap[ir+1][jr  ] = tile + TILESET::COUNT_W;
			m_bgTileMap[ir  ][jr+1] = tile + 1;
			m_bgTileMap[ir+1][jr+1] = tile + TILESET::COUNT_W + 1;
		}
	}
}

std::vector<u8>& TileMap::operator[](int key)
{
	return m_tileMap[key];
}