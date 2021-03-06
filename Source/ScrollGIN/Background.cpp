/*
	Background.cpp - The background class

	Copyright (c) 2002, Blaine Myers
*/

#include "background.h"
#include "../Renderer2/RendererImage.h"
#include "../ImageLib/bitmapex.h"

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
	switch(m_nScrollMode)
	{
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
	if(InsertImage(
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

void SgBackground::Destroy()
{
	for(int i=0; i<m_nCurrentImage; i++)
	{
		Renderer_DestroySprite( m_cBackgroundImage[i] );
		m_cBackgroundImage[i] = 0;
	}
	m_nCurrentImage = 0;
}

#include <Windows.h>

bool SgBackground::InsertImage(int nImage, const char* szBitmapName, int nScrollRatio, int dwDeviceWidth, int dwDeviceHeight)
{
	if(nImage<1 || nImage >MAX_BACKGROUNDS)return false;

	//first we erase any potential image
	Renderer_DestroySprite( m_cBackgroundImage[nImage-1] );
	m_cBackgroundImage[nImage-1] = 0;

	if(szBitmapName[0]==0)return false;
	
	sgRendererImageCreateParms Parms;
	memset(&Parms, 0, sizeof(Parms));
	Parms.Type = RENDERER_IMAGE_BITMAP;
	strcpy_s( Parms.BmFile , countof(Parms.BmFile), szBitmapName );
	Parms.BmFileOffset = 0;
	Parms.BmX = IMAGE_CREATE_DEFAULT;
	Parms.BmY = IMAGE_CREATE_DEFAULT;
	Parms.BmWidth = IMAGE_CREATE_DEFAULT;
	Parms.BmHeight = IMAGE_CREATE_DEFAULT;
	Parms.Width = dwDeviceWidth;
	Parms.Height = dwDeviceHeight;

	m_cBackgroundImage[nImage-1] = Renderer_CreateSprite( &Parms );

	m_nScrollRatio[nImage-1]=nScrollRatio;

	return true;
}
