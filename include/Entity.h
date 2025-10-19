#pragma once

#include "pch.h"
#include "Sprite.h"
#include "Hitbox.h"
#include "GameObject.h"

class Camera;
class Debug;

class Entity: public GameObject
{
public:
	void move(Vector2f delta);
	virtual void update(float dt);
	void display();

	void setDirection(u8 direction, bool enable);
	void setAllDirections(u8 directions);
	void setSprite(Sprite* sprite);
	void setSize(Vector2i size);

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

	Entity(std::string name);

	friend class Debug;
	friend class GameObject;
};