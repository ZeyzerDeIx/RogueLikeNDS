#include "NDSMath.h"

int NDSMath::RoundAbsCeil(float x)
{
	return x>=0 ?
		static_cast<int>(std::ceil(x)):
		- static_cast<int>(std::ceil(std::abs(x)));
}

Vector2i NDSMath::RoundAbsCeil(Vector2f vec)
{
	return {RoundAbsCeil(vec.x), RoundAbsCeil(vec.y)};
}