/*
	ImageLibrary.cpp - The CImageLibrary class

	Copyright (c) 2003, Blaine Myers
*/
#include "ImageLib.h"
#include "bitmapex.h"
#include <stdio.h>

CImageLibrary::CImageLibrary()
{
	m_nNumImages=0;
	m_nNumBitmaps=0;
	m_pImageData=NULL;

	for(int i=0; i<MAX_BITMAPS; i++)
	{
		m_szBitmapFilenameA[i][0]=0;
		m_hBitmap[i]=NULL;
	}
}

CImageLibrary::~CImageLibrary()
{
	SAFE_DELETE_ARRAY(m_pImageData);
	CloseMainBitmaps();
}

bool CImageLibrary::GetImageData(sg_uint32 nEntry, IMAGEDATA *imgData)
{
	if((nEntry<1) || (nEntry>m_nNumImages))return false;

	*imgData = m_pImageData[nEntry-1];

	return true;
}

sg_uint16 CImageLibrary::GetNumFrames(sg_uint32 nEntry)
{
	//return 0 if image doesn't exist
	if((nEntry<1) || (nEntry>m_nNumImages))return 0;

	return m_pImageData[nEntry-1].nFrames;
}

bool CImageLibrary::GetImageName(char* Out, size_t OutSize , sg_uint32 nEntry )
{
	char szTemp[IMAGE_NAME_LENGTH];

	bool result = FALSE != WideCharToMultiByte(
		CP_ACP, 
		0, 
		m_pImageData[nEntry-1].szImgLabel, 
		-1,
		szTemp,
		IMAGE_NAME_LENGTH,
		NULL,
		NULL);

	strcpy_s( Out, OutSize , szTemp);
	return result;
}

HBITMAP CImageLibrary::GetBitmap(sg_uint16 nBitmap)
{
	if((nBitmap<1) || (nBitmap>MAX_BITMAPS))return NULL;

	return m_hBitmap[nBitmap-1];
}

sg_uint32 CImageLibrary::GetNumEntries()
{
	return m_nNumImages;
}


bool CImageLibrary::OpenBitmap(LPSTR szFilename, sg_uint16 nBitmap)
{
	return OpenBitmapOffset(szFilename, 0, nBitmap);
}

bool CImageLibrary::OpenBitmapOffset(LPCSTR szFilename, sg_uint32 nOffset, sg_uint16 nBitmap)
{
	if( (nBitmap<1) || (nBitmap>MAX_BITMAPS) )return false;

	DeleteObject(m_hBitmap[nBitmap-1]);
	m_hBitmap[nBitmap-1]=LoadBitmapOffset(szFilename, nOffset);

	if(m_hBitmap[nBitmap-1] == NULL)return false;
	strcpy_s(m_szBitmapFilenameA[nBitmap-1], countof(m_szBitmapFilenameA[nBitmap-1]), szFilename);
	return true;
}

void CImageLibrary::CloseMainBitmaps()
{
	for(int i=0; i<m_nNumBitmaps; i++)
	{
		DeleteObject(m_hBitmap[i]);
		m_hBitmap[i] = NULL;
	}
	m_nNumBitmaps=0;
}


void CImageLibrary::ClearDataBase()
{
	for(sg_uint32 i=0;i<m_nNumImages;i++)
	{
		ZeroMemory(&m_pImageData[i], sizeof(IMAGEDATA));
	}
	m_nNumImages=0;
	m_nNumBitmaps=0;
	CloseMainBitmaps();
	SAFE_DELETE_ARRAY(m_pImageData);
	m_pImageData=NULL;
}


void CImageLibrary::CopyImageToDC(HDC hdcDest, sg_uint32 nEntry, int x, int y, BOOL bTransp)
{
	HDC hdcMainBitmap=NULL;

	if( (nEntry<1) || (nEntry>m_nNumImages) )return;

	hdcMainBitmap=CreateCompatibleDC(hdcMainBitmap);
	SelectObject(hdcMainBitmap, m_hBitmap[m_pImageData[nEntry-1].nBitmap-1]);


	if(!bTransp){
		StretchBlt(
			hdcDest, 
			x, 
			y, 
			m_pImageData[nEntry-1].nWidth, 
			m_pImageData[nEntry-1].nHeight, 
			hdcMainBitmap, 
			m_pImageData[nEntry-1].nX,
			m_pImageData[nEntry-1].nY, 
			m_pImageData[nEntry-1].nWidthSrc, 
			m_pImageData[nEntry-1].nHeightSrc, 
			SRCCOPY);
	}else{
		TransparentBlt2(
			hdcDest,
			x,
			y,
			m_pImageData[nEntry-1].nWidth,
			m_pImageData[nEntry-1].nHeight,
			hdcMainBitmap,
			m_pImageData[nEntry-1].nX,
			m_pImageData[nEntry-1].nY, 
			m_pImageData[nEntry-1].nWidthSrc, 
			m_pImageData[nEntry-1].nHeightSrc,
			RGB(255,0,255));

	}

	DeleteDC(hdcMainBitmap);
}

sg_uint16 CImageLibrary::GetNumBitmaps()
{
	return m_nNumBitmaps;
}

void CImageLibrary::StretchImageToDC(HDC hdcDest, sg_uint32 nEntry, int x, int y, int nWidth, int nHeight, BOOL bTransp)
{
	HDC hdcMainBitmap=NULL;

	if( (nEntry<1) || (nEntry>m_nNumImages) )return;

	hdcMainBitmap=CreateCompatibleDC(hdcMainBitmap);
	SelectObject(hdcMainBitmap, m_hBitmap[m_pImageData[nEntry-1].nBitmap-1]);


	if(!bTransp){
		StretchBlt(
			hdcDest, 
			x, 
			y, 
			nWidth, 
			nHeight, 
			hdcMainBitmap, 
			m_pImageData[nEntry-1].nX,
			m_pImageData[nEntry-1].nY, 
			m_pImageData[nEntry-1].nWidthSrc, 
			m_pImageData[nEntry-1].nHeightSrc, 
			SRCCOPY);
	}else{
		TransparentBlt2(
			hdcDest,
			x,
			y,
			nWidth,
			nHeight,
			hdcMainBitmap,
			m_pImageData[nEntry-1].nX,
			m_pImageData[nEntry-1].nY, 
			m_pImageData[nEntry-1].nWidthSrc, 
			m_pImageData[nEntry-1].nHeightSrc,
			RGB(255,0,255));

	}

	DeleteDC(hdcMainBitmap);
}

void CImageLibrary::GetBitmapName(char* Out, size_t OutSize, sg_uint16 nBitmap)
{
	if((nBitmap<1)||(nBitmap>m_nNumBitmaps)){
		strcpy_s(Out, OutSize, "");
	}else{
		strcpy_s(Out, OutSize, m_szBitmapFilenameA[nBitmap-1]);
	}
}
