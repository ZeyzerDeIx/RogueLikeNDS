#pragma once

#include "pch.h"

class GameMap;

class Hitbox
{
public:
	Hitbox(Rect<int> bounds = {0,0,0,0});
	~Hitbox();

	const Rect<int>& getBounds() const;
	void setPos(int x, int y);
	void setX(int x);
	void setY(int y);
	void setWidth(int width);
	void setHeight(int height);
	bool intersects(const Hitbox& other) const;
	bool intersects(const GameMap& gameMap) const;
private:
	Rect<int> m_bounds;
};