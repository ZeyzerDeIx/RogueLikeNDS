#include "Camera.h"

Camera::Camera(GameContext& context):
	m_offset{0,0},
	m_context(context)
{}

void Camera::update()
{
	m_offset = static_cast<Vector2i>(m_context.player->getPosition())
	- Vector2i{BG::SIZE_W/2,BG::SIZE_H/2};
	bgSetScroll(BG::ID, 
		BG::SIZE_W/2 + m_offset.x % META_TILE::SIZE,
		BG::SIZE_H/2 + m_offset.y % META_TILE::SIZE);
}

const Vector2i Camera::getDisplayPos(const Entity& entity) const
{
	if(&entity == m_context.player) return {SCREEN_SIZE_W/2,SCREEN_SIZE_H/2};

	return {0,0};
}

const Vector2i Camera::getMetaTileOffset() const
{
	return m_offset / META_TILE::SIZE;
}