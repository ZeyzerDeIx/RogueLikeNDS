#include "Entity.h"


Entity::Entity(Sprite* sprite, Vector2i size):
	m_sprite{sprite},
	m_position{0,0},
	m_size{size},
	m_directions{DIRECTION::NONE},
	m_hitbox{{0,0,0,0}},
	m_speed(100.f)
{
	updateHitboxPos();

	m_hitbox.getBounds().w = m_size.x;
	m_hitbox.getBounds().h = m_size.y/2;
}


void Entity::move(Vector2f delta)
{
	m_position += delta;
	updateHitboxPos();
}

void Entity::update(float delta)
{
	using namespace DIRECTION;
	Vector2f deltaPos = m_speed * delta * Vector2f
	{
		static_cast<float>(getDirection(RIGHT) - getDirection(LEFT)),
		static_cast<float>(getDirection(BOT)   - getDirection(TOP))
	};

	move(deltaPos);

	updateSpriteDirection();
	m_sprite->update();
}

void Entity::display()
{
	m_sprite->display(static_cast<Vector2i>(m_position - static_cast<Vector2f>(m_size)/2.f));
}



void Entity::setDirection(u8 direction, bool enable)
{
	enable ? m_directions |= direction : m_directions &= ~direction;
}

void Entity::setAllDirections(u8 directions)
{
	m_directions = directions;
}

bool Entity::getDirection(u8 direction)
{
	return m_directions & direction;
}

bool Entity::isMoving()
{
	return m_directions != DIRECTION::NONE;
}


void Entity::updateHitboxPos()
{
	m_hitbox.getBounds().x = m_position.x - m_size.x/2;
	m_hitbox.getBounds().y = m_position.y;
}

void Entity::updateSpriteDirection()
{
	namespace Anim = ENTITY::ANIMATION;
	using AnimDir = Anim::DIRECTION;
	namespace Dir = DIRECTION;

	if (isMoving())
		m_sprite->setState(
			getDirection(Dir::TOP)  ? AnimDir::TOP_MOVING  :
			getDirection(Dir::BOT)  ? AnimDir::BOT_MOVING  :
			getDirection(Dir::LEFT) ? AnimDir::LEFT_MOVING :
									  AnimDir::RIGHT_MOVING);
	else if (Anim::isMovingState(m_sprite->getState()))
		m_sprite->setState(m_sprite->getState() - Anim::MOVING_STATE_OFFSET);
}