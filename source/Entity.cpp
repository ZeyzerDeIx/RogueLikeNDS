#include "Entity.h"
#include "Camera.h"
#include "AudioManager.h"
#include "GameContext.h"

using namespace std;

void Entity::Move(Vector2f delta)
{
	// X axe
	if (delta.x != 0)
	{
		Hitbox futureHitbox = m_Hitbox;
		futureHitbox.SetX(futureHitbox.GetBounds().x + NDSMath::RoundAbsCeil(delta.x));

		if (!futureHitbox.Intersects(*GameContext::get().m_GameMap))
			m_Position.x += delta.x;
	}

	// Y axe
	if (delta.y != 0)
	{
		Hitbox futureHitbox = m_Hitbox;
		futureHitbox.SetY(futureHitbox.GetBounds().y + NDSMath::RoundAbsCeil(delta.y));

		if (!futureHitbox.Intersects(*GameContext::get().m_GameMap))
			m_Position.y += delta.y;
	}
	
	UpdateHitboxPos();

	UpdateAudio();
}

void Entity::Update(float delta)
{
	using namespace DIRECTION;
	Vector2f deltaPos = m_Speed * delta * Vector2f
	{
		static_cast<float>(GetDirection(RIGHT) - GetDirection(LEFT)),
		static_cast<float>(GetDirection(BOT)   - GetDirection(TOP))
	};

	Move(deltaPos);

	UpdateSpriteDirection();
	m_Sprite->Update(deltaPos.y != 0 ? 2.f : 1.f);
}

void Entity::Display()
{
	m_Sprite->Display(GameContext::get().m_Camera->GetDisplayPos(*this) - m_Size/2);
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



bool Entity::GetDirection(u8 direction)
{
	return m_Directions & direction;
}

const Vector2f& Entity::GetPosition()
{
	return m_Position;
}

const Vector2i Entity::GetCoordinates()
{
	return static_cast<Vector2i>(m_Position/META_TILE::SIZE) - Vector2i{m_Position.x < 0, m_Position.y < 0};
}

const Vector2i& Entity::GetSize()
{
	return m_Size;
}

float Entity::GetSpeed() const
{
	return m_Speed;
}

const Hitbox& Entity::GetHitbox()
{
	return m_Hitbox;
}

bool Entity::IsMoving()
{
	return m_Directions != DIRECTION::NONE;
}


void Entity::UpdateHitboxPos()
{
	m_Hitbox.SetPos(m_Position.x - m_Size.x/2,m_Position.y);
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
	GameContext::get().m_AudioManager->PlayRandomFootstep();
}




Entity::Entity(string name):
	GameObject(name),
	m_Sprite{nullptr},
	m_Position{META_TILE::SIZE/2,META_TILE::SIZE/3},
	m_Size{20,20}, //default size
	m_Directions{DIRECTION::NONE},
	m_Hitbox{{0,0,0,0}},
	m_Speed(50.f),
	m_SfxPlayInterval(36),
	m_SfxElapsedFrames(0)
{}