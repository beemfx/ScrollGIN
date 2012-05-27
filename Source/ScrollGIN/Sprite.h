/*
	sprite.h - header for teh sprite class

	Copyright (c) 2002, Blaine Myers
*/
#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <windows.h>
#include "defines.h"
#include "../GFXG7/gfxg7.h"

typedef enum tagSPRITEFACE{
	SF_LEFT=0, 
	SF_RIGHT
}SPRITEFACE;


typedef struct tagIMAGEDIM{
	int nWidth;
	int nHeight;
}IMAGEDIM;

typedef enum tagLOOPMODE{
	LP_FORWARD=0,
	LP_FORWARDBACKWARD,
	LP_BACKWARD,
	LP_ONCEFORWARD,
	LP_ONCEBACKWARD
}LOOPMODE;

class CSprite{
private:
	CImage m_cSpriteImage[MAX_IMAGES_PER_SPRITE]; //contains the default sprite images
	CImage m_cReverseSpriteImage[MAX_IMAGES_PER_SPRITE]; //contains a left/right swaped version of teh sprite images
	IMAGEDIM m_sImageDim[MAX_IMAGES_PER_SPRITE];  //contains the dimensions of the sprite
	int m_nFrames; //indicates how many frames are in teh sprite
	TCHAR m_szSpriteName[MAX_SPRITE_NAME_LENGTH+1]; //The name of the sprite, used to objtain a sprite
	//BOOL m_bLoopBackward; //if true the sprite should go through each frame, forward then backward
public:
	CSprite();  //cosntructor
	~CSprite(); //destructor
	HRESULT DisplaySprite(LPVOID lpBuffer, 
						int nFrame, 
						SPRITEFACE nFace, 
						int x, 
						int y,
						LOOPMODE nLoopMode); //pastes the sprite to the indicated surface using the center of the image as the x, y
	HRESULT Restore(); //restores the surfaces of the sprite
	void Release(); //releases the surfaces of the sprite
	HRESULT ReloadImages(); //reloads images into sprites
	HRESULT ClearSprite();  //clears the current sprite for replacement
	HRESULT CreateSpriteFrameBM(
		LPVOID lpDevice, 
		DWORD dwTransparent,
		int nFrame, 
		TCHAR lpFilename[MAX_PATH],
		int nWidth, 
		int nHeight, 
		int nFX, 
		int nFY, 
		int nFWidth, 
		int nFHeight);//creaet sprite fraem from bitmap
	HRESULT CreateSpriteFrameBM(
		LPVOID lpDevice, 
		DWORD dwTransparent,
		TCHAR lpFilename[MAX_PATH], 
		int nWidth, 
		int nHeight, 
		int nFX, 
		int nFY, 
		int nFWidth, 
		int nFHeight);//create sprite frame from bitmap at next available position
	HRESULT CreateSpriteFrameBMInMemory(
		LPVOID lpDevice, 
		DWORD dwTransparent,
		int nFrame, 
		HBITMAP hBitmap,
		int nWidth, 
		int nHeight, 
		int nFX, 
		int nFY, 
		int nFWidth, 
		int nFHeight);//creaet sprite fraem from bitmap
	HRESULT CreateSpriteFrameBMInMemory(
		LPVOID lpDevice, 
		DWORD dwTransparent,
		HBITMAP hBitmap, 
		int nWidth, 
		int nHeight, 
		int nFX, 
		int nFY, 
		int nFWidth, 
		int nFHeight);
	int GetNumFrames(LOOPMODE nLoopMode); //Returns number of frames in sprite depending on loop mode
	BOOL NameSprite(TCHAR szSpriteName[MAX_SPRITE_NAME_LENGTH+1]); //names the sprite for reference, no more than 32 characters
	BOOL GetSpriteName(TCHAR szSpriteName[MAX_SPRITE_NAME_LENGTH+1]); //retreives the naem of the sprite for reference
	//HRESULT SetLoopBackward(BOOL bLoopMode);
};

#endif //__sprite_h__