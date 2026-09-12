#include "Camera.h"
#include "GameContext.h"

using namespace std;

void Camera::Update(float dt)
{
	m_Offset = static_cast<Vector2i>(GameContext::Get().m_Player->GetPosition())
	- Vector2i{BG::SIZE_W/2,BG::SIZE_H/2};
	bgSetScroll(BG::ID, 
		BG::SIZE_W/2 + m_Offset.x % META_TILE::SIZE,
		BG::SIZE_H/2 + m_Offset.y % META_TILE::SIZE);
}

const Vector2i Camera::GetDisplayPos(const Entity& entity) const
{
	if(&entity == GameContext::Get().m_Player) return {SCREEN_SIZE_W/2,SCREEN_SIZE_H/2};

	return {0,0};
}

const Vector2i Camera::GetMetaTileOffset() const
{
	return m_Offset / META_TILE::SIZE;
}


Camera::Camera(string name): GameObject(name), m_Offset{0,0} {}