#include "Hitbox.h"

Hitbox::Hitbox(Rect<int> bounds):
	m_bounds(bounds)
{}

Hitbox::~Hitbox(){}

bool Hitbox::intersects(const Hitbox& other)
{
	// Check if there is no intersection between the two rectangles
	return !(m_bounds.x + m_bounds.w <= other.m_bounds.x ||       // This is completely to the left of other
			 other.m_bounds.x + other.m_bounds.w <= m_bounds.x || // Other is completely to the left of this
			 m_bounds.y + m_bounds.h <= other.m_bounds.y ||       // This is completely above other
			 other.m_bounds.y + other.m_bounds.h <= m_bounds.y);  // Other is completely above this
}


Rect<int>& Hitbox::getBounds() { return m_bounds; }