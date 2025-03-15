#include "GameMap.h"

// Alias for better readability
namespace MT = META_TILE;

MT::Type getRandomTileType()
{
	return static_cast<MT::Type>(rand() % static_cast<int>(MT::Type::Last));
}
GameMap::GameMap(TileMap& tileMap, Camera& camera):
	m_map(GAME_MAP::SIZE_W, std::vector<MT::Type>(GAME_MAP::SIZE_H, MT::Type::Path)),
	m_tileMap(tileMap),
	m_camera(camera)
{
	for(auto& row: m_map)
		for(auto& element: row)
			element = getRandomTileType();
}


void GameMap::update()
{
	loadDisplayableTilesIntoTileMap();
}


void GameMap::loadDisplayableTilesIntoTileMap()
{
	const Vector2i offset = m_camera.getMetaTileOffset();

	auto get = [&](int x, int y)
	{
		if(x < 0 or x >= static_cast<int>(m_map.size()) or
		   y < 0 or y >= static_cast<int>(m_map[0].size()))
			return MT::Type::Wall;
		else return m_map[x][y];
	};

	for (int i = 0; i < MT::COUNT_W; ++i)
		for (int j = 0; j < MT::COUNT_H; ++j)
			m_tileMap[i][j].setType(get(offset.y + i, offset.x + j));

	m_tileMap.calculateConnections();
}