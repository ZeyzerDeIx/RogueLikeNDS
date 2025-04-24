#pragma once

#include "pch.h"
#include "Sprite.h"
#include "Hitbox.h"
#include "GameContext.h"

class Camera;
class Debug;

class Entity
{
public:
	Entity(Sprite* sprite, Vector2i size, GameContext& context);
	~Entity();

	void move(Vector2f delta);
	void update(float dt);
	void display(const Camera& camera);

	void setDirection(u8 direction, bool enable);
	void setAllDirections(u8 directions);

	bool getDirection(u8 direction);
	const Vector2f& getPosition();
	const Vector2i getCoordinates();
	bool isMoving();
	
private:
	Sprite* m_sprite;
	// Center of the entity
	Vector2f m_position;
	Vector2i m_size;
	GameContext& m_context;
	u8 m_directions;
	Hitbox m_hitbox;
	float m_speed;

	void updateHitboxPos();
	void updateSpriteDirection();

	friend class Debug;
};