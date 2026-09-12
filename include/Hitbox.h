#pragma once

#include "pch.h"

class GameMap;

class Hitbox
{
public:
	Hitbox(Rect<int> bounds = {0,0,0,0});
	~Hitbox() = default;

	[[nodiscard]] const Rect<int>& GetBounds() const;
	void SetPos(int x, int y);
	void SetX(int x);
	void SetY(int y);
	void SetWidth(int width);
	void SetHeight(int height);
	[[nodiscard]] bool Intersects(const Hitbox& other) const;
	[[nodiscard]] bool Intersects(const GameMap& gameMap) const;
private:
	Rect<int> m_bounds;
};