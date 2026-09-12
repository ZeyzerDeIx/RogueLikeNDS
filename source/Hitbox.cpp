#include "Hitbox.h"
#include "GameMap.h"

Hitbox::Hitbox(Rect<int> bounds): m_bounds(bounds) {}

bool Hitbox::Intersects(const Hitbox& other) const
{
	// Check if there is no intersection between the two rectangles
			 // This is completely to the left of other
	return !(m_bounds.x + m_bounds.w <= other.m_bounds.x ||
			 // Other is completely to the left of this
			 other.m_bounds.x + other.m_bounds.w <= m_bounds.x ||
			 // This is completely above other
			 m_bounds.y + m_bounds.h <= other.m_bounds.y ||
			 // Other is completely above this
			 other.m_bounds.y + other.m_bounds.h <= m_bounds.y);
}

bool Hitbox::Intersects(const GameMap& gameMap) const
{
	// Convert bounds to tile coordinates
	int startX = m_bounds.x / META_TILE::SIZE - static_cast<int>(m_bounds.x<0);
	int startY = m_bounds.y / META_TILE::SIZE - static_cast<int>(m_bounds.y<0);
	int boundsR = m_bounds.x + m_bounds.w;
	int boundsB = m_bounds.y + m_bounds.h;
	int endX = boundsR / META_TILE::SIZE - static_cast<int>(boundsR<0);
	int endY = boundsB / META_TILE::SIZE - static_cast<int>(boundsB<0);

	for (int x = startX; x <= endX; ++x) 
		for (int y = startY; y <= endY; ++y) 
			if (!gameMap.IsCrossable({x,y}))
				return true;

	return false;
}


const Rect<int>& Hitbox::GetBounds() const { return m_bounds; }

void Hitbox::SetPos(int x, int y)
{
	m_bounds.x = x;
	m_bounds.y = y;
}

void Hitbox::SetX(int x) { m_bounds.x = x; }
void Hitbox::SetY(int y) { m_bounds.y = y; }
void Hitbox::SetWidth(int width) { m_bounds.w = width; }
void Hitbox::SetHeight(int height) { m_bounds.h = height; }
