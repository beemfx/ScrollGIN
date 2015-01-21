/*
	sprite.cpp - the sprite class

	Copyright (c) 2002, Blaine Myers
*/

#include <tchar.h>
#include "sprite.h"
#include "../Renderer2/RendererImage.h"

CSprite::CSprite(){
	m_nFrames=0;
	m_szSpriteName[0]=NULL;
	for( int i=0; i<MAX_IMAGES_PER_SPRITE; i++ )
	{
		m_cSpriteImage[i] = 0;
		m_cReverseSpriteImage[i] = 0;
	}
}

CSprite::~CSprite(){
}

BOOL CSprite::GetSpriteName(TCHAR szSpriteName[MAX_SPRITE_NAME_LENGTH+1]){
	_tcscpy(szSpriteName, m_szSpriteName);
	return TRUE;
}

BOOL CSprite::NameSprite(TCHAR szSpriteName[MAX_SPRITE_NAME_LENGTH+1]){
	_tcscpy(m_szSpriteName, szSpriteName);
	return TRUE;
}

HRESULT CSprite::CreateSpriteFrameBMInMemory(
	DWORD dwTransparent,
	HBITMAP hBitmap, 
	int nWidth, 
	int nHeight, 
	int nFX, 
	int nFY, 
	int nFWidth, 
	int nFHeight)
{

	if(SUCCEEDED(CreateSpriteFrameBMInMemory(
		dwTransparent,
		m_nFrames+1,
		hBitmap,
		nWidth,
		nHeight,
		nFX,
		nFY,
		nFWidth,
		nFHeight)))
	{
		m_nFrames++;
		return S_OK;
	}else return E_FAIL;
	
}

HRESULT CSprite::CreateSpriteFrameBMInMemory(
	DWORD dwTransparent,
	int nFrame, 
	HBITMAP hBitmap,
	int nWidth, 
	int nHeight, 
	int nFX, 
	int nFY, 
	int nFWidth, 
	int nFHeight)
{
	if(nFrame<1)return E_FAIL;
	if(nFrame>MAX_IMAGES_PER_SPRITE)return E_FAIL;

	m_sImageDim[nFrame-1].nWidth=nWidth;
	m_sImageDim[nFrame-1].nHeight=nHeight;

	sgRendererImageCreateParms Parms;
	memset( &Parms , 0 , sizeof(Parms) );
	Parms.Type = RENDERER_IMAGE_BITMAP;
	Parms.BmX = nFX;
	Parms.BmY = nFY;
	Parms.BmWidth = nFWidth;
	Parms.BmHeight = nFHeight;
	Parms.Bitmap = hBitmap;
	Parms.Width = nWidth;
	Parms.Height = nHeight;

	m_cSpriteImage[nFrame-1]        = Renderer_CreateSprite( &Parms );
	m_cReverseSpriteImage[nFrame-1] = Renderer_CreateSprite( &Parms );
	
	return S_OK;
}

/*
HRESULT CSprite::SetLoopBackward(BOOL bLoopMode){
	m_bLoopBackward=bLoopMode;
	return S_OK;
}
*/

HRESULT CSprite::ClearSprite(){
	Release();

	m_nFrames=0;

	return S_OK;
}

int CSprite::GetNumFrames(LOOPMODE nLoopMode){
	if((nLoopMode==LP_FORWARDBACKWARD))
		if(m_nFrames==1)return 1;
		else return ((m_nFrames*2)-2);

	else return m_nFrames;
}

void CSprite::Release(){
	for(int i=0; i<MAX_IMAGES_PER_SPRITE; i++)
	{
		Renderer_DestroySprite( m_cSpriteImage[i] );
		m_cSpriteImage[i] = 0;
		Renderer_DestroySprite( m_cReverseSpriteImage[i] );
		m_cReverseSpriteImage[i] = 0;
	}
}

HRESULT CSprite::DisplaySprite(int nFrame, SPRITEFACE nFace, int x, int y, LOOPMODE nLoopMode)
{
	switch(nLoopMode)
	{
	case LP_FORWARDBACKWARD:
		if(m_nFrames>1)
			if(nFrame>((m_nFrames*2)-2))
				return E_FAIL;
		if(nFrame>m_nFrames)
			nFrame=(nFrame-(2*(nFrame-m_nFrames)));
		break;
	case LP_FORWARD:
		if(nFrame>m_nFrames)return E_FAIL;
		break;
	case LP_BACKWARD:
		if(nFrame>m_nFrames)return E_FAIL;
		nFrame=m_nFrames-nFrame+1;
		break;
	case LP_ONCEFORWARD:
		if(nFrame>m_nFrames)return E_FAIL;
		break;
	case LP_ONCEBACKWARD:
		if(nFrame>m_nFrames)return E_FAIL;
		nFrame=m_nFrames-nFrame+1;
		break;
	}
	/*
	if(nLoopMode==LPF){
		if(nFrame>m_nFrames)return E_FAIL;
	}else{
		if(nFrame>((m_nFrames*2)-2))return E_FAIL;
	}
	if(!lpBuffer)return E_FAIL;

	//determine which frame to draw if looping
	if(m_bLoopBackward){
		if(nFrame>m_nFrames){
			nFrame=(nFrame-(2*(nFrame-m_nFrames)));
		}
	}
	*/


	switch(nFace){
	case SF_RIGHT:
		//To the right means we blt the actual image
		m_cSpriteImage[nFrame-1]->Draw(x-m_sImageDim[nFrame-1].nWidth/2, y-m_sImageDim[nFrame-1].nHeight/2);
		break;
	case SF_LEFT:
		//to the left means to reverse the iamge
		m_cReverseSpriteImage[nFrame-1]->Draw(x-m_sImageDim[nFrame-1].nWidth/2, y-m_sImageDim[nFrame-1].nHeight/2);
		break;
	}
	return S_OK;
}
