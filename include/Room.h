#pragma once

#include "pch.h"

struct Room
{
	Vector2i Coordinate;
	Vector2i Size;
	[[nodiscard]] Room Oversized(int oversize) const noexcept
	{
		return {Coordinate - oversize, Size + oversize*2};
	}
};