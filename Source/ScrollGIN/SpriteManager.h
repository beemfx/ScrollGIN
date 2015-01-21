/*
	Spritemanager.h - header for the sprite manager class
	
	Copyright (c) 2002, Blaine Myers
*/
#ifndef __SPRITEMANAGER_H__
#define __SPRITEMANAGER_H__

#include "sprite.h"

typedef struct tagSPRITECREATESTRUCT{
	int nFX;
	int nFY;
	int nFWidth;
	int nFHeight;
	int nWidth;
	int nHeight;
}SPRITECREATESTRUCT;

class CSpriteManager{
private:
	CSprite	m_cSprite[MAX_SPRITES]; //the sprites
	int	m_nNumSprites; //indicates how many sprites are available
	const DWORD m_dwMaxSprites;
public:
	//constructor and destruct
	CSpriteManager(); //constructor
	~CSpriteManager(); //destructor

	//Functions for restoring and destroying sprites
	void Release(); //Releases all sprite surfaces
	
	void ClearDataBase(); //this function clears the database

	CSprite* LetPointer(int nSpriteIndex); //Gives out a pointer to a sprite
	CSprite* LetPointer(LPTSTR szSpriteName);//Give out a pointer to a sprite by name
	HRESULT DisplaySprite(
		int nSprite, 
		int nFrame, 
		SPRITEFACE nFace, 
		int x, 
		int y); //Displays a sprite

	//I need to create a series of functions for creating sprites
	//including functions that load sprites from a file, and others
	HRESULT CreateSpritesFromFile(DWORD nSprite, LPTSTR szFilename);
	HRESULT CreateSpritesFromFile(LPTSTR szFilename);
};


#endif //__spritemanager_h__