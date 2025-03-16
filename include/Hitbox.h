#pragma once

#include "PCH.h"

class GameMap;

class Hitbox
{
public:
	Hitbox(Rect<int> bounds = {0,0,0,0});
	~Hitbox();

	Rect<int>& getBounds();
	bool intersects(const Hitbox& other) const;
	bool intersects(const GameMap& gameMap) const;
private:
	Rect<int> m_bounds;
};