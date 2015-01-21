/*
	Background.cpp - The background class

	Copyright (c) 2002, Blaine Myers
*/

#include "background.h"
#include "../Renderer2/RendererImage.h"
/*
extern DWORD g_nDeviceWidth;
extern DWORD g_nDeviceHeight;
*/

SgBackground::SgBackground(){
	m_nCurrentImage=0;
	m_nScrollMode=FOUR_DIRECTIONS;
}

SgBackground::~SgBackground(){

}

SgRendererImage* SgBackground::LetPointer(int nImage){
	if(nImage<m_nCurrentImage || nImage>MAX_BACKGROUNDS)return NULL;
	return m_cBackgroundImage[nImage-1];
}

void SgBackground::SetScrollMode(SCROLLMODE nNewMode){
	m_nScrollMode=nNewMode;
}

bool SgBackground::DrawBackgrounds(int x, int y, DWORD dwWidth, DWORD dwHeight){
	for(int i=0; i<m_nCurrentImage; i++){
		DisplayBackground(i+1, x, y, dwWidth, dwHeight);
	}
	return TRUE;
}

HRESULT SgBackground::DisplayBackground(int nBGLayer, int x, int y, DWORD dwWidth, DWORD dwHeight){
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
	
	m_cBackgroundImage[nBGLayer-1]->Draw(nXDisplayPos, nYDisplayPos);
	m_cBackgroundImage[nBGLayer-1]->Draw(nXDisplayPos+dwWidth, nYDisplayPos);
	m_cBackgroundImage[nBGLayer-1]->Draw(nXDisplayPos, nYDisplayPos+dwHeight);
	m_cBackgroundImage[nBGLayer-1]->Draw(nXDisplayPos+dwWidth, nYDisplayPos+dwHeight);
	
	return S_OK;
}

HRESULT SgBackground::LoadBackgroundImage(
	char szBitmapName[MAX_PATH], 
	int nScrollRatio,
	DWORD dwDeviceWidth,
	DWORD dwDeviceHeight){
	if(SUCCEEDED(LoadBackgroundImage(
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

HRESULT SgBackground::LoadBackgroundImage(
	int nImage, char szBitmapName[MAX_PATH], 
	int nScrollRatio,
	DWORD dwDeviceWidth,
	DWORD dwDeviceHeight){
	if(nImage<1 || nImage >MAX_BACKGROUNDS)return E_FAIL;

	HRESULT hr=0;
	//first we erase any potential image
	Renderer_DestroySprite( m_cBackgroundImage[nImage-1] );

	if(szBitmapName[0]==NULL)return E_FAIL;
	
	HBITMAP hTempBM=(HBITMAP)LoadImage(NULL, szBitmapName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	//HBITMAP hTempBM=LoadBitmapOffset(szBitmapName, 0);
	BITMAP bmTemp;
	GetObject(hTempBM, sizeof(bmTemp), &bmTemp);
	int nFWidth=bmTemp.bmWidth;
	int nFHeight=bmTemp.bmHeight;
	//DeleteObject(hTempBM);
	sgRendererImageCreateParms Parms;
	memset(&Parms, 0, sizeof(Parms));
	Parms.Type = RENDERER_IMAGE_BITMAP;
	Parms.Bitmap = hTempBM;
	Parms.BmX = 0;
	Parms.BmY = 0;
	Parms.BmWidth = nFWidth;
	Parms.BmHeight = nFHeight;
	Parms.Width = dwDeviceWidth;
	Parms.Height = dwDeviceHeight;

	m_cBackgroundImage[nImage-1] = Renderer_CreateSprite( &Parms );

	DeleteObject(hTempBM);

	/*
	if(FAILED(m_cBackgroundImage[nImage-1].CreateImageBM(0, szBitmapName, 0, 0, nFWidth, nFHeight,
						g_nDeviceWidth, g_nDeviceHeight, NULL)))return E_FAIL;
	*/
	m_nScrollRatio[nImage-1]=nScrollRatio;

	if(FAILED(hr))return E_FAIL;
	return S_OK;
}

void SgBackground::Release(){
	for(int i=0; i<m_nCurrentImage; i++)
		Renderer_DestroySprite( m_cBackgroundImage[i] );
}
