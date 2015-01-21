/*
	bitmap.cpp - the bitmap function

	Copyright (c) 2002, Blaine Myers
*/
#include "bitmapex.h"
#include "img_lib/img_lib.h"

#define SAFE_FREE(p)         { if(p) { free(p); (p)=NULL; } }

/* This function loads a bitmap from a given offset within a file. */
HBITMAP LoadBitmapOffset(const char szFilename[MAX_PATH], int nOffset)
{
	#if 0
	HBITMAP hBitmap=NULL;
	HANDLE hFile;
	BITMAPFILEHEADER bmfh;
	BITMAPINFO *pbmi;
	BYTE *pBits;
	DWORD dwBytesRead;
	BOOL bSuccess;
	DWORD dwInfoSize;
	#endif
	HANDLE hFile=CreateFileA(szFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	DWORD FileSize = GetFileSize( hFile , NULL );

	if(hFile==INVALID_HANDLE_VALUE)return NULL;
	unsigned __int8* FileData = malloc( FileSize );
	if( NULL == FileData )
	{
		CloseHandle( hFile );
		return NULL;
	}
	SetFilePointer( hFile , 0, NULL, FILE_BEGIN );
	DWORD SizeRead = 0;
	BOOL ReadSucc = ReadFile( hFile , FileData , FileSize, &SizeRead , NULL );

	if( !ReadSucc || SizeRead != FileSize )
	{
		SAFE_FREE( FileData );
		CloseHandle( hFile );
		return NULL;
	}

	HIMG Img = IMG_OpenMemory( &FileData[nOffset] , FileSize-nOffset );
	SAFE_FREE( FileData );
	CloseHandle( hFile );

	if( IMG_NULL == Img )
	{
		return NULL;
	}

	IMG_DESC ImgDesc;
	IMG_GetDesc( Img , &ImgDesc );

	BITMAPINFO BmInfo;
	memset( &BmInfo, 0, sizeof(BmInfo) );
	BmInfo.bmiHeader.biCompression   = BI_RGB;
	BmInfo.bmiHeader.biPlanes   = 1;
	BmInfo.bmiHeader.biBitCount = 32;
	BmInfo.bmiHeader.biHeight   = ImgDesc.Height;
	BmInfo.bmiHeader.biWidth    = ImgDesc.Width;
	BmInfo.bmiHeader.biSizeImage=0;
	BmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	unsigned __int32* Bits = NULL;

	HBITMAP hBitmap = CreateDIBSection( NULL , &BmInfo , DIB_RGB_COLORS , &Bits , NULL , 0 );
	
	if (NULL == hBitmap )
	{
		IMG_Delete(Img);
		return NULL;
	}

	IMG_DEST_RECT DestRect;
	DestRect.nFormat = IMGFMT_A8R8G8B8;
	DestRect.nWidth  = ImgDesc.Width;
	DestRect.nHeight = ImgDesc.Height;
	DestRect.nPitch  = sizeof( unsigned __int32)*ImgDesc.Width;
	DestRect.nOrient = IMGORIENT_BOTTOMLEFT;
	DestRect.pImage  = Bits;

	DestRect.rcCopy.top    = 0;
	DestRect.rcCopy.left   = 0;
	DestRect.rcCopy.bottom = ImgDesc.Height;
	DestRect.rcCopy.right  = ImgDesc.Width;

	img_bool Copied = IMG_CopyBits( Img , &DestRect , IMGFILTER_NONE , IMG_NULL , 0 );

	IMG_Delete(Img);
	Img = IMG_NULL;

	if( IMG_FALSE == Copied )
	{
		DeleteObject( hBitmap );
		hBitmap = NULL;
	}

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