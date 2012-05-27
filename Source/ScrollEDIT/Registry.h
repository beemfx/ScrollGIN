/*
	registry.h - header for registry.cpp

	Copyright (c) 2002, Blaine Myers
*/
#ifndef __REGISTRY_H__
#define __REGISTRY_H__
#include <windows.h>
#include <tchar.h>

BOOL GetRegFilename(TCHAR szFilename[MAX_PATH]);
BOOL SaveRegFilename(TCHAR szFilename[MAX_PATH]);

#endif //__registry_h__