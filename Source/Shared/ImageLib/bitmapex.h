/*
	bitmap.h - header for teh bitmap function

	Copyright (c) 2002, Blaine Myers
*/
#ifndef __BITMAP_H__
#define __BITMAP_H__
#include <windows.h>

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

//The LoadBitmapOffset function loads a bitmap from and offset within a file.
HBITMAP LoadBitmapOffset(const char szFilename[MAX_PATH], int nOffset);

BOOL TransparentBlt2(
	HDC hdcDest,
	int nXOriginDest,
	int nYOriginDest,
	int nWidthDest,
	int nHeightDest,
	HDC hdcSrc,
	int nXOriginSrc,
	int nYOriginSrc,
	int nWidthSrc,
	int nHeightSrc,
	sg_uint32 crTransparent);

#ifdef __cplusplus
} //extern "C"
#endif //__cplusplus

#endif //__BITMAP_H__