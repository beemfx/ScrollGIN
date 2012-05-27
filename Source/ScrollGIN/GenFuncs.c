/* GenFuncs.c generic functions.
	Copyright (c) 2004 Blaine Myers. */

#include <stdlib.h>

void RandomSeed(unsigned int nSeed)
{
	srand(nSeed);
}

int Random(int nLow, int nHigh)
{
	return rand() % (nHigh - nLow + 1) + nLow;
}