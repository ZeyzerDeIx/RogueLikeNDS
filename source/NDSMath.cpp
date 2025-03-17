#include "NDSMath.h"

int NDSMath::roundAbsCeil(float x)
{
	return x>=0 ?
		static_cast<int>(std::ceil(x)):
		- static_cast<int>(std::ceil(std::abs(x)));
}