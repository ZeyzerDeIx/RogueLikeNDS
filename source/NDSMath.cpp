#include "NDSMath.h"

int NDSMath::RoundAbsCeil(int x)
{
	return x >= 0 ?
		((x + (1 << FIXED_POINT_SHIFT) - 1) >> FIXED_POINT_SHIFT) :
		-(((-x) + (1 << FIXED_POINT_SHIFT) - 1) >> FIXED_POINT_SHIFT);
}

Vector2i NDSMath::RoundAbsCeil(Vector2i vec)
{
	return {RoundAbsCeil(vec.x), RoundAbsCeil(vec.y)};
}

int NDSMath::ToFixedPointInt(int x)
{
	return x << FIXED_POINT_SHIFT;
}

int NDSMath::ToNormalInt(int x)
{
	return x >> FIXED_POINT_SHIFT;
}
