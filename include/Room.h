#pragma once

#include "pch.h"

struct Room
{
	Vector2i coordinate;
	Vector2i size;
	Room oversized(int oversize) const noexcept
	{
		return {coordinate - oversize, size + oversize*2};
	}
};