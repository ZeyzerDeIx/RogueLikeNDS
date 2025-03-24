#include "GameMap.h"
#include "Camera.h"

// Alias for better readability
namespace MT = META_TILE;

GameMap::GameMap(GameContext& context):
	m_context(context)
{
	for (int i = 0; i < 10; ++i)
		for (int j = 0; j < 10; ++j)
			m_map[{i,j}] = MT::Type::Path;
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

	int rows = MT::COUNT_W - WORD_BORDER_SIZE;
	int cols = MT::COUNT_H - WORD_BORDER_SIZE;

	// 2 represent how many tiles are out of camera fov
	for (int i = WORD_BORDER_SIZE; i < rows; ++i)
		for (int j = WORD_BORDER_SIZE; j < cols; ++j)
			m_tileMap[i][j].setType(getTile({offset.y+i, offset.x+j}));

	m_tileMap.calculateConnections();
}