/*
	error.cpp - functions for setting errors

	Copyright (c) 2002, Blaine Myers
*/
#include "error.h"
#include <Windows.h>

void SetError(const char* szString){
	OutputDebugString(szString);
	OutputDebugString(TEXT("\n"));
}