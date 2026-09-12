#include "GameMap.h"
#include "Camera.h"
#include "GameContext.h"
#include <limits>

using namespace std;


void GameMap::Update(float dt)
{
	if(Vector2i const& offset = GameContext::Get().m_Camera->GetMetaTileOffset();  offset != m_LastOffset)
	{
		Vector2i delta = offset - m_LastOffset;

		if (std::abs(delta.x) > 1 || std::abs(delta.y) > 1)
		{
			m_TileMap.UpdateAllMetaTiles(offset);
		}
		else
		{
			if (delta.x > 0)
				m_TileMap.UpdateMetaTileLine(DIRECTION::RIGHT, offset);
			else if (delta.x < 0)
				m_TileMap.UpdateMetaTileLine(DIRECTION::LEFT, offset);

			if (delta.y > 0)
				m_TileMap.UpdateMetaTileLine(DIRECTION::BOT, offset);
			else if (delta.y < 0)
				m_TileMap.UpdateMetaTileLine(DIRECTION::TOP, offset);
		}

		m_LastOffset = offset;
	}

	m_TileMap.Flush();

	if(m_PlayerChunk != GetPlayerChunk())
		UpdatePlayerChunk();

	if(!m_ChunksToGenerate.empty())
	{
		GenerateChunk(m_ChunksToGenerate.front());
		m_ChunksToGenerate.pop();
	}
}

META_TILE::Type GameMap::GetTile(const Vector2i& tileCoordinate) const
{
	Vector2i realCoordinate = tileCoordinate + Vector2i{GAME_MAP::SIZE_W/2,GAME_MAP::SIZE_H/2};
	if (realCoordinate.x < 0 || realCoordinate.x >= GAME_MAP::SIZE_W ||
		realCoordinate.y < 0 || realCoordinate.y >= GAME_MAP::SIZE_H)
	{
		return META_TILE::Type::Wall;
	}
	return m_Map[realCoordinate.x * GAME_MAP::SIZE_W + realCoordinate.y];
}

bool GameMap::IsCrossable(const Vector2i& tileCoordinate) const
{
	return GetTile(tileCoordinate) != META_TILE::Type::Wall;
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
	FastRNG rng{seed};

	// Try to generate a room in this chunk
	if (rng.Range(0,100) < 60) // 40% chance to create a room
	{
		Vector2i roomSize = {
			rng.Range(2,5),
			rng.Range(2,5)
		};

		Vector2i roomCoord = {
			chunkCoordinate.x * GAME_MAP::CHUNK_SIZE + rng.Range(0, roomSize.x - 1),
			chunkCoordinate.y * GAME_MAP::CHUNK_SIZE + rng.Range(0, roomSize.y - 1)
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

Vector2i GameMap::GetLateOffset() const
{
	return m_LastOffset;
}

// Function to compute the center of a room
Vector2i GetRoomCenter(const Room& room) {
	return { room.Coordinate.x + room.Size.x / 2, room.Coordinate.y + room.Size.y / 2 };
}

// Function to connect the new room to one of the nearest reserved rooms
// newRoom: the room to connect (not yet in m_reservedRooms)
// rng: a random number generator passed from the caller
void GameMap::ConnectNearestRoom(const Room& newRoom, FastRNG& rng)
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
	Vector2i newCenter = GetRoomCenter(newRoom);
	
	// List to hold indices of rooms with the minimum distance squared
	std::vector<size_t> candidateIndices;
	int minDistSq = std::numeric_limits<int>::max();
	
	// Traverse all reserved rooms to find those with the smallest distance
	for (size_t i = 0; i < m_ReservedRooms.size(); ++i)
	{
		Vector2i roomCenter = GetRoomCenter(m_ReservedRooms[i]);
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
		size_t chosenIndex = candidateIndices[rng.Range(0, candidateIndices.size() - 1)];
		connectRooms(m_ReservedRooms[chosenIndex], newRoom);
	}
}


void GameMap::SetTile(const Vector2i& tileCoordinate, META_TILE::Type tileType)
{
	Vector2i realCoordinate = tileCoordinate + Vector2i{GAME_MAP::SIZE_W/2,GAME_MAP::SIZE_H/2};
	if (realCoordinate.x < 0 || realCoordinate.x >= GAME_MAP::SIZE_W ||
		realCoordinate.y < 0 || realCoordinate.y >= GAME_MAP::SIZE_H)
	{
		return;
	}
	m_Map[realCoordinate.x * GAME_MAP::SIZE_W + realCoordinate.y] = tileType;
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
	SetTile(tileCoordinate, META_TILE::Type::Path);
	m_TileMap.UpdateTileIfVisible(tileCoordinate);
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
	if(GameContext::Get().m_Player == nullptr) return {0,0};
	const Vector2i playerCoo = GameContext::Get().m_Player->GetCoordinates();
	return playerCoo / GAME_MAP::CHUNK_SIZE - Vector2i{playerCoo.x < 0, playerCoo.y < 0};
}



GameMap::GameMap(string name): GameObject(name), m_TileMap(this)
{
	m_Map.fill(META_TILE::Type::Wall);
	CreateRoom({{-2,-2},{5,5}});
	GenerateChunk({0,0});
	UpdatePlayerChunk();
}