/*
	Spritemanager.h - header for the sprite manager class
	
	Copyright (c) 2002, Blaine Myers
*/
#ifndef __SPRITEMANAGER_H__
#define __SPRITEMANAGER_H__

#include "sprite.h"

class SgSpriteManager
{
public:
	//constructor and destruct
	SgSpriteManager(); //constructor
	~SgSpriteManager(); //destructor

	void LoadLib(const char* szFilename);
	void Clear();
	
	SgSprite* GetSprite(const char* szSpriteName);

private:
	void InsertSprites(int nSprite, const char* szFilename);
private:
	SgSprite	   m_cSprite[MAX_SPRITES];
	int	      m_nNumSprites; 
	const int   m_dwMaxSprites;
};


#endif //__spritemanager_h__