/*
	sprite.cpp - the sprite class

	Copyright (c) 2002, Blaine Myers
*/

#include "sprite.h"
#include "../Renderer2/RendererImage.h"

SgSprite::SgSprite()
{
	m_nFrames=0;
	m_szSpriteName[0]=NULL;
	for( int i=0; i<MAX_IMAGES_PER_SPRITE; i++ )
	{
		m_cSpriteImage[i] = 0;
		m_cReverseSpriteImage[i] = 0;
	}
}

SgSprite::~SgSprite(){
}

void SgSprite::GetSpriteName( char* Out , size_t MaxOut )
{
	strcpy_s( Out , MaxOut , m_szSpriteName );
}

void SgSprite::NameSprite(const char* szSpriteName)
{
	strcpy_s( m_szSpriteName , countof(m_szSpriteName) , szSpriteName );
}

bool SgSprite::CreateSprite(sg_cpstr BmFile , int BmOffset, int nWidth, int nHeight, int nFX, int nFY, int nFWidth, int nFHeight)
{

	if(CreateNextSprite(
		m_nFrames+1,
		BmFile,
		BmOffset,
		nWidth,
		nHeight,
		nFX,
		nFY,
		nFWidth,
		nFHeight))
	{
		m_nFrames++;
		return true;
	}

	return false;	
}

bool SgSprite::CreateNextSprite(int nFrame, sg_cpstr BmFile , int BmOffset, int nWidth, int nHeight, int nFX, int nFY, int nFWidth, int nFHeight)
{
	if(nFrame<1)return false;
	if(nFrame>MAX_IMAGES_PER_SPRITE)return false;

	m_sImageDim[nFrame-1].nWidth=nWidth;
	m_sImageDim[nFrame-1].nHeight=nHeight;

	sgRendererImageCreateParms Parms;
	memset( &Parms , 0 , sizeof(Parms) );
	Parms.Type = RENDERER_IMAGE_BITMAP;
	Parms.BmX = nFX;
	Parms.BmY = nFY;
	Parms.BmWidth = nFWidth;
	Parms.BmHeight = nFHeight;
	Parms.Width = nWidth;
	Parms.Height = nHeight;
	strcpy_s(Parms.BmFile, countof(Parms.BmFile), BmFile);
	Parms.BmFileOffset = BmOffset;

	m_cSpriteImage[nFrame-1]        = Renderer_CreateSprite( &Parms );
	Parms.BmX = nFX + nFWidth;
	Parms.BmWidth = -nFWidth;
	m_cReverseSpriteImage[nFrame-1] = Renderer_CreateSprite( &Parms );
	
	return true;
}

int SgSprite::GetNumFrames(LOOPMODE nLoopMode)
{
	if((nLoopMode==LP_FORWARDBACKWARD))
		if(m_nFrames==1)return 1;
		else return ((m_nFrames*2)-2);

	else return m_nFrames;
}

void SgSprite::Destroy()
{
	for(int i=0; i<MAX_IMAGES_PER_SPRITE; i++)
	{
		Renderer_DestroySprite( m_cSpriteImage[i] );
		m_cSpriteImage[i] = 0;
		Renderer_DestroySprite( m_cReverseSpriteImage[i] );
		m_cReverseSpriteImage[i] = 0;
	}
	m_nFrames = 0;
}

void SgSprite::Draw(int nFrame, SPRITEFACE nFace, int x, int y, LOOPMODE nLoopMode)
{
	switch(nLoopMode)
	{
	case LP_FORWARDBACKWARD:
		if(m_nFrames>1)
			if(nFrame>((m_nFrames*2)-2))
				return;
		if(nFrame>m_nFrames)
			nFrame=(nFrame-(2*(nFrame-m_nFrames)));
		break;
	case LP_FORWARD:
		if(nFrame>m_nFrames)return;
		break;
	case LP_BACKWARD:
		if(nFrame>m_nFrames)return;
		nFrame=m_nFrames-nFrame+1;
		break;
	case LP_ONCEFORWARD:
		if(nFrame>m_nFrames)return;
		break;
	case LP_ONCEBACKWARD:
		if(nFrame>m_nFrames)return;
		nFrame=m_nFrames-nFrame+1;
		break;
	}

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
	return;
}
