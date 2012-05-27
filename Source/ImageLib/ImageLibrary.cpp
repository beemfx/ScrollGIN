/*
	ImageLibrary.cpp - The CImageLibrary class

	Copyright (c) 2003, Blaine Myers
*/

#include "bitmapex.h"
#include <stdio.h>
#include "ImageLib.h"
#include "Defines.h"

CImageLibrary::CImageLibrary(){
	m_nNumImages=0;
	m_nNumBitmaps=0;
	m_pImageData=NULL;
	/*
	ZeroMemory(m_szBitmapFilenameA, sizeof(m_szBitmapFilenameA));
	ZeroMemory(m_szBitmapFilenameW, sizeof(m_szBitmapFilenameW));
	*/
	for(int i=0; i<MAX_BITMAPS; i++){
		m_szBitmapFilenameA[i][0]=0;
		m_szBitmapFilenameW[i][0]=0;
		m_hBitmap[i]=NULL;
	}
}

CImageLibrary::~CImageLibrary(){
	SAFE_DELETE_ARRAY(m_pImageData);
	CloseMainBitmaps();
}

BOOL CImageLibrary::GetImageData(DWORD nEntry, IMAGEDATA *imgData){
	if((nEntry<1) || (nEntry>m_nNumImages))return FALSE;

	imgData->nBitmap=m_pImageData[nEntry-1].nBitmap;
	imgData->nFrames=m_pImageData[nEntry-1].nFrames;
	imgData->nHeight=m_pImageData[nEntry-1].nHeight;
	imgData->nHeightSrc=m_pImageData[nEntry-1].nHeightSrc;
	imgData->nWidth=m_pImageData[nEntry-1].nWidth;
	imgData->nWidthSrc=m_pImageData[nEntry-1].nWidthSrc;
	imgData->nX=m_pImageData[nEntry-1].nX;
	imgData->nY=m_pImageData[nEntry-1].nY;

	wcscpy(imgData->szImgLabel, m_pImageData[nEntry-1].szImgLabel);

	return TRUE;
}

WORD CImageLibrary::GetNumFrames(DWORD nEntry){
	//return 0 if image doesn't exist
	if((nEntry<1) || (nEntry>m_nNumImages))return 0;

	return m_pImageData[nEntry-1].nFrames;
}

BOOL CImageLibrary::GetImageNameA(char szImageName[], DWORD nEntry){
	

	char szTemp[IMAGE_NAME_LENGTH];

	BOOL result=FALSE;

	result=WideCharToMultiByte(
		CP_ACP, 
		0, 
		m_pImageData[nEntry-1].szImgLabel, 
		-1,
		szTemp,
		IMAGE_NAME_LENGTH,
		NULL,
		NULL);

	strcpy(szImageName, szTemp);
	return result;
}

BOOL CImageLibrary::GetImageNameW(WCHAR szImageName[], DWORD nEntry){
	if((nEntry<1) || (nEntry>m_nNumImages))return FALSE;
	wcscpy(szImageName, m_pImageData[nEntry-1].szImgLabel);
	return TRUE;
}

HBITMAP CImageLibrary::GetBitmap(WORD nBitmap){
	if((nBitmap<1) || (nBitmap>MAX_BITMAPS))return NULL;

	return m_hBitmap[nBitmap-1];
}

DWORD CImageLibrary::GetNumEntries(){
	return m_nNumImages;
}


HRESULT CImageLibrary::OpenBitmapA(LPSTR szFilename, WORD nBitmap){
	return OpenBitmapOffsetA(szFilename, 0, nBitmap);
}

HRESULT CImageLibrary::OpenBitmapW(LPWSTR szFilename, WORD nBitmap){
	return OpenBitmapOffsetW(szFilename, 0, nBitmap);
}

HRESULT CImageLibrary::OpenBitmapOffsetA(LPSTR szFilename, DWORD nOffset, WORD nBitmap){
	if( (nBitmap<1) || (nBitmap>MAX_BITMAPS) )return E_FAIL;

	CloseHandle(m_hBitmap[nBitmap-1]);
	m_hBitmap[nBitmap-1]=LoadBitmapOffsetA(szFilename, nOffset);

	if(m_hBitmap[nBitmap-1] == NULL)return E_FAIL;
	strcpy(m_szBitmapFilenameA[nBitmap-1], szFilename);
	return S_OK;
}

HRESULT CImageLibrary::OpenBitmapOffsetW(LPWSTR szFilename, DWORD nOffset, WORD nBitmap){
	if( (nBitmap<1) || (nBitmap>MAX_BITMAPS) )return E_FAIL;

	CloseHandle(m_hBitmap[nBitmap-1]);
	m_hBitmap[nBitmap-1]=LoadBitmapOffsetW(szFilename, nOffset);

	if(m_hBitmap[nBitmap-1] == NULL)return E_FAIL;
	wcscpy(m_szBitmapFilenameW[nBitmap-1], szFilename);
	return S_OK;
}

void CImageLibrary::CloseMainBitmaps(){
	for(int i=0; i<m_nNumBitmaps; i++){
		DeleteObject(m_hBitmap[i]);
	}
	m_nNumBitmaps=0;
}


HRESULT CImageLibrary::ClearDataBase(){
	for(DWORD i=0;i<m_nNumImages;i++){
		ZeroMemory(&m_pImageData[i], sizeof(IMAGEDATA));
	}
	m_nNumImages=0;
	m_nNumBitmaps=0;
	CloseMainBitmaps();
	SAFE_DELETE_ARRAY(m_pImageData);
	m_pImageData=NULL;
	return S_OK;
}


HRESULT CImageLibrary::CopyImageToDC(
	HDC hdcDest, 
	DWORD nEntry, 
	int x, 
	int y, 
	BOOL bTransp)
{
	HDC hdcMainBitmap=NULL;

	if( (nEntry<1) || (nEntry>m_nNumImages) )return S_FALSE;

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

	return S_OK;
}

WORD CImageLibrary::GetNumBitmaps(){
	return m_nNumBitmaps;
}

HRESULT CImageLibrary::StretchImageToDC(
	HDC hdcDest, 
	DWORD nEntry, 
	int x, 
	int y, 
	int nWidth, 
	int nHeight, 
	BOOL bTransp)
{
HDC hdcMainBitmap=NULL;

	if( (nEntry<1) || (nEntry>m_nNumImages) )return S_FALSE;

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

	return S_OK;
}

void CImageLibrary::GetBitmapNameA(char szName[], WORD nBitmap){
	if((nBitmap<1)||(nBitmap>m_nNumBitmaps)){
		strcpy(szName, "");
	}else{
		strcpy(szName, m_szBitmapFilenameA[nBitmap-1]);
	}
}

void CImageLibrary::GetBitmapNameW(WCHAR szName[], WORD nBitmap){
	if((nBitmap<1)||(nBitmap>m_nNumBitmaps)){
		wcscpy(szName, L"");
	}else{
		wcscpy(szName, m_szBitmapFilenameW[nBitmap-1]);
	}
}
