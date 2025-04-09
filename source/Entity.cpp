#include "Entity.h"
#include "Camera.h"


Entity::Entity(Sprite* sprite, Vector2i size, GameContext& context):
	m_sprite{sprite},
	m_position{META_TILE::SIZE/2,META_TILE::SIZE/3},
	m_size{size},
	m_context(context),
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
	// X axe
	if (delta.x != 0)
	{
		Hitbox futureHitbox = m_hitbox;
		futureHitbox.getBounds().x += NDSMath::roundAbsCeil(delta.x);

		if (!futureHitbox.intersects(*m_context.gameMap))
			m_position.x += delta.x;
	}

	// Y axe
	if (delta.y != 0)
	{
		Hitbox futureHitbox = m_hitbox;
		futureHitbox.getBounds().y += NDSMath::roundAbsCeil(delta.y);

		if (!futureHitbox.intersects(*m_context.gameMap))
			m_position.y += delta.y;
	}
	
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

void Entity::display(const Camera& camera)
{
	m_sprite->display(camera.getDisplayPos(*this) - m_size/2);
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

const Vector2f& Entity::getPosition()
{
	return m_position;
}

const Vector2i Entity::getCoordinates()
{
	return static_cast<Vector2i>(m_position/META_TILE::SIZE) - Vector2i{m_position.x < 0, m_position.y < 0};
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