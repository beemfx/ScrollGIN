/*
	bitmap.cpp - the bitmap function

	Copyright (c) 2002, Blaine Myers
*/

#include "bitmapex.h"
#include "SgImage.h"
#include "img_lib/img_lib.h"

/* This function loads a bitmap from a given offset within a file. */
HBITMAP LoadBitmapOffset(const char szFilename[MAX_PATH], int nOffset, int FileSize)
{
	SgImage Img(szFilename, nOffset, FileSize);
	if (!Img.IsLoaded())
	{
		return NULL;
	}

	BITMAPINFO BmInfo = { };
	BmInfo.bmiHeader.biCompression = BI_RGB;
	BmInfo.bmiHeader.biPlanes = 1;
	BmInfo.bmiHeader.biBitCount = sizeof(SgImage::sgPixel)*8;
	BmInfo.bmiHeader.biHeight = -static_cast<LONG>(Img.GetHeight());
	BmInfo.bmiHeader.biWidth = Img.GetWidth();
	BmInfo.bmiHeader.biSizeImage = 0;
	BmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	SgImage::sgPixel* Bits = nullptr;

	const std::size_t ImageSize = Img.GetWidth() * Img.GetHeight() * sizeof(SgImage::sgPixel);

	HBITMAP hBitmap = CreateDIBSection(NULL, &BmInfo, DIB_RGB_COLORS, reinterpret_cast<void**>(&Bits), NULL, 0);

	if (NULL == hBitmap)
	{
		return NULL;
	}

	std::memcpy(Bits, Img.GetPixels(), ImageSize);

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
	sg_uint32 crTransparent)
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