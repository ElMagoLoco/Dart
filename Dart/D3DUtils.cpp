#include "D3DUtils.h"

//some utility functions
float getRandomFloat(float _low, float _high)
{
	return _low + ((float)((float)rand() / (float)RAND_MAX) * (float)((_high)-(_low)));
}

