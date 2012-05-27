/*
	bitmap.cpp - the bitmap function

	Copyright (c) 2002, Blaine Myers
*/
#include "bitmapex.h"

#define SAFE_FREE(p)         { if(p) { free(p); (p)=NULL; } }

/* This function loads a bitmap from a given offset within a file. */
HBITMAP LoadBitmapOffsetA(char szFilename[MAX_PATH], int nOffset){
	HBITMAP hBitmap=NULL;
	HANDLE hFile;
	BITMAPFILEHEADER bmfh;
	BITMAPINFO *pbmi;
	BYTE *pBits;
	DWORD dwBytesRead;
	BOOL bSuccess;
	DWORD dwInfoSize;

	hFile=CreateFileA(szFilename, GENERIC_READ, FILE_SHARE_READ, NULL,
							OPEN_EXISTING, 0, NULL);

	if(hFile==INVALID_HANDLE_VALUE)return NULL;

	SetFilePointer(hFile, nOffset, NULL, FILE_BEGIN);
	bSuccess=ReadFile(hFile, &bmfh, sizeof(BITMAPFILEHEADER), &dwBytesRead, NULL);

	if(!bSuccess || (dwBytesRead != sizeof(BITMAPFILEHEADER))
					|| (bmfh.bfType != * (WORD *) "BM"))
	{
		CloseHandle(hFile);
		return NULL;
	}

	dwInfoSize=bmfh.bfOffBits - sizeof(BITMAPFILEHEADER);
	pbmi = calloc(dwInfoSize, sizeof(BITMAPINFO));
	bSuccess=ReadFile(hFile, pbmi, dwInfoSize, &dwBytesRead, NULL);
	if(!bSuccess ||(dwBytesRead != dwInfoSize))
	{
		SAFE_FREE(pbmi);
		CloseHandle(hFile);
		return NULL;
	}

	hBitmap=CreateDIBSection(NULL, pbmi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);
	if(hBitmap==NULL)
	{
		SAFE_FREE(pbmi);
		CloseHandle(hFile);
		return NULL;
	}

	ReadFile(hFile, pBits, bmfh.bfSize-bmfh.bfOffBits, &dwBytesRead, NULL);

	SAFE_FREE(pbmi);
	CloseHandle(hFile);

	return hBitmap;
}

//This function loads a bitmap from a given offset within a file
HBITMAP LoadBitmapOffsetW(WCHAR szFilename[MAX_PATH], int nOffset){
	HBITMAP hBitmap=NULL;
	HANDLE hFile;
	BITMAPFILEHEADER bmfh;
	BITMAPINFO *pbmi;
	BYTE *pBits;
	DWORD dwBytesRead;
	BOOL bSuccess;
	DWORD dwInfoSize;

	hFile=CreateFileW(szFilename, GENERIC_READ, FILE_SHARE_READ, NULL,
							OPEN_EXISTING, 0, NULL);

	if(hFile==INVALID_HANDLE_VALUE)return NULL;

	SetFilePointer(hFile, nOffset, NULL, FILE_BEGIN);
	bSuccess=ReadFile(hFile, &bmfh, sizeof(BITMAPFILEHEADER), &dwBytesRead, NULL);

	if(!bSuccess || (dwBytesRead != sizeof(BITMAPFILEHEADER))
					|| (bmfh.bfType != * (WORD *) "BM"))
	{
		CloseHandle(hFile);
		return NULL;
	}

	dwInfoSize=bmfh.bfOffBits - sizeof(BITMAPFILEHEADER);
	pbmi = calloc(dwInfoSize, sizeof(BITMAPINFO));
	bSuccess=ReadFile(hFile, pbmi, dwInfoSize, &dwBytesRead, NULL);
	if(!bSuccess ||(dwBytesRead != dwInfoSize))
	{
		SAFE_FREE(pbmi);
		CloseHandle(hFile);
		return NULL;
	}

	hBitmap=CreateDIBSection(NULL, pbmi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);
	if(hBitmap==NULL)
	{
		SAFE_FREE(pbmi);
		CloseHandle(hFile);
		return NULL;
	}

	ReadFile(hFile, pBits, bmfh.bfSize-bmfh.bfOffBits, &dwBytesRead, NULL);

	SAFE_FREE(pbmi);
	CloseHandle(hFile);

	return hBitmap;
}


//TransparentBlt2 transparently blt
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
	DWORD crTransparent)
{	
	HBITMAP hTileBitmap;
	HDC hdcTileBitmap;

	HBITMAP bmAndBack, bmAndObject, bmAndMem;
	HDC hdcMem, hdcBack, hdcObject, hdcTemp;
	COLORREF cColor;

	//Create an hdc and put the chosen bitmap in it
	hdcTileBitmap=CreateCompatibleDC(hdcDest);
	hTileBitmap=CreateCompatibleBitmap(hdcDest, nWidthDest, nHeightDest);
	SelectObject(hdcTileBitmap, hTileBitmap);

	StretchBlt(
		hdcTileBitmap, 
		0, 
		0, 
		nWidthDest, 
		nHeightDest, 
		hdcSrc, 
		nXOriginSrc,
		nYOriginSrc, 
		nWidthSrc, 
		nHeightSrc,
		SRCCOPY);
		
	//I'm not exactly sure how the following code works, I genereated
	//it by modifying some code found int eh MS knowledge base

	//Create compatible DC's
	hdcMem=CreateCompatibleDC(hdcDest);
	hdcBack=CreateCompatibleDC(hdcDest);
	hdcObject=CreateCompatibleDC(hdcDest);
	hdcTemp=CreateCompatibleDC(hdcDest);


	//Monochrome bitmaps
	bmAndBack=CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);
	bmAndObject=CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);
		
	//Bitmaps
	bmAndMem=CreateCompatibleBitmap(hdcDest, nWidthDest, nHeightDest);

	//Select bitmaps int DC's.
	SelectObject(hdcBack, bmAndBack);
	SelectObject(hdcObject, bmAndObject);
	SelectObject(hdcMem, bmAndMem);

	//Set map mode
	SetMapMode(hdcMem, GetMapMode(hdcDest));
		
	//Set bacground color to desired value and save old value
	cColor=SetBkColor(hdcTileBitmap, crTransparent);

	//BitBlt monocrhrom version of image, transparent as white
	BitBlt(hdcObject, 0, 0, nWidthDest, nHeightDest, hdcTileBitmap, 0, 0, SRCCOPY);

	//Restore background color
	SetBkColor(hdcTileBitmap, cColor);

	//Create inverse of monochrome image
	BitBlt(hdcBack, 0, 0, nWidthDest, nHeightDest, hdcObject, 0, 0, NOTSRCCOPY);

	//Get copy of dest image
	BitBlt(hdcMem, 0, 0, nWidthDest, nHeightDest, hdcDest, nXOriginDest, nYOriginDest, SRCCOPY);

	//Blt bw version to dest
	BitBlt(hdcMem, 0, 0, nWidthDest, nHeightDest, hdcObject, 0, 0, SRCAND);

	//blt inverted src to image
	BitBlt(hdcTileBitmap, 0, 0, nWidthDest, nHeightDest, hdcBack, 0, 0, SRCAND);

	//Blt main image to dest
	BitBlt(hdcMem, 0, 0, nWidthDest, nHeightDest, hdcTileBitmap, 0, 0, SRCPAINT);
		
	//Blt to screen
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hdcMem, 0, 0, SRCCOPY);
		
	//Delete unused variables
	DeleteDC(hdcTileBitmap);
	DeleteDC(hdcMem);
	DeleteDC(hdcBack);
	DeleteDC(hdcObject);
	DeleteDC(hdcTemp);
		
	DeleteObject(bmAndBack);
	DeleteObject(bmAndObject);
	DeleteObject(bmAndMem);
	DeleteObject(hTileBitmap);

	return TRUE;
}