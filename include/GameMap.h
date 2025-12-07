#pragma once

#include "pch.h"
#include "TileMap.h"
#include "Room.h"
#include "GameObject.h"


class GameMap: public GameObject
{
public:
	virtual void update(float dt);

	META_TILE::Type getTile(const Vector2i& tileCoordinate) const;

	bool isCrossable(const Vector2i& tileCoordinate) const;
	bool isChunkGenerated(const Vector2i& chunkCoordinate) const;

	void generateChunk(const Vector2i& chunkCoordinate);

private:
	// Speed to expand but also to access + do not store useless data
	std::unordered_map<Vector2i, META_TILE::Type, NDSMath::HashVector2i> m_map;
	std::unordered_map<Vector2i, bool, NDSMath::HashVector2i> m_generatedChunks;
	std::queue<Vector2i> m_chunksToGenerate;
	TileMap m_tileMap;
	Vector2i m_playerChunk;
	std::vector<Room> m_reservedRooms;

	Vector2i m_lastOffset = {0,0};

	void updatePlayerChunk();
	void collapseTile(const Vector2i& tileCoordinate);
	void createRoom(const Room& room);
	void addToQueue(const Vector2i& chunkCoordinate);
	const Vector2i getPlayerChunk() const;
	void loadDisplayableTilesIntoTileMap(Vector2i const& offset);
	void connectNearestRoom(const Room& newRoom, std::mt19937& rng);

	GameMap(std::string name);

	friend class GameObject;
};