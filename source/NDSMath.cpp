#include "NDSMath.h"

int NDSMath::ToFixedPointInt(int x)
{
	return x << FIXED_POINT_SHIFT;
}

int NDSMath::ToNormalInt(int x)
{
	return x >> FIXED_POINT_SHIFT;
}
