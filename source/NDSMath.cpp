#include "NDSMath.h"

int NDSMath::roundAbsCeil(float x)
{
	return x>=0 ?
		static_cast<int>(std::ceil(x)):
		- static_cast<int>(std::ceil(std::abs(x)));
}

Vector2i NDSMath::roundAbsCeil(Vector2f vec)
{
	return {roundAbsCeil(vec.x), roundAbsCeil(vec.y)};
}