#include "GameMap.h"
#include "Camera.h"

// Alias for better readability
namespace MT = META_TILE;

MT::Type getRandomTileType()
{
	return static_cast<MT::Type>(rand() % static_cast<int>(MT::Type::Debug));
}
GameMap::GameMap(GameContext& context):
	m_context(context)
{
	m_map[{0,0}] = MT::Type::Path;
	m_map[{1,0}] = MT::Type::Path;
	m_map[{0,1}] = MT::Type::Path;
	m_map[{1,1}] = MT::Type::Path;
	m_map[{0,2}] = MT::Type::Path;
	m_map[{1,2}] = MT::Type::Path;
	m_map[{2,0}] = MT::Type::Path;
	m_map[{2,1}] = MT::Type::Path;
	m_map[{2,2}] = MT::Type::Path;
}


void GameMap::update()
{
	loadDisplayableTilesIntoTileMap();
	m_tileMap.flush();
}

MT::Type GameMap::getTile(const Vector2i& tileCoordinate) const
{
	auto pair = m_map.find(tileCoordinate);

	if (pair == m_map.end())
		return MT::Type::Wall;
	
	return pair->second;
}

bool GameMap::isCrossable(const Vector2i& tileCoordinate) const
{
	return getTile(tileCoordinate) != MT::Type::Wall;
}


void GameMap::loadDisplayableTilesIntoTileMap()
{
	const Vector2i offset = m_context.camera->getMetaTileOffset();

	for (int i = 0; i < MT::COUNT_W; ++i)
		for (int j = 0; j < MT::COUNT_H; ++j)
			m_tileMap[i][j].setType(getTile({offset.y + i, offset.x + j}));

	m_tileMap.calculateConnections();
}