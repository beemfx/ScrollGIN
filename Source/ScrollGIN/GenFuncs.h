/* GenFuncs.h - Header for generic functions.
	Copyright (c) 2004 Blaine Myers */

#ifndef __GENFUNCS_H__
#define __GENFUNCS_H__

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

int RandomSeed(unsigned int nSeed);
int Random(int nLow, int nHigh);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif __GENFUNCS_H__