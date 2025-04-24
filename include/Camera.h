#pragma once

#include "pch.h"
#include "Entity.h"

class Camera
{
public:
	Camera(GameContext& context);
	~Camera();

	void update();

	const Vector2i getDisplayPos(const Entity& entity) const;

	const Vector2i getMetaTileOffset() const;

private:
	Vector2i m_offset;
	GameContext& m_context;
};