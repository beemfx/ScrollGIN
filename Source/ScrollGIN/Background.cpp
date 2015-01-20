/*
	Background.cpp - The background class

	Copyright (c) 2002, Blaine Myers
*/

#include "background.h"
/*
extern DWORD g_nDeviceWidth;
extern DWORD g_nDeviceHeight;
*/

CBackground::CBackground(){
	m_nCurrentImage=0;
	m_nScrollMode=FOUR_DIRECTIONS;
}

CBackground::~CBackground(){

}

CImage* CBackground::LetPointer(int nImage){
	if(nImage<m_nCurrentImage || nImage>MAX_BACKGROUNDS)return NULL;
	return &m_cBackgroundImage[nImage-1];
}

void CBackground::SetScrollMode(SCROLLMODE nNewMode){
	m_nScrollMode=nNewMode;
}

BOOL CBackground::DrawBackgrounds(LPVOID lpBuffer, int x, int y, DWORD dwWidth, DWORD dwHeight){
	for(int i=0; i<m_nCurrentImage; i++){
		DisplayBackground(i+1, lpBuffer, x, y, dwWidth, dwHeight);
	}
	return TRUE;
}

HRESULT CBackground::DisplayBackground(int nBGLayer, LPVOID lpBuffer, int x, int y, DWORD dwWidth, DWORD dwHeight){
	//check scrollmode and disable necessary scrolling
	switch(m_nScrollMode){
		case FOUR_DIRECTIONS:break;
		case LEFT_RIGHT:y=0;break;
		case UP_DOWN:x=0;break;
		case NO_SCROLL:x=0;y=0;break;
		default:break;
	}
	//we need to convert the x,y coordinates to logical screen coords
	int nAdjustedY=y-y/m_nScrollRatio[nBGLayer-1];
	int nAdjustedX=x-x/m_nScrollRatio[nBGLayer-1];
	int nYOffset=((-nAdjustedY) + (nAdjustedY/dwHeight)*dwHeight);
	int nXOffset= ((-nAdjustedX) + (nAdjustedX/dwWidth)*dwWidth);

	int nXDisplayPos=nXOffset;
	int nYDisplayPos=nYOffset;
	
	m_cBackgroundImage[nBGLayer-1].DrawPrefered(nXDisplayPos, nYDisplayPos);
	m_cBackgroundImage[nBGLayer-1].DrawPrefered(nXDisplayPos+dwWidth, nYDisplayPos);
	m_cBackgroundImage[nBGLayer-1].DrawPrefered(nXDisplayPos, nYDisplayPos+dwHeight);
	m_cBackgroundImage[nBGLayer-1].DrawPrefered(nXDisplayPos+dwWidth, nYDisplayPos+dwHeight);
	
	
	/*
	if(FAILED(m_cBackgroundImage[nBGLayer-1].DrawImage(lpBuffer, 
						x-g_nDeviceWidth, 
						y)))return E_FAIL;
	if(FAILED(m_cBackgroundImage[nBGLayer-1].DrawImage(lpBuffer,
						x,
						y-g_nDeviceHeight)))return E_FAIL;
	if(FAILED(m_cBackgroundImage[nBGLayer-1].DrawImage(lpBuffer,
						x-g_nDeviceWidth,
						y-g_nDeviceHeight)))return E_FAIL;
	*/
	return S_OK;
}

HRESULT CBackground::LoadBackgroundImage(
	LPVOID lpDevice, 
	DWORD dwTransparent,
	TCHAR szBitmapName[MAX_PATH], 
	int nScrollRatio,
	DWORD dwDeviceWidth,
	DWORD dwDeviceHeight){
	if(SUCCEEDED(LoadBackgroundImage(
		lpDevice,
		dwTransparent,
		m_nCurrentImage+1, 
		szBitmapName, 
		nScrollRatio, 
		dwDeviceWidth,
		dwDeviceHeight)))
	{
		m_nCurrentImage++;
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CBackground::LoadBackgroundImage(
	LPVOID lpDevice, 
	DWORD dwTransparent,
	int nImage, TCHAR szBitmapName[MAX_PATH], 
	int nScrollRatio,
	DWORD dwDeviceWidth,
	DWORD dwDeviceHeight){
	if(nImage<1 || nImage >MAX_BACKGROUNDS)return E_FAIL;

	HRESULT hr=0;
	//first we erase any potential image
	m_cBackgroundImage[nImage-1].Release();

	if(szBitmapName[0]==NULL)return E_FAIL;
	
	HBITMAP hTempBM=(HBITMAP)LoadImage(NULL, szBitmapName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	//HBITMAP hTempBM=LoadBitmapOffset(szBitmapName, 0);
	BITMAP bmTemp;
	GetObject(hTempBM, sizeof(bmTemp), &bmTemp);
	int nFWidth=bmTemp.bmWidth;
	int nFHeight=bmTemp.bmHeight;
	//DeleteObject(hTempBM);
	hr=m_cBackgroundImage[nImage-1].CreateImageBMInMemory(
		lpDevice,
		dwTransparent,
		hTempBM, 
		0, 
		0, 
		nFWidth, 
		nFHeight, 
		dwDeviceWidth, 
		dwDeviceHeight, 
		NULL);
	DeleteObject(hTempBM);

	/*
	if(FAILED(m_cBackgroundImage[nImage-1].CreateImageBM(0, szBitmapName, 0, 0, nFWidth, nFHeight,
						g_nDeviceWidth, g_nDeviceHeight, NULL)))return E_FAIL;
	*/
	m_nScrollRatio[nImage-1]=nScrollRatio;

	if(FAILED(hr))return E_FAIL;
	return S_OK;
}

void CBackground::Release(){
	for(int i=0; i<m_nCurrentImage; i++)
		m_cBackgroundImage[i].Release();
}

void CBackground::ReloadImages(){
	for(int i=0; i<m_nCurrentImage; i++)
		m_cBackgroundImage[i].ReloadImageIntoSurface();
}

void CBackground::Restore(){
	for(int i=0; i<m_nCurrentImage; i++)
		m_cBackgroundImage[i].Restore();
}