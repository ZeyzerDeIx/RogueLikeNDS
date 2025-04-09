#include "GameMap.h"
#include "Camera.h"

// Alias for better readability
namespace MT = META_TILE;

GameMap::GameMap(GameContext& context):
	m_context(context)
{
	generateChunck({0,0});
	updatePlayerChunck();
}


void GameMap::update()
{
	loadDisplayableTilesIntoTileMap();
	m_tileMap.flush();

	if(m_playerChunck != getPlayerChunck())
		updatePlayerChunck();

	if(!m_chuncksToGenerate.empty())
	{
		generateChunck(m_chuncksToGenerate.front());
		m_chuncksToGenerate.pop();
	}
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

bool GameMap::isChunckGenerated(const Vector2i& chunckCoordinate) const
{	
	return m_generatedChuncks.find(chunckCoordinate) != m_generatedChuncks.end();
}

void GameMap::generateChunck(const Vector2i& chunckCoordinate)
{
	for (int i = 0; i < GAME_MAP::CHUNCK_SIZE; ++i)
		for (int j = 0; j < GAME_MAP::CHUNCK_SIZE; ++j)
			collapseTile(Vector2i{chunckCoordinate.y,chunckCoordinate.x} * GAME_MAP::CHUNCK_SIZE + Vector2i{i,j});
	m_generatedChuncks[chunckCoordinate] = true;
}



void GameMap::updatePlayerChunck()
{
	m_playerChunck = getPlayerChunck();
	addToQueue(m_playerChunck + Vector2i{0,1});
	addToQueue(m_playerChunck + Vector2i{1,0});
	addToQueue(m_playerChunck + Vector2i{1,1});
	addToQueue(m_playerChunck - Vector2i{0,1});
	addToQueue(m_playerChunck - Vector2i{1,0});
	addToQueue(m_playerChunck - Vector2i{1,1});
	addToQueue(m_playerChunck + Vector2i{1,0} - Vector2i{0,1});
	addToQueue(m_playerChunck - Vector2i{1,0} + Vector2i{0,1});
}

void GameMap::collapseTile(const Vector2i& tileCoordinate)
{
	m_map[tileCoordinate] = MT::Type::Path;
}

void GameMap::addToQueue(const Vector2i& chunckCoordinate)
{
	if(isChunckGenerated(chunckCoordinate)) return;

	m_chuncksToGenerate.push(chunckCoordinate);
	m_generatedChuncks[chunckCoordinate] = true;
}

const Vector2i GameMap::getPlayerChunck() const
{
	if(m_context.player == nullptr) return {0,0};
	const Vector2i playerCoo = m_context.player->getCoordinates();
	return playerCoo / GAME_MAP::CHUNCK_SIZE - Vector2i{playerCoo.x < 0, playerCoo.y < 0};
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
