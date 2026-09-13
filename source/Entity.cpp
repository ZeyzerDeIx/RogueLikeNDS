#include "Entity.h"

#include <utility>
#include "Camera.h"
#include "AudioManager.h"
#include "GameContext.h"

using namespace std;

void Entity::Move(Vector2fp delta)
{
	// X axe
	if (delta.x.m_Raw != 0)
	{
		Hitbox futureHitbox = m_Hitbox;
		futureHitbox.SetX((m_Position.x + delta.x).ToInt() - m_Size.x/2);

		if (!futureHitbox.Intersects(*GameContext::Get().m_GameMap))
			m_Position.x += delta.x;
	}

	// Y axe
	if (delta.y.m_Raw != 0)
	{
		Hitbox futureHitbox = m_Hitbox;
		futureHitbox.SetY((m_Position.y + delta.y).ToInt());

		if (!futureHitbox.Intersects(*GameContext::Get().m_GameMap))
			m_Position.y += delta.y;
	}
	
	UpdateHitboxPos();

	UpdateAudio();
}

void Entity::Update(FixedPoint dt)
{
	using namespace DIRECTION;
	Vector2i directions =
	{
		GetDirection(RIGHT) - GetDirection(LEFT),
		GetDirection(BOT)   - GetDirection(TOP)
	};
	Vector2fp deltaPos = (m_Speed * dt) * directions;

	Move(deltaPos);

	UpdateSpriteDirection();
	m_Sprite->Update(FixedPoint::FromInt(deltaPos.y.m_Raw != 0 ? 2 : 1));
}

void Entity::Display()
{
	m_Sprite->Display(Camera::GetDisplayPos(*this) - m_Size/2);
}



void Entity::SetDirection(u8 direction, bool enable)
{
	enable ? m_Directions |= direction : m_Directions &= ~direction;
}

void Entity::SetAllDirections(u8 directions)
{
	m_Directions = directions;
}

void Entity::SetSprite(Sprite* sprite)
{
	m_Sprite = sprite;
}

void Entity::SetSize(Vector2i size)
{
	m_Size = size;

	UpdateHitboxPos();
	m_Hitbox.SetWidth(m_Size.x);
	m_Hitbox.SetHeight(m_Size.y/2);
}



bool Entity::GetDirection(u8 direction) const
{
	return m_Directions & direction;
}

const Vector2fp& Entity::GetPosition() const
{
	return m_Position;
}

Vector2i Entity::GetPixelPosition() const
{
	return
	{
		m_Position.x.ToInt(),
		m_Position.y.ToInt()
	};
}

Vector2i Entity::GetCoordinates() const
{
	Vector2i pixPos = GetPixelPosition();
	return { pixPos.x >> META_TILE::SHIFT, pixPos.y >> META_TILE::SHIFT };
}

const Vector2i& Entity::GetSize()
{
	return m_Size;
}

FixedPoint Entity::GetSpeed() const
{
	return m_Speed;
}

const Hitbox& Entity::GetHitbox()
{
	return m_Hitbox;
}

bool Entity::IsMoving() const
{
	return m_Directions != DIRECTION::NONE;
}


void Entity::UpdateHitboxPos()
{
	Vector2i pixelPosition = GetPixelPosition();
	m_Hitbox.SetPos(pixelPosition.x - m_Size.x/2,pixelPosition.y);
}

void Entity::UpdateSpriteDirection()
{
	namespace Anim = ENTITY::ANIMATION;
	using AnimDir = Anim::DIRECTION;
	namespace Dir = DIRECTION;

	if (IsMoving())
		m_Sprite->SetState(
			GetDirection(Dir::TOP)  ? AnimDir::TOP_MOVING  :
			GetDirection(Dir::BOT)  ? AnimDir::BOT_MOVING  :
			GetDirection(Dir::LEFT) ? AnimDir::LEFT_MOVING :
									  AnimDir::RIGHT_MOVING);
	else if (Anim::IsMovingState(m_Sprite->GetState()))
		m_Sprite->SetState(m_Sprite->GetState() - Anim::MOVING_STATE_OFFSET);
}

void Entity::UpdateAudio()
{
	if(!IsMoving())
	{
		m_SfxElapsedFrames = m_SfxPlayInterval;
		return;
	}
	if(m_SfxElapsedFrames++ != m_SfxPlayInterval) return;

	m_SfxElapsedFrames = 0;
	GameContext::Get().m_AudioManager->PlayRandomFootstep();
}




Entity::Entity(string name):
	GameObject(std::move(name)),
	m_Sprite{nullptr},
	m_Position{FixedPoint(META_TILE::SIZE/2),FixedPoint(META_TILE::SIZE/3)},
	m_Size{20,20}, //default size
	m_Directions{DIRECTION::NONE},
	m_Hitbox{{0,0,0,0}},
	m_Speed(FixedPoint::FromInt(50)),
	m_SfxPlayInterval(36),
	m_SfxElapsedFrames(0)
{}