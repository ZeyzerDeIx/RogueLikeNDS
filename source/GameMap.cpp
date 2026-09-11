#include "GameMap.h"
#include "Camera.h"
#include "GameContext.h"
#include <limits>

using namespace std;

// Alias for better readability
namespace MT = META_TILE;


void GameMap::Update(float dt)
{
	if(Vector2i const& offset = GameContext::get().m_Camera->GetMetaTileOffset();  offset != m_LastOffset)
	{
		LoadDisplayableTilesIntoTileMap(offset);
		m_TileMap.Flush();
	}

	if(m_PlayerChunk != GetPlayerChunk())
		UpdatePlayerChunk();

	if(!m_ChunksToGenerate.empty())
	{
		GenerateChunk(m_ChunksToGenerate.front());
		m_ChunksToGenerate.pop();
	}
}

MT::Type GameMap::GetTile(const Vector2i& tileCoordinate) const
{
	auto pair = m_Map.find(tileCoordinate);

	if (pair == m_Map.end())
		return MT::Type::Wall;
	
	return pair->second;
}

bool GameMap::IsCrossable(const Vector2i& tileCoordinate) const
{
	return GetTile(tileCoordinate) != MT::Type::Wall;
}

bool GameMap::IsChunkGenerated(const Vector2i& chunkCoordinate) const
{	
	return m_GeneratedChunks.find(chunkCoordinate) != m_GeneratedChunks.end();
}

void GameMap::GenerateChunk(const Vector2i& chunkCoordinate)
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
		for (const auto& room : m_ReservedRooms)
		{
			Room o = room.Oversized(1);
			if (roomCoord.x < o.Coordinate.x + o.Size.x &&
				roomCoord.x + roomSize.x > o.Coordinate.x &&
				roomCoord.y < o.Coordinate.y + o.Size.y &&
				roomCoord.y + roomSize.y > o.Coordinate.y)
			{
				overlaps = true;
				break;
			}
		}

		if (!overlaps)
		{
			ConnectNearestRoom(newRoom, rng);
			CreateRoom(newRoom);
		}
	}
		
	m_GeneratedChunks[chunkCoordinate] = true;
}

// Function to compute the center of a room
Vector2i getRoomCenter(const Room& room) {
	return { room.Coordinate.x + room.Size.x / 2, room.Coordinate.y + room.Size.y / 2 };
}

// Function to connect the new room to one of the nearest reserved rooms
// newRoom: the room to connect (not yet in m_reservedRooms)
// rng: a random number generator passed from the caller
void GameMap::ConnectNearestRoom(const Room& newRoom, std::mt19937& rng)
{
	auto connectRooms = [&](const Room& a, const Room& b)
	{
		Vector2i centerA = {a.Coordinate.x + a.Size.x / 2, a.Coordinate.y + a.Size.y / 2};
		Vector2i centerB = {b.Coordinate.x + b.Size.x / 2, b.Coordinate.y + b.Size.y / 2};

		// Corridor horizontal then vertical
		for (int x = std::min(centerA.x, centerB.x); x <= std::max(centerA.x, centerB.x); ++x)
			CollapseTile({x, centerA.y});

		for (int y = std::min(centerA.y, centerB.y); y <= std::max(centerA.y, centerB.y); ++y)
			CollapseTile({centerB.x, y});
	};


	if (m_ReservedRooms.empty())
		return; // Nothing to connect to

	// Calculate the center of the new room
	Vector2i newCenter = getRoomCenter(newRoom);
	
	// List to hold indices of rooms with the minimum distance squared
	std::vector<size_t> candidateIndices;
	int minDistSq = std::numeric_limits<int>::max();
	
	// Traverse all reserved rooms to find those with the smallest distance
	for (size_t i = 0; i < m_ReservedRooms.size(); ++i)
	{
		Vector2i roomCenter = getRoomCenter(m_ReservedRooms[i]);
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
		connectRooms(m_ReservedRooms[chosenIndex], newRoom);
	}
}




void GameMap::UpdatePlayerChunk()
{
	Vector2i center = GetPlayerChunk();
	int radius = 3;

	for (int x = -radius; x <= radius; ++x)
		for (int y = -radius; y <= radius; ++y)
			AddToQueue(center + Vector2i{x, y});
}

void GameMap::CollapseTile(const Vector2i& tileCoordinate)
{
	m_Map[tileCoordinate] = MT::Type::Path;
}

void GameMap::CreateRoom(const Room& room)
{
	m_ReservedRooms.push_back(room);

	for (int dx = 0; dx < room.Size.x; ++dx)
		for (int dy = 0; dy < room.Size.y; ++dy)
			CollapseTile({room.Coordinate.x + dx, room.Coordinate.y + dy});
}

void GameMap::AddToQueue(const Vector2i& chunkCoordinate)
{
	if(IsChunkGenerated(chunkCoordinate)) return;

	m_ChunksToGenerate.push(chunkCoordinate);
	m_GeneratedChunks[chunkCoordinate] = true;
}

const Vector2i GameMap::GetPlayerChunk() const
{
	if(GameContext::get().m_Player == nullptr) return {0,0};
	const Vector2i playerCoo = GameContext::get().m_Player->GetCoordinates();
	return playerCoo / GAME_MAP::CHUNK_SIZE - Vector2i{playerCoo.x < 0, playerCoo.y < 0};
}

void GameMap::LoadDisplayableTilesIntoTileMap(Vector2i const& offset)
{
	m_LastOffset = offset;

	int rows = MT::COUNT_W - WORD_BORDER_SIZE;
	int cols = MT::COUNT_H - WORD_BORDER_SIZE;

	// 2 represent how many tiles are out of camera fov
	for (int i = WORD_BORDER_SIZE; i < rows; ++i)
		for (int j = WORD_BORDER_SIZE; j < cols; ++j)
			m_TileMap[i][j].SetType(GetTile({offset.y+i, offset.x+j}));

	m_TileMap.CalculateConnections();
}



GameMap::GameMap(string name): GameObject(name)
{
	CreateRoom({{-2,-2},{5,5}});
	GenerateChunk({0,0});
	UpdatePlayerChunk();
}