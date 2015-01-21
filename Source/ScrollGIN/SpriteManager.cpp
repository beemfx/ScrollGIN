/*
	Spritemanager.cpp - The sprite  manager class

	Copyright (c) 2002, Blaine Myers
*/
/*
	The sprite manager class stores and array of sprites and provides the
	ability to do certain things with them.  One of the most important things being
	the ability to release a pointer, for other objects to use.
*/

#include "../ImageLib/ImageLib.h"
#include "spritemanager.h"

SgSpriteManager::SgSpriteManager():
m_dwMaxSprites(MAX_SPRITES)
{
	m_nNumSprites=0;
}

SgSpriteManager::~SgSpriteManager()
{

}

void SgSpriteManager::LoadLib(const char* szFilename)
{
	if(m_nNumSprites>=MAX_SPRITES)return;
	InsertSprites(m_nNumSprites+1, szFilename);
}

void SgSpriteManager::InsertSprites(int nSprite, const char* szFilename)
{
	if((nSprite<1) || (nSprite>=MAX_SPRITES))return;
	m_nNumSprites=nSprite;
	CImageArchive ILibrary;

	bool Res = ILibrary.LoadArchive(szFilename);
	if(!(Res))return;
	//The following algorithm loads all sprites from library
	DWORD i=0, j=0, nFrames=0;
	HBITMAP hBitmap=0;
	IMAGEDATA id;
	char szImageName[IMAGE_NAME_LENGTH];
	ZeroMemory(&id, sizeof(IMAGEDATA));

	//loop for each of the entries
	for(i=1; i<=ILibrary.GetNumEntries(); i++){
		
		ILibrary.GetImageData(i, &id);
		ILibrary.GetImageName(szImageName, countof(szImageName), i);
	
		if(id.nFrames==1)
		{
			hBitmap=ILibrary.GetBitmap(id.nBitmap);

			m_cSprite[m_nNumSprites-1].CreateSprite(
				hBitmap,
				id.nWidth,
				id.nHeight,
				id.nX,
				id.nY,
				id.nWidthSrc,
				id.nHeightSrc);
			m_cSprite[m_nNumSprites-1].NameSprite(szImageName);

			m_nNumSprites++;
		}
		else if(id.nFrames==0)
		{
			//do nothing this image does not need to be added
		}
		else if(id.nFrames>1)
		{
			nFrames=id.nFrames;
			for(j=1; j<=nFrames; j++){
				ILibrary.GetImageData(j+i-1, &id);
				hBitmap=ILibrary.GetBitmap(id.nBitmap);

				m_cSprite[m_nNumSprites-1].CreateSprite(
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
}

void SgSpriteManager::Clear()
{
	for(int i=0; i<m_nNumSprites; i++)
	{
		m_cSprite[i].Destroy();
	}
	m_nNumSprites=0;
}

SgSprite* SgSpriteManager::GetSprite(const char* szSpriteName)
{
	char szTempSpriteName[MAX_SPRITE_NAME_LENGTH+1];
	for(int i=0; i<m_nNumSprites; i++){
		m_cSprite[i].GetSpriteName(szTempSpriteName,countof(szTempSpriteName));
		if(strcmp(szTempSpriteName, szSpriteName)==0)
			return &m_cSprite[i];
	}
	return 0;
}