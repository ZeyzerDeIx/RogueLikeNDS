#pragma once

#include "pch.h"
#include "Entity.h"
#include "GameObject.h"

class Camera: public GameObject
{
public:
	void Update(float dt) override;

	[[nodiscard]] const Vector2i GetDisplayPos(const Entity& entity) const;
	[[nodiscard]] const Vector2i GetMetaTileOffset() const;

private:
	Vector2i m_Offset;

	Camera(std::string name);

	friend class GameObject;
};