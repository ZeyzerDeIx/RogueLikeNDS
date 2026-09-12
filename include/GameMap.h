#pragma once

#include <bitset>

#include "pch.h"
#include "TileMap.h"
#include "Room.h"
#include "GameObject.h"


class GameMap: public GameObject
{
public:
	void Update(float dt) override;

	META_TILE::Type GetTile(const Vector2i& tileCoordinate) const;

	bool IsCrossable(const Vector2i& tileCoordinate) const;
	bool IsChunkGenerated(const Vector2i& chunkCoordinate) const;

	void SetChunkGenerated(const Vector2i& chunkCoordinate);
	void GenerateChunk(const Vector2i& chunkCoordinate);
	[[nodiscard]] Vector2i GetLateOffset() const;

private:
	std::array<META_TILE::Type, GAME_MAP::SIZE_W * GAME_MAP::SIZE_H> m_Map;
	std::bitset<GAME_MAP::CHUNKS_W * GAME_MAP::CHUNKS_H> m_GeneratedChunks;
	std::queue<Vector2i> m_ChunksToGenerate;
	TileMap m_TileMap;
	Vector2i m_PlayerChunk;
	std::vector<Room> m_ReservedRooms;

	Vector2i m_LastOffset = {0,0};

	void SetTile(const Vector2i& tileCoordinate, META_TILE::Type tileType);

	void UpdatePlayerChunk();
	void CollapseTile(const Vector2i& tileCoordinate);
	void CreateRoom(const Room& room);
	void AddToQueue(const Vector2i& chunkCoordinate);
	const Vector2i GetPlayerChunk() const;
	void ConnectNearestRoom(const Room& newRoom, FastRNG& rng);

	GameMap(std::string name);

	friend class GameObject;
};