#pragma once

#include "pch.h"
#include "Sprite.h"
#include "Hitbox.h"
#include "GameObject.h"

class Camera;

class Entity: public GameObject
{
public:
	void Move(Vector2fp delta);
	void Update(FixedPoint dt) override;
	void Display();

	void SetDirection(u8 direction, bool enable);
	void SetAllDirections(u8 directions);
	void SetSprite(Sprite* sprite);
	void SetSize(Vector2i size);

	[[nodiscard]] bool GetDirection(u8 direction) const;
	[[nodiscard]] const Vector2fp& GetPosition() const;
	[[nodiscard]] Vector2i GetPixelPosition() const;
	[[nodiscard]] Vector2i GetCoordinates() const;
	[[nodiscard]] const Vector2i& GetSize();
	[[nodiscard]] FixedPoint GetSpeed() const;
	[[nodiscard]] const Hitbox& GetHitbox();
	[[nodiscard]] bool IsMoving() const;
	
private:
	Sprite* m_Sprite;
	// Center of the entity
	Vector2fp m_Position;
	Vector2i m_Size;
	u8 m_Directions;
	Hitbox m_Hitbox;
	FixedPoint m_Speed;
	short m_SfxPlayInterval; //in frames
	short m_SfxElapsedFrames; //in frames

	void UpdateHitboxPos();
	void UpdateSpriteDirection();
	void UpdateAudio();

	Entity(std::string name);

	friend class GameObject;
};