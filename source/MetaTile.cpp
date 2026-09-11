#include "MetaTile.h"

using namespace META_TILE;

MetaTile::MetaTile(META_TILE::Type type):
	m_Tiles
	{
		Tile(CORNER::TOP_LEFT),
		Tile(CORNER::TOP_RIGHT),
		Tile(CORNER::BOT_LEFT),
		Tile(CORNER::BOT_RIGHT)
	},
	m_Type(type)
{}

void MetaTile::Flush(std::span<u16[SUB_TILE::COUNT_H]> bgTileMap, Vector2i pos)
{
	pos *= 2;
	m_Tiles[CORNER::TOP_LEFT ].Flush(bgTileMap, m_Type, pos);
	m_Tiles[CORNER::TOP_RIGHT].Flush(bgTileMap, m_Type, {pos.x+1, pos.y  });
	m_Tiles[CORNER::BOT_LEFT ].Flush(bgTileMap, m_Type, {pos.x  , pos.y+1});
	m_Tiles[CORNER::BOT_RIGHT].Flush(bgTileMap, m_Type, {pos.x+1, pos.y+1});
}

const Type& MetaTile::GetType() const
{
	return m_Type;
}

void MetaTile::SetType(const META_TILE::Type& type)
{
	m_Type = type;
}

constexpr u8 bitCheck(u8 source, u8 filter, u8 offset)
{
	return ((source & filter) == filter) << offset;
}
constexpr u8 cornerFilter(u8 source)
{
	if(source == 0b0000'0111) return 0b0000'0100;
	return source & 0b0000'0011;
}
void MetaTile::SetConnections(u8 connections)
{
    m_Connections = connections;

    // Alias for DIRECTION and CORNER
    namespace Dir = DIRECTION;
    namespace Cor = CORNER;

    // Helper lambda to calculate corner connections and set them
    auto setCorCon = [this, connections](u8 corner, u8 dir1, u8 dir2, u8 dir3)
    {
        u8 cornerValue = 
            bitCheck(connections, dir1, 0) | 
            bitCheck(connections, dir2, 1) | 
            bitCheck(connections, dir3, 2);
        m_Tiles[corner].SetConnections(cornerFilter(cornerValue));
    };

    // Apply the lambda to all corners
    setCorCon(Cor::TOP_LEFT,  Dir::TOP, Dir::LEFT,  Dir::TOP_LEFT);
    setCorCon(Cor::TOP_RIGHT, Dir::TOP, Dir::RIGHT, Dir::TOP_RIGHT);
    setCorCon(Cor::BOT_LEFT,  Dir::BOT, Dir::LEFT,  Dir::BOT_LEFT);
    setCorCon(Cor::BOT_RIGHT, Dir::BOT, Dir::RIGHT, Dir::BOT_RIGHT);
}
