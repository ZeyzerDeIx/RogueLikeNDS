#include "MetaTile.h"

using namespace META_TILE;

MetaTile::MetaTile(META_TILE::Type type):
	m_tiles
	{
		Tile(CORNER::TOP_LEFT),
		Tile(CORNER::BOT_LEFT),
		Tile(CORNER::TOP_RIGHT),
		Tile(CORNER::BOT_RIGHT)
	},
	m_type(type)
{}

void MetaTile::flush(std::span<u16[SUB_TILE::COUNT_H]> bgTileMap, Vector2i pos)
{
	pos *= 2;
	m_tiles[CORNER::TOP_LEFT ].flush(bgTileMap, m_type, pos);
	m_tiles[CORNER::TOP_RIGHT].flush(bgTileMap, m_type, {pos.x+1, pos.y  });
	m_tiles[CORNER::BOT_LEFT ].flush(bgTileMap, m_type, {pos.x  , pos.y+1});
	m_tiles[CORNER::BOT_RIGHT].flush(bgTileMap, m_type, {pos.x+1, pos.y+1});
}

const Type& MetaTile::getType() const
{
	return m_type;
}

void MetaTile::setType(const META_TILE::Type& type)
{
	m_type = type;
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
void MetaTile::setConnections(u8 connections)
{
    m_connections = connections;

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
        m_tiles[corner].setConnections(cornerFilter(cornerValue));
    };

    // Apply the lambda to all corners
    setCorCon(Cor::TOP_LEFT,  Dir::LEFT, Dir::TOP,  Dir::TOP_LEFT);
    setCorCon(Cor::BOT_LEFT, Dir::LEFT, Dir::BOT, Dir::BOT_LEFT);
    setCorCon(Cor::TOP_RIGHT,  Dir::RIGHT, Dir::TOP,  Dir::TOP_RIGHT);
    setCorCon(Cor::BOT_RIGHT, Dir::RIGHT, Dir::BOT, Dir::BOT_RIGHT);
}
