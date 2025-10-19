#pragma once

#include "pch.h"
#include "Entity.h"
#include "GameObject.h"

class Camera: public GameObject
{
public:
	friend class GameObject;

	virtual void update(float dt);

	const Vector2i getDisplayPos(const Entity& entity) const;

	const Vector2i getMetaTileOffset() const;

private:
	Vector2i m_offset;

	Camera(std::string name);
};