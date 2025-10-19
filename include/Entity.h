#pragma once

#include "pch.h"
#include "Sprite.h"
#include "Hitbox.h"

class Camera;
class Debug;

class Entity
{
public:
	Entity(Sprite* sprite, Vector2i size);
	~Entity();

	void move(Vector2f delta);
	void update(float dt);
	void display();

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
	u8 m_directions;
	Hitbox m_hitbox;
	float m_speed;
	short m_sfxPlayInterval; //in frames
	short m_sfxElapsedFrames; //in frames

	void updateHitboxPos();
	void updateSpriteDirection();
	void updateAudio();

	friend class Debug;
};