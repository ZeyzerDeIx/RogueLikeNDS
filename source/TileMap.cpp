#include "TileMap.h"

#include "Camera.h"
#include "GameContext.h"
#include "GameMap.h"
#include "TileSet.h"

u16 TileMap::m_BgTileMap[SUB_TILE::COUNT_W][SUB_TILE::COUNT_H];

TileMap::TileMap(GameMap* map): m_GameMap(map)
{
	// Load tiles data in memory
	swiFastCopy(TileSetTiles, bgGetGfxPtr(BG::ID), TileSetTilesLen>>2);
	swiFastCopy(TileSetPal, BG_PALETTE, TileSetPalLen>>2);

	UpdateAllMetaTiles({0,0});
}

void TileMap::Flush() // NOLINT(*-convert-member-functions-to-static)
{
	dmaCopy(m_BgTileMap, bgGetMapPtr(BG::ID), sizeof(m_BgTileMap));
}

u16* TileMap::operator[](int key)
{
	return m_BgTileMap[key];
}


void TileMap::UpdateAllMetaTiles(Vector2i const& offset)
{
	for (int localX = 0; localX < META_TILE::COUNT_W; ++localX)
		for (int localY = 0; localY < META_TILE::COUNT_H; ++localY)
			UpdateMetaTile({offset.x + localX, offset.y + localY});
}

[[nodiscard]] constexpr int WrapPos16(int val) noexcept
{
	return val & 0xF;
}

void TileMap::UpdateTileIfVisible(const Vector2i& worldPos, bool updateNeighbors)
{
	Vector2i windowPos = m_GameMap->GetLateOffset();

	if (worldPos.x >= windowPos.x && worldPos.x < windowPos.x + 16 &&
		worldPos.y >= windowPos.y && worldPos.y < windowPos.y + 16)
	{
		UpdateMetaTile(worldPos);
	}

	if (!updateNeighbors) return;

	UpdateTileIfVisible({worldPos.x - 1, worldPos.y}, false);
	UpdateTileIfVisible({worldPos.x + 1, worldPos.y}, false);
	UpdateTileIfVisible({worldPos.x, worldPos.y - 1}, false);
	UpdateTileIfVisible({worldPos.x, worldPos.y + 1}, false);

	UpdateTileIfVisible({worldPos.x - 1, worldPos.y - 1}, false);
	UpdateTileIfVisible({worldPos.x + 1, worldPos.y - 1}, false);
	UpdateTileIfVisible({worldPos.x - 1, worldPos.y + 1}, false);
	UpdateTileIfVisible({worldPos.x + 1, worldPos.y + 1}, false);
}

void TileMap::UpdateMetaTile(Vector2i const& worldPos) const
{
	MetaTile metaTile(m_GameMap->GetTile(worldPos));

	int con = 0;

	auto SameType = [&](int X, int Y){ return m_GameMap->GetTile({X, Y}) == metaTile.GetType(); };

	// Cardinales
	if (SameType(worldPos.x, worldPos.y - 1)) con |= DIRECTION::TOP;
	if (SameType(worldPos.x, worldPos.y + 1)) con |= DIRECTION::BOT;
	if (SameType(worldPos.x - 1, worldPos.y)) con |= DIRECTION::LEFT;
	if (SameType(worldPos.x + 1, worldPos.y)) con |= DIRECTION::RIGHT;

	// Diagonales
	if (SameType(worldPos.x - 1, worldPos.y - 1)) con |= DIRECTION::TOP_LEFT;
	if (SameType(worldPos.x + 1, worldPos.y - 1)) con |= DIRECTION::TOP_RIGHT;
	if (SameType(worldPos.x - 1, worldPos.y + 1)) con |= DIRECTION::BOT_LEFT;
	if (SameType(worldPos.x + 1, worldPos.y + 1)) con |= DIRECTION::BOT_RIGHT;

	metaTile.SetConnections(con);

	metaTile.Flush(m_BgTileMap, {WrapPos16(worldPos.x), WrapPos16(worldPos.y)});
}

void TileMap::UpdateMetaTileLine(int direction, Vector2i const& offset)
{
	switch (direction)
	{
	case DIRECTION::TOP:
		for (int x = 0; x < META_TILE::COUNT_W; ++x)
			UpdateMetaTile({offset.x + x, offset.y});
		break;
	case DIRECTION::BOT:
		for (int x = 0; x < META_TILE::COUNT_W; ++x)
			UpdateMetaTile({offset.x + x, offset.y + META_TILE::COUNT_H-1});
		break;
	case DIRECTION::LEFT:
		for (int y = 0; y < META_TILE::COUNT_H; ++y)
			UpdateMetaTile({offset.x, offset.y + y});
		break;
	case DIRECTION::RIGHT:
		for (int y = 0; y < META_TILE::COUNT_H; ++y)
			UpdateMetaTile({offset.x + META_TILE::COUNT_W-1, offset.y + y});
		break;
	default:;
	}
}
