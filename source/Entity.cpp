#include "Entity.h"

#include <utility>
#include "Camera.h"
#include "AudioManager.h"
#include "GameContext.h"

using namespace std;

void Entity::Move(Vector2i delta)
{
	// X axe
	if (delta.x != 0)
	{
		Hitbox futureHitbox = m_Hitbox;
		futureHitbox.SetX(NDSMath::ToNormalInt(m_Position.x + delta.x)- m_Size.x/2);

		if (!futureHitbox.Intersects(*GameContext::Get().m_GameMap))
			m_Position.x += delta.x;
	}

	// Y axe
	if (delta.y != 0)
	{
		Hitbox futureHitbox = m_Hitbox;
		futureHitbox.SetY(NDSMath::ToNormalInt(m_Position.y + delta.y));

		if (!futureHitbox.Intersects(*GameContext::Get().m_GameMap))
			m_Position.y += delta.y;
	}
	
	UpdateHitboxPos();

	UpdateAudio();
}

void Entity::Update(int delta)
{
	using namespace DIRECTION;
	Vector2i deltaPos = NDSMath::FixedPointMult(m_Speed, delta) * Vector2i
	{
		GetDirection(RIGHT) - GetDirection(LEFT),
		GetDirection(BOT)   - GetDirection(TOP)
	};

	Move(deltaPos);

	UpdateSpriteDirection();
	m_Sprite->Update(NDSMath::ToFixedPointInt(deltaPos.y != 0 ? 2 : 1));
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

const Vector2i& Entity::GetPosition() const
{
	return m_Position;
}

Vector2i Entity::GetPixelPosition() const
{
	return
	{
		NDSMath::ToNormalInt(m_Position.x),
		NDSMath::ToNormalInt(m_Position.y)
	};
}

Vector2i Entity::GetCoordinates()
{
	return m_Position/META_TILE::SIZE - Vector2i{m_Position.x < 0, m_Position.y < 0};
}

const Vector2i& Entity::GetSize()
{
	return m_Size;
}

int Entity::GetSpeed() const
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
	m_Position{META_TILE::SIZE/2,META_TILE::SIZE/3},
	m_Size{20,20}, //default size
	m_Directions{DIRECTION::NONE},
	m_Hitbox{{0,0,0,0}},
	m_Speed(NDSMath::ToFixedPointInt(50)),
	m_SfxPlayInterval(36),
	m_SfxElapsedFrames(0)
{}