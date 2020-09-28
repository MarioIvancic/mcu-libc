#include <math.h>
#include "features.h"

float remainderf(float x, float y)
{
	int q;
	return remquof(x, y, &q);
}

weak_alias(remainderf, dremf);
