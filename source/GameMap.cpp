#include "GameMap.h"
#include "Camera.h"
#include "GameContext.h"
#include <limits>

using namespace std;

// Alias for better readability
namespace MT = META_TILE;


// Petit helper pour gérer le modulo négatif en C++
inline int WrapPos(int val, int max) {
	int res = val % max;
	return res < 0 ? res + max : res;
}

// Calcule et écrit UNE SEULE tuile directement dans le buffer VRAM
void GameMap::UpdateSingleMetaTile(int worldX, int worldY)
{
	// 1. On interroge ton dictionnaire en {Y, X} comme le fait ton moteur de collision
	META_TILE::Type type = GetTile({worldY, worldX});
	u8 con = 0;

	// 2. La lambda s'adapte aussi à l'inversion
	auto t = [&](int x, int y){ return GetTile({y, x}) == type; };

	if (t(worldX, worldY-1)) con |= DIRECTION::TOP;
	if (t(worldX, worldY+1)) con |= DIRECTION::BOT;
	if (t(worldX-1, worldY)) con |= DIRECTION::LEFT;
	if (t(worldX+1, worldY)) con |= DIRECTION::RIGHT;
	if (t(worldX-1, worldY-1)) con |= DIRECTION::TOP_LEFT;
	if (t(worldX+1, worldY-1)) con |= DIRECTION::TOP_RIGHT;
	if (t(worldX-1, worldY+1)) con |= DIRECTION::BOT_LEFT;
	if (t(worldX+1, worldY+1)) con |= DIRECTION::BOT_RIGHT; 

	MetaTile metaTile(type);
	metaTile.SetConnections(con);

	int vramX = WrapPos(worldX, 16);
	int vramY = WrapPos(worldY, 16);

	// 3. CRUCIAL : MetaTile::Flush attend historiquement {Ligne, Colonne}, donc {Y, X} !
	metaTile.Flush(TileMap::m_BgTileMap, {vramY, vramX});
}

// Charge un carré 16x16 (utile au lancement ou lors d'une téléportation)
void GameMap::LoadFullRingBuffer(Vector2i const& windowPos)
{
	for(int y = 0; y < 16; ++y)
		for(int x = 0; x < 16; ++x)
			UpdateSingleMetaTile(windowPos.x + x, windowPos.y + y);
}

void GameMap::Update(float dt)
{
	// On définit notre fenêtre 16x16. On recule de 4 tuiles pour que la caméra soit au centre du buffer.
	Vector2i currentWindowPos = GameContext::get().m_Camera->GetMetaTileOffset() - Vector2i{4, 4};

	if(currentWindowPos != m_LastOffset)
	{
		Vector2i delta = currentWindowPos - m_LastOffset;

		// Si on va trop vite ou si c'est le début du jeu, on recharge tout
		if (std::abs(delta.x) > 1 || std::abs(delta.y) > 1)
		{
			LoadFullRingBuffer(currentWindowPos);
		}
		else
		{
			// Mouvement horizontal : On charge la colonne qui apparaît
			if (delta.x > 0) // Mouvement vers la droite
				for (int y = 0; y < 16; ++y) UpdateSingleMetaTile(currentWindowPos.x + 15, currentWindowPos.y + y);
			else if (delta.x < 0) // Mouvement vers la gauche
				for (int y = 0; y < 16; ++y) UpdateSingleMetaTile(currentWindowPos.x, currentWindowPos.y + y);

			// Mouvement vertical : On charge la ligne qui apparaît
			if (delta.y > 0) // Mouvement vers le bas
				for (int x = 0; x < 16; ++x) UpdateSingleMetaTile(currentWindowPos.x + x, currentWindowPos.y + 15);
			else if (delta.y < 0) // Mouvement vers le haut
				for (int x = 0; x < 16; ++x) UpdateSingleMetaTile(currentWindowPos.x + x, currentWindowPos.y);
		}

		m_LastOffset = currentWindowPos;
		m_NeedsFlush = true; // Déclenchera le Display() dans le main.cpp
	}

	// --- Reste de ton code pour les chunks ---
	if(m_PlayerChunk != GetPlayerChunk()) UpdatePlayerChunk();

	if(!m_ChunksToGenerate.empty())
	{
		GenerateChunk(m_ChunksToGenerate.front());
		m_ChunksToGenerate.pop();

		LoadFullRingBuffer(m_LastOffset);
		m_NeedsFlush = true;
	}
}

void GameMap::Display()
{
	if (m_NeedsFlush)
	{
		m_NeedsFlush = false;
		m_TileMap.Flush();
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



GameMap::GameMap(string name): GameObject(name)
{
	CreateRoom({{-2,-2},{5,5}});
	GenerateChunk({0,0});
	UpdatePlayerChunk();
}