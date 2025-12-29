#include "GameMap.h"
#include "Camera.h"
#include "GameContext.h"
#include <limits>

using namespace std;

// Alias for better readability
namespace MT = META_TILE;

static u8 determineDirections(Vector2i const& lastOffset, Vector2i const& newOffset)
{
	return 
	    ((lastOffset.x > newOffset.x) ? DIRECTION::LEFT : 0) |
	    ((lastOffset.x < newOffset.x) ? DIRECTION::RIGHT : 0) |
	    ((lastOffset.y > newOffset.y) ? DIRECTION::TOP : 0) |
	    ((lastOffset.y < newOffset.y) ? DIRECTION::BOT : 0);
}


void GameMap::update(float dt)
{
	if(Vector2i const& offset = GameContext::get().camera->getMetaTileOffset();  offset != m_offset)
	{
		u8 directions = determineDirections(m_offset,offset);
		m_offset = offset;
		if(directions & DIRECTION::LEFT) left();
		else if(directions & DIRECTION::RIGHT) right();
		if(directions & DIRECTION::TOP) top();
		else if(directions & DIRECTION::BOT) bottom();
		m_tileMap.calculateConnections();
		m_tileMap.flush();
	}

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
	//temporary
	unsigned int globalSeed = time(0);

	// Generate deterministic seed for this chunk
	unsigned int seed = globalSeed ^ (chunkCoordinate.x * 73856093) ^ (chunkCoordinate.y * 19349663);
	std::mt19937 rng(seed);
	std::uniform_int_distribution<> roomChance(0, 100);

	// Try to generate a room in this chunk
	if (roomChance(rng) < 60) // 40% chance to create a room
	{
		Vector2i roomSize = {
			2 + static_cast<int>(rng() % 4),
			2 + static_cast<int>(rng() % 4)
		};

		Vector2i roomCoord = {
			chunkCoordinate.x * GAME_MAP::CHUNK_SIZE + static_cast<int>(rng() % roomSize.x),
			chunkCoordinate.y * GAME_MAP::CHUNK_SIZE + static_cast<int>(rng() % roomSize.y)
		};

		Room newRoom{roomCoord, roomSize};

		// Check for overlap with existing rooms
		bool overlaps = false;
		for (const auto& room : m_reservedRooms)
		{
			Room o = room.oversized(1);
			if (roomCoord.x < o.coordinate.x + o.size.x &&
				roomCoord.x + roomSize.x > o.coordinate.x &&
				roomCoord.y < o.coordinate.y + o.size.y &&
				roomCoord.y + roomSize.y > o.coordinate.y)
			{
				overlaps = true;
				break;
			}
		}

		if (!overlaps)
		{
			connectNearestRoom(newRoom, rng);
			createRoom(newRoom);
		}
	}
		
	m_generatedChunks[chunkCoordinate] = true;
}

// Function to compute the center of a room
Vector2i getRoomCenter(const Room& room) {
	return { room.coordinate.x + room.size.x / 2, room.coordinate.y + room.size.y / 2 };
}

// Function to connect the new room to one of the nearest reserved rooms
// newRoom: the room to connect (not yet in m_reservedRooms)
// rng: a random number generator passed from the caller
void GameMap::connectNearestRoom(const Room& newRoom, std::mt19937& rng)
{
	auto connectRooms = [&](const Room& a, const Room& b)
	{
		Vector2i centerA = {a.coordinate.x + a.size.x / 2, a.coordinate.y + a.size.y / 2};
		Vector2i centerB = {b.coordinate.x + b.size.x / 2, b.coordinate.y + b.size.y / 2};

		// Corridor horizontal then vertical
		for (int x = std::min(centerA.x, centerB.x); x <= std::max(centerA.x, centerB.x); ++x)
			collapseTile({x, centerA.y});

		for (int y = std::min(centerA.y, centerB.y); y <= std::max(centerA.y, centerB.y); ++y)
			collapseTile({centerB.x, y});
	};


	if (m_reservedRooms.empty())
		return; // Nothing to connect to

	// Calculate the center of the new room
	Vector2i newCenter = getRoomCenter(newRoom);
	
	// List to hold indices of rooms with the minimum distance squared
	std::vector<size_t> candidateIndices;
	int minDistSq = std::numeric_limits<int>::max();
	
	// Traverse all reserved rooms to find those with the smallest distance
	for (size_t i = 0; i < m_reservedRooms.size(); ++i)
	{
		Vector2i roomCenter = getRoomCenter(m_reservedRooms[i]);
		int dx = newCenter.x - roomCenter.x;
		int dy = newCenter.y - roomCenter.y;
		int distSq = dx * dx + dy * dy;
		
		if (distSq < minDistSq)
		{
			minDistSq = distSq;
			candidateIndices.clear();
			candidateIndices.push_back(i);
		}
		else if (distSq == minDistSq)
			candidateIndices.push_back(i);
	}
	
	// Randomly select one candidate room from the candidates
	if (!candidateIndices.empty())
	{
		std::uniform_int_distribution<> candidateDist(0, candidateIndices.size() - 1);
		size_t chosenIndex = candidateIndices[candidateDist(rng)];
		connectRooms(m_reservedRooms[chosenIndex], newRoom);
	}
}




void GameMap::updatePlayerChunk()
{
	Vector2i center = getPlayerChunk();
	int radius = 3;

	for (int x = -radius; x <= radius; ++x)
		for (int y = -radius; y <= radius; ++y)
			addToQueue(center + Vector2i{x, y});
}

void GameMap::collapseTile(const Vector2i& tileCoordinate)
{
	m_map[tileCoordinate] = MT::Type::Path;
}

void GameMap::createRoom(const Room& room)
{
	m_reservedRooms.push_back(room);

	for (int dx = 0; dx < room.size.x; ++dx)
		for (int dy = 0; dy < room.size.y; ++dy)
			collapseTile({room.coordinate.x + dx, room.coordinate.y + dy});
}

void GameMap::addToQueue(const Vector2i& chunkCoordinate)
{
	if(isChunkGenerated(chunkCoordinate)) return;

	m_chunksToGenerate.push(chunkCoordinate);
	m_generatedChunks[chunkCoordinate] = true;
}

const Vector2i GameMap::getPlayerChunk() const
{
	if(GameContext::get().player == nullptr) return {0,0};
	const Vector2i playerCoo = GameContext::get().player->getCoordinates();
	return playerCoo / GAME_MAP::CHUNK_SIZE - Vector2i{playerCoo.x < 0, playerCoo.y < 0};
}

void GameMap::loadDisplayableTilesIntoTileMap()
{
	m_tileMap.calculateConnections();
	int rows = MT::COUNT_W - WORD_BORDER_SIZE;
	int cols = MT::COUNT_H - WORD_BORDER_SIZE;

	// 2 represent how many tiles are out of camera fov
	for (int i = WORD_BORDER_SIZE; i < rows; ++i)
		for (int j = WORD_BORDER_SIZE; j < cols; ++j)
			m_tileMap[i][j].setType(getTile({m_offset.x+j, m_offset.y+i}));
}

void GameMap::left()
{
	m_tileMap.left(*this, m_offset);
}
void GameMap::right()
{
	m_tileMap.right(*this, m_offset);
}
void GameMap::top()
{
	m_tileMap.top(*this, m_offset);
}
void GameMap::bottom()
{
	m_tileMap.bottom(*this, m_offset);
}



GameMap::GameMap(string name): GameObject(name)
{
	m_offset = GameContext::get().camera->getMetaTileOffset();

	//not sure why but those are absolute necessity
	left(); top();

	createRoom({{-2,-2},{5,5}});
	generateChunk({0,0});
	updatePlayerChunk();
	while(!m_chunksToGenerate.empty())
	{
		generateChunk(m_chunksToGenerate.front());
		m_chunksToGenerate.pop();
	}
	loadDisplayableTilesIntoTileMap();
	m_tileMap.calculateConnections();
	m_tileMap.flush();
}