#include "TileMap.h"

u8 TileMap::m_bgTileMap[SUB_TILE::COUNT_W][SUB_TILE::COUNT_H];

TileMap::TileMap(Vector2i size): 
	m_tileMap(TILE::COUNT_W, std::vector<u8>(TILE::COUNT_H, 0))
{}

void TileMap::flush()
{
	convertMap();
	swiFastCopy(m_bgTileMap, bgGetMapPtr(BG::ID), sizeof(m_bgTileMap)>>2);
}

void TileMap::convertMap()
{
	constexpr int ratio = (TILE::SIZE / SUB_TILE::SIZE);
	for (uint i = 0 ; i < m_tileMap.size() ; ++i)
	{
		for (uint j = 0 ; j < m_tileMap[i].size() ; ++j)
		{
			u8 ir = i*ratio;
			u8 jr = j*ratio;
			u8 xAxe = m_tileMap[i][j]%TILESET::COUNT_W;
			u8 yAxe = m_tileMap[i][j]-xAxe;
			u8 tile = xAxe * ratio + yAxe * ratio*ratio ;
			m_bgTileMap[ir  ][jr  ] = tile;
			m_bgTileMap[ir+1][jr  ] = tile + TILESET::COUNT_W * ratio;
			m_bgTileMap[ir  ][jr+1] = tile + 1;
			m_bgTileMap[ir+1][jr+1] = tile + TILESET::COUNT_W * ratio + 1;
		}
	}
}

std::vector<u8>& TileMap::operator[](int key)
{
	return m_tileMap[key];
}