#pragma once

#include "PCH.h"

class Hitbox
{
public:
	Hitbox(Rect<int> bounds = {0,0,0,0});
	~Hitbox();

	Rect<int>& getBounds();
	bool intersects(const Hitbox& other);
private:
	Rect<int> m_bounds;
};