#pragma once

#include "PCH.h"
#include "TileMap.h"
#include "GameContext.h"
#include <queue>


class GameMap
{
public:
	GameMap(GameContext& context);
	~GameMap();

	void update();

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
	GameContext& m_context;
	Vector2i m_playerChunk;

	void updatePlayerChunk();
	void collapseTile(const Vector2i& tileCoordinate);
	void addToQueue(const Vector2i& chunkCoordinate);
	const Vector2i getPlayerChunk() const;
	void loadDisplayableTilesIntoTileMap();
};