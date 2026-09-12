#pragma once

#include "pch.h"
#include "Entity.h"
#include "GameObject.h"

class Camera: public GameObject
{
public:
	void Update(float dt) override;

	[[nodiscard]] static Vector2i GetDisplayPos(const Entity& entity) ;
	[[nodiscard]] Vector2i GetMetaTileOffset() const;

private:
	Vector2i m_Offset;

	Camera(std::string name);

	friend class GameObject;
};