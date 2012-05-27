/*
	error.cpp - functions for setting errors

	Copyright (c) 2002, Blaine Myers
*/
#include "error.h"

void SetError(LPCTSTR szString){
	OutputDebugString(szString);
	OutputDebugString(TEXT("\n"));
}