#include "Camera.h"

Camera::Camera(Entity& player):
	m_offset{0,0},
	m_player(player)
{}

void Camera::update()
{
	m_offset = static_cast<Vector2i>(m_player.getPosition())
	- Vector2i{BG::SIZE_W/2,BG::SIZE_H/2};
	bgSetScroll(BG::ID, 
		BG::SIZE_W/2 + m_offset.x % META_TILE::SIZE,
		BG::SIZE_H/2 + m_offset.y % META_TILE::SIZE);
}

const Vector2i Camera::getMetaTileOffset() const
{
	return m_offset / META_TILE::SIZE;
}