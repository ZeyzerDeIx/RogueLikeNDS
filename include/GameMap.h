#pragma once

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

	void GenerateChunk(const Vector2i& chunkCoordinate);
	[[nodiscard]] Vector2i GetLateOffset() const;

private:
	// Speed to expand but also to access + do not store useless data
	std::unordered_map<Vector2i, META_TILE::Type, NDSMath::HashVector2i> m_Map;
	std::unordered_map<Vector2i, bool, NDSMath::HashVector2i> m_GeneratedChunks;
	std::queue<Vector2i> m_ChunksToGenerate;
	TileMap m_TileMap;
	Vector2i m_PlayerChunk;
	std::vector<Room> m_ReservedRooms;

	Vector2i m_LastOffset = {0,0};

	void UpdatePlayerChunk();
	void CollapseTile(const Vector2i& tileCoordinate);
	void CreateRoom(const Room& room);
	void AddToQueue(const Vector2i& chunkCoordinate);
	const Vector2i GetPlayerChunk() const;
	void ConnectNearestRoom(const Room& newRoom, std::mt19937& rng);

	GameMap(std::string name);

	friend class GameObject;
};