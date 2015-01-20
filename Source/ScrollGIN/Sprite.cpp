/*
	sprite.cpp - the sprite class

	Copyright (c) 2002, Blaine Myers
*/

#include <tchar.h>
#include "sprite.h"

CSprite::CSprite(){
	m_nFrames=0;
	m_szSpriteName[0]=NULL;
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
	LPVOID lpDevice, 
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
		lpDevice,
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

HRESULT CSprite::CreateSpriteFrameBM(
	LPVOID lpDevice, 
	DWORD dwTransparent,
	TCHAR lpFilename[MAX_PATH],
	int nWidth,
	int nHeight,
	int nFX,
	int nFY,
	int nFWidth,
	int nFHeight
){
	if(SUCCEEDED(CreateSpriteFrameBM(
		lpDevice,
		dwTransparent,
		m_nFrames+1,
		lpFilename,
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
	LPVOID lpDevice, 
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

	if(FAILED(m_cSpriteImage[nFrame-1].CreateImageBMInMemory(
		lpDevice,
		dwTransparent,
		hBitmap,
		nFX,
		nFY,
		nFWidth,
		nFHeight,
		nWidth,
		nHeight,
		NULL)))return E_FAIL;

	if(FAILED(m_cReverseSpriteImage[nFrame-1].CreateImageBMInMemory(
		lpDevice,
		dwTransparent,
		hBitmap,
		nFX,
		nFY,
		nFWidth,
		nFHeight,
		nWidth,
		nHeight,
		RV_LEFTRIGHT)))return E_FAIL;
	
	return S_OK;
}

HRESULT CSprite::CreateSpriteFrameBM(
	LPVOID lpDevice, 
	DWORD dwTransparent,
	int nFrame,
	TCHAR lpFilename[MAX_PATH],
	int nWidth,
	int nHeight,
	int nFX,
	int nFY,
	int nFWidth,
	int nFHeight
){
	if(nFrame<1)return E_FAIL;
	if(nFrame>MAX_IMAGES_PER_SPRITE)return E_FAIL;

	m_sImageDim[nFrame-1].nWidth=nWidth;
	m_sImageDim[nFrame-1].nHeight=nHeight;

	if(FAILED(m_cSpriteImage[nFrame-1].CreateImageBM(
		lpDevice,
		dwTransparent,
		lpFilename,
		nFX,
		nFY,
		nFWidth,
		nFHeight,
		nWidth,
		nHeight,
		NULL)))return E_FAIL;

	
	//this isn't working
	if(FAILED(m_cReverseSpriteImage[nFrame-1].CreateImageBM( 
		lpDevice,
		dwTransparent,
		lpFilename, 
		nFX, 
		nFY, 
		nFWidth, 
		nFHeight,
		nWidth, 
		nHeight,
		RV_LEFTRIGHT)))return E_FAIL;
	

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

HRESULT CSprite::ReloadImages(){
	for(int i=0; i<m_nFrames; i++){
		m_cSpriteImage[i].ReloadImageIntoSurface();
		m_cReverseSpriteImage[i].ReloadImageIntoSurface();
	}

	return S_OK;
}

HRESULT CSprite::Restore(){
	for(int i=0; i<m_nFrames; i++){
		m_cSpriteImage[i].Restore();
		m_cReverseSpriteImage[i].Restore();
	}
	return S_OK;
}

void CSprite::Release(){
	for(int i=0; i<MAX_IMAGES_PER_SPRITE; i++){
		m_cSpriteImage[i].Release();
		m_cReverseSpriteImage[i].Release();
	}
}

HRESULT CSprite::DisplaySprite(LPVOID lpBuffer, int nFrame, SPRITEFACE nFace, int x, int y, LOOPMODE nLoopMode){
	if(!lpBuffer)return E_FAIL;
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
		if(SUCCEEDED(m_cSpriteImage[nFrame-1].DrawPrefered(x-m_sImageDim[nFrame-1].nWidth/2, y-m_sImageDim[nFrame-1].nHeight/2)))return S_OK;
	break;
	case SF_LEFT:
		//to the left means to reverse the iamge
		if(SUCCEEDED(m_cReverseSpriteImage[nFrame-1].DrawPrefered(x-m_sImageDim[nFrame-1].nWidth/2, y-m_sImageDim[nFrame-1].nHeight/2)))return S_OK;
		break;
	}
	return E_FAIL;
}
