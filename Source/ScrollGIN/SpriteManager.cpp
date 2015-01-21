/*
	Spritemanager.cpp - The sprite  manager class

	Copyright (c) 2002, Blaine Myers
*/
/*
	The sprite manager class stores and array of sprites and provides the
	ability to do certain things with them.  One of the most important things being
	the ability to release a pointer, for other objects to use.
*/

#include <stdio.h>
#include "../ImageLib/ImageLib.h"
#include "spritemanager.h"

CSpriteManager::CSpriteManager():
m_dwMaxSprites(MAX_SPRITES)
{
	m_nNumSprites=0;
}

CSpriteManager::~CSpriteManager(){

}

HRESULT CSpriteManager::CreateSpritesFromFile(LPTSTR szFilename)
{
	if(m_nNumSprites>=MAX_SPRITES)return E_FAIL;
	return CreateSpritesFromFile(m_nNumSprites+1, szFilename);
}

HRESULT CSpriteManager::CreateSpritesFromFile(DWORD nSprite, LPTSTR szFilename){
	if((nSprite<1) || (nSprite>=MAX_SPRITES))return E_FAIL;
	m_nNumSprites=nSprite;
	CImageArchive ILibrary;

	HRESULT hr=0;
	hr=ILibrary.LoadArchive(szFilename);
	if(FAILED(hr))return E_FAIL;
	//The following algorithm loads all sprites from library
	DWORD i=0, j=0, nFrames=0;
	HBITMAP hBitmap=0;
	IMAGEDATA id;
	char szImageName[IMAGE_NAME_LENGTH];
	ZeroMemory(&id, sizeof(IMAGEDATA));

	//loop for each of the entries
	for(i=1; i<=ILibrary.GetNumEntries(); i++){
		
		ILibrary.GetImageData(i, &id);
		ILibrary.GetImageName(szImageName, i);

		
		
		
		if(id.nFrames==1){

			hBitmap=ILibrary.GetBitmap(id.nBitmap);

			m_cSprite[m_nNumSprites-1].CreateSpriteFrameBMInMemory(
				hBitmap,
				id.nWidth,
				id.nHeight,
				id.nX,
				id.nY,
				id.nWidthSrc,
				id.nHeightSrc);
			m_cSprite[m_nNumSprites-1].NameSprite(szImageName);

			m_nNumSprites++;
		}else if(id.nFrames==0){
			//do nothing this image does not need to be added
		}else if(id.nFrames>1){
			nFrames=id.nFrames;
			for(j=1; j<=nFrames; j++){
				ILibrary.GetImageData(j+i-1, &id);
				hBitmap=ILibrary.GetBitmap(id.nBitmap);

				m_cSprite[m_nNumSprites-1].CreateSpriteFrameBMInMemory(
					hBitmap,
					id.nWidth,
					id.nHeight,
					id.nX,
					id.nY,
					id.nWidthSrc,
					id.nHeightSrc);
			}
			m_cSprite[m_nNumSprites-1].NameSprite(szImageName);
			m_nNumSprites++;
		}
		
	}


	ILibrary.CloseArchive();
	return S_OK;
}

void CSpriteManager::Release(){
	for(int i=0; i<m_nNumSprites; i++)
		m_cSprite[i].Release();
}

HRESULT CSpriteManager::DisplaySprite(int nSprite, int nFrame, SPRITEFACE nFace, int x, int y){
	if(nSprite<1 || nSprite>m_nNumSprites)return E_FAIL;

	return m_cSprite[nSprite-1].DisplaySprite(nFrame, nFace, x, y, LP_FORWARD);
}

void CSpriteManager::ClearDataBase(){
	Release();
	m_nNumSprites=0;
}

//LetPointer() function release the address of the sprite
//of the chosen index, to allow another class to use that sprite
//directly.
SgSprite* CSpriteManager::LetPointer(int nSpriteIndex){
	if((nSpriteIndex<1) || (nSpriteIndex>(int)m_dwMaxSprites))return NULL;
	return &m_cSprite[nSpriteIndex-1];
}

SgSprite* CSpriteManager::LetPointer(LPTSTR szSpriteName){
	char szTempSpriteName[MAX_SPRITE_NAME_LENGTH+1];
	for(int i=0; i<m_nNumSprites; i++){
		m_cSprite[i].GetSpriteName(szTempSpriteName);
		if(strcmp(szTempSpriteName, szSpriteName)==0)
			return LetPointer(i+1);
	}
	return NULL;
}