#pragma once

#include "PCH.h"
#include "Entity.h"

class Camera
{
public:
	Camera(Entity& player);
	~Camera();

	void update();

	const Vector2i getMetaTileOffset() const;

private:
	Vector2i m_offset;
	Entity& m_player;
};