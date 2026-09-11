#pragma once

#include "pch.h"
#include "Sprite.h"
#include "Hitbox.h"
#include "GameObject.h"

class Camera;

class Entity: public GameObject
{
public:
	void Move(Vector2f delta);
	void Update(float dt) override;
	void Display();

	void SetDirection(u8 direction, bool enable);
	void SetAllDirections(u8 directions);
	void SetSprite(Sprite* sprite);
	void SetSize(Vector2i size);

	bool GetDirection(u8 direction);
	const Vector2f& GetPosition();
	const Vector2i GetCoordinates();
	const Vector2i& GetSize();
	[[nodiscard]] float GetSpeed() const;
	const Hitbox& GetHitbox();
	bool IsMoving();
	
private:
	Sprite* m_Sprite;
	// Center of the entity
	Vector2f m_Position;
	Vector2i m_Size;
	u8 m_Directions;
	Hitbox m_Hitbox;
	float m_Speed;
	short m_SfxPlayInterval; //in frames
	short m_SfxElapsedFrames; //in frames

	void UpdateHitboxPos();
	void UpdateSpriteDirection();
	void UpdateAudio();

	Entity(std::string name);

	friend class GameObject;
};