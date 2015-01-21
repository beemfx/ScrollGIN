/*
	Background.cpp - The background class

	Copyright (c) 2002, Blaine Myers
*/

#include "background.h"
#include "../Renderer2/RendererImage.h"

SgBackground::SgBackground()
{
	m_nCurrentImage=0;
	m_nScrollMode=FOUR_DIRECTIONS;

	for(int i=0; i<MAX_BACKGROUNDS; i++)
	{
		m_cBackgroundImage[i] = 0;
	}
}

SgBackground::~SgBackground()
{

}

SgRendererImage* SgBackground::LetPointer(int nImage)
{
	if(nImage<m_nCurrentImage || nImage>MAX_BACKGROUNDS)return 0;
	return m_cBackgroundImage[nImage-1];
}

void SgBackground::SetScrollMode(SCROLLMODE nNewMode)
{
	m_nScrollMode=nNewMode;
}

void SgBackground::DrawBackgrounds(int x, int y, int dwWidth, int dwHeight)
{
	for(int i=0; i<m_nCurrentImage; i++){
		DisplayBackground(i+1, x, y, dwWidth, dwHeight);
	}
}

void SgBackground::DisplayBackground(int nBGLayer, int x, int y, int dwWidth, int dwHeight)
{
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
}

bool SgBackground::LoadBackgroundImage( const char* szBitmapName , int nScrollRatio , int dwDeviceWidth , int dwDeviceHeight )
{
	if(LoadBackgroundImage(
		m_nCurrentImage+1, 
		szBitmapName, 
		nScrollRatio, 
		dwDeviceWidth,
		dwDeviceHeight))
	{
		m_nCurrentImage++;
		return true;
	}
	return false;
}

void SgBackground::Release()
{
	for(int i=0; i<m_nCurrentImage; i++)
	{
		Renderer_DestroySprite( m_cBackgroundImage[i] );
		m_cBackgroundImage[i] = 0;
	}
}

#include <Windows.h>

bool SgBackground::LoadBackgroundImage(int nImage, const char* szBitmapName, int nScrollRatio, int dwDeviceWidth, int dwDeviceHeight)
{
	if(nImage<1 || nImage >MAX_BACKGROUNDS)return false;

	//first we erase any potential image
	Renderer_DestroySprite( m_cBackgroundImage[nImage-1] );
	m_cBackgroundImage[nImage-1] = 0;

	if(szBitmapName[0]==0)return false;
	
	HBITMAP hTempBM=(HBITMAP)LoadImage(0, szBitmapName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
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

	m_nScrollRatio[nImage-1]=nScrollRatio;

	return true;
}
