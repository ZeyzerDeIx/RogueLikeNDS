#include "GameMap.h"
#include "Camera.h"

// Alias for better readability
namespace MT = META_TILE;

MT::Type getRandomTileType()
{
	return static_cast<MT::Type>(rand() % static_cast<int>(MT::Type::Debug));
}
GameMap::GameMap(GameContext& context):
	m_map(GAME_MAP::SIZE_W, std::vector<MT::Type>(GAME_MAP::SIZE_H, MT::Type::Path)),
	m_context(context)
{
	for(auto& row: m_map)
		for(auto& element: row)
			element = getRandomTileType();
}


void GameMap::update()
{
	loadDisplayableTilesIntoTileMap();
	m_tileMap.flush();
}

MT::Type GameMap::getTile(const Vector2i& tileCoordinate) const
{
	if (tileCoordinate.x < 0 || tileCoordinate.y < 0 ||
		tileCoordinate.x >= static_cast<int>(m_map.size()) ||
		tileCoordinate.y >= static_cast<int>(m_map[0].size()))
		return MT::Type::Wall;
	
	return m_map[tileCoordinate.x][tileCoordinate.y];
}

bool GameMap::isCrossable(const Vector2i& tileCoordinate) const
{
	return getTile(tileCoordinate) != MT::Type::Wall;
}


void GameMap::loadDisplayableTilesIntoTileMap()
{
	const Vector2i offset = m_context.camera->getMetaTileOffset();

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