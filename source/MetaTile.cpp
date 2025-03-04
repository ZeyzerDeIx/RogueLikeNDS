#include "MetaTile.h"

using namespace META_TILE;
using META_TILE::CORNER;

MetaTile::MetaTile(META_TILE::Type type):
	m_tiles
	{
		Tile(TOP_LEFT),
		Tile(TOP_RIGHT),
		Tile(BOT_LEFT),
		Tile(BOT_RIGHT)
	},
	m_type(type)
{}

void MetaTile::flush(std::span<u8[SUB_TILE::COUNT_H]> bgTileMap, Vector2i pos)
{
	pos *= 2;
	m_tiles[TOP_LEFT ].flush(bgTileMap, pos);
	m_tiles[TOP_RIGHT].flush(bgTileMap, {pos.x  , pos.y+1});
	m_tiles[BOT_LEFT ].flush(bgTileMap, {pos.x+1, pos.y  });
	m_tiles[BOT_RIGHT].flush(bgTileMap, {pos.x+1, pos.y+1});
}

void MetaTile::setConnections(u8 connections)
{
	m_connections = connections;
}