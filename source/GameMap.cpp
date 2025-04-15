#include "GameMap.h"
#include "Camera.h"

// Alias for better readability
namespace MT = META_TILE;

GameMap::GameMap(GameContext& context):
	m_context(context)
{
	generateChunk({0,0});
	updatePlayerChunk();
}


void GameMap::update()
{
	loadDisplayableTilesIntoTileMap();
	m_tileMap.flush();

	if(m_playerChunk != getPlayerChunk())
		updatePlayerChunk();

	if(!m_chunksToGenerate.empty())
	{
		generateChunk(m_chunksToGenerate.front());
		m_chunksToGenerate.pop();
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

bool GameMap::isChunkGenerated(const Vector2i& chunkCoordinate) const
{	
	return m_generatedChunks.find(chunkCoordinate) != m_generatedChunks.end();
}

void GameMap::generateChunk(const Vector2i& chunkCoordinate)
{
	for (int i = 0; i < GAME_MAP::CHUNK_SIZE; ++i)
		for (int j = 0; j < GAME_MAP::CHUNK_SIZE; ++j)
			collapseTile(Vector2i{chunkCoordinate.y,chunkCoordinate.x} * GAME_MAP::CHUNK_SIZE + Vector2i{i,j});
		
	m_generatedChunks[chunkCoordinate] = true;
}



void GameMap::updatePlayerChunk()
{
	m_playerChunk = getPlayerChunk();
	addToQueue(m_playerChunk + Vector2i{0,1});
	addToQueue(m_playerChunk + Vector2i{1,0});
	addToQueue(m_playerChunk + Vector2i{1,1});
	addToQueue(m_playerChunk - Vector2i{0,1});
	addToQueue(m_playerChunk - Vector2i{1,0});
	addToQueue(m_playerChunk - Vector2i{1,1});
	addToQueue(m_playerChunk + Vector2i{1,0} - Vector2i{0,1});
	addToQueue(m_playerChunk - Vector2i{1,0} + Vector2i{0,1});
}

void GameMap::collapseTile(const Vector2i& tileCoordinate)
{
	m_map[tileCoordinate] = MT::Type::Path;
}

void GameMap::addToQueue(const Vector2i& chunkCoordinate)
{
	if(isChunkGenerated(chunkCoordinate)) return;

	m_chunksToGenerate.push(chunkCoordinate);
	m_generatedChunks[chunkCoordinate] = true;
}

const Vector2i GameMap::getPlayerChunk() const
{
	if(m_context.player == nullptr) return {0,0};
	const Vector2i playerCoo = m_context.player->getCoordinates();
	return playerCoo / GAME_MAP::CHUNK_SIZE - Vector2i{playerCoo.x < 0, playerCoo.y < 0};
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
