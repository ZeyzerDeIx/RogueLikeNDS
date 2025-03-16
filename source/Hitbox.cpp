#include "Hitbox.h"
#include "GameMap.h"

Hitbox::Hitbox(Rect<int> bounds):
	m_bounds(bounds)
{}

Hitbox::~Hitbox(){}

bool Hitbox::intersects(const Hitbox& other) const
{
	// Check if there is no intersection between the two rectangles
	return !(m_bounds.x + m_bounds.w <= other.m_bounds.x ||       // This is completely to the left of other
			 other.m_bounds.x + other.m_bounds.w <= m_bounds.x || // Other is completely to the left of this
			 m_bounds.y + m_bounds.h <= other.m_bounds.y ||       // This is completely above other
			 other.m_bounds.y + other.m_bounds.h <= m_bounds.y);  // Other is completely above this
}

bool Hitbox::intersects(const GameMap& gameMap) const
{
	 // Convert bounds to tile coordinates
	int startX = m_bounds.x / META_TILE::SIZE;
	int startY = m_bounds.y / META_TILE::SIZE;
	int endX = (m_bounds.x + m_bounds.w) / META_TILE::SIZE;
	int endY = (m_bounds.y + m_bounds.h) / META_TILE::SIZE;

	for (int x = startX; x <= endX; ++x) 
		for (int y = startY; y <= endY; ++y) 
			if (!gameMap.isCrossable({y,x})) 
				return true;

	return false;
}


Rect<int>& Hitbox::getBounds() { return m_bounds; }