#include "D3DUtils.h"

//some utility functions
float getRandomFloat(float _low, float _high)
{
	return _low + ((float)((float)rand() / (float)RAND_MAX) * (float)((_high)-(_low)));
}

void CopyString(const char* input, char** output)
{
	if (input)
	{
		UINT length = (UINT)::strlen(input) + 1; // add 1 for terminating null character.
		*output = new char[length];
		::strcpy(*output, input);
	}
	else
	{
		*output = 0;
	}
}