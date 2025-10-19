#include "Camera.h"
#include "GameContext.h"

using namespace std;

void Camera::update(float dt)
{
	m_offset = static_cast<Vector2i>(GameContext::get().player->getPosition())
	- Vector2i{BG::SIZE_W/2,BG::SIZE_H/2};
	bgSetScroll(BG::ID, 
		BG::SIZE_W/2 + m_offset.x % META_TILE::SIZE,
		BG::SIZE_H/2 + m_offset.y % META_TILE::SIZE);
}

const Vector2i Camera::getDisplayPos(const Entity& entity) const
{
	if(&entity == GameContext::get().player) return {SCREEN_SIZE_W/2,SCREEN_SIZE_H/2};

	return {0,0};
}

const Vector2i Camera::getMetaTileOffset() const
{
	return m_offset / META_TILE::SIZE;
}


Camera::Camera(string name): GameObject(name), m_offset{0,0} {}