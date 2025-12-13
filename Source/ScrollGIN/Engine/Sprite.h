/*
	sprite.h - header for teh sprite class

	Copyright (c) 2002, Blaine Myers
*/
#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <windows.h>
#include "GameConfig.h"
#include "../Renderer2/Renderer.h"

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

class SgSprite{
private:
	SgRendererImage* m_cSpriteImage[MAX_IMAGES_PER_SPRITE]; //contains the default sprite images
	SgRendererImage* m_cReverseSpriteImage[MAX_IMAGES_PER_SPRITE]; //contains a left/right swaped version of teh sprite images
	IMAGEDIM m_sImageDim[MAX_IMAGES_PER_SPRITE];  //contains the dimensions of the sprite
	int m_nFrames; //indicates how many frames are in teh sprite
	char m_szSpriteName[MAX_SPRITE_NAME_LENGTH+1]; //The name of the sprite, used to objtain a sprite
	//BOOL m_bLoopBackward; //if true the sprite should go through each frame, forward then backward
public:
	SgSprite();
	~SgSprite();
	void Draw(int nFrame, SPRITEFACE nFace, int x, int y, LOOPMODE nLoopMode);
	bool CreateSprite(sg_cpstr BmFile , int BmOffset, int nWidth, int nHeight, int nFX, int nFY, int nFWidth, int nFHeight);
	void Destroy(); //releases the surfaces of the sprite
	int  GetNumFrames(LOOPMODE nLoopMode); //Returns number of frames in sprite depending on loop mode
	void NameSprite(const char* szSpriteName); //names the sprite for reference, no more than 32 characters
	void GetSpriteName( char* Out , size_t MaxOut ); //retreives the naem of the sprite for reference
private:
	bool CreateNextSprite(int nFrame, sg_cpstr BmFile , int BmOffset,int nWidth, int nHeight, int nFX,int nFY, int nFWidth, int nFHeight);//creaet sprite fraem from bitmap
};

#endif //__sprite_h__