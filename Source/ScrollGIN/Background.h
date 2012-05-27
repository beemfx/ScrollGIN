/*
	Background.h - Header for background class

	Copyright (c) 2002, Blaine Myers
*/

#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include "defines.h"
#include "../GFXG7/gfxg7.h"

typedef enum tagSCROLLMODE
{
	FOUR_DIRECTIONS=0, 
	LEFT_RIGHT, 
	UP_DOWN, 
	NO_SCROLL
}SCROLLMODE;

class CBackground{
private:
	CImage m_cBackgroundImage[MAX_BACKGROUNDS]; //background images
	int m_nScrollRatio[MAX_BACKGROUNDS]; //ratio for each image
	int m_nCurrentImage; //how many images there are
	SCROLLMODE m_nScrollMode; //the current scroll mode
public:
	CBackground(); //constructor
	~CBackground(); //destructor
	void Restore(); //restore
	void Release(); //release
	void ReloadImages(); //reload images
	void SetScrollMode(SCROLLMODE nNewMode); //sets current scroll mode
	BOOL DrawBackgrounds(LPVOID lpBuffer, int x, int y, DWORD dwWidth, DWORD dwHeight); //draw all backgrounds
	HRESULT DisplayBackground(int BGLayer, LPVOID lpBuffer, int x, int y, DWORD dwWidth, DWORD dwHeight); //displays chosen background
	HRESULT LoadBackgroundImage(LPVOID lpDevice, DWORD dwTransparent, int nImage, TCHAR szBitmapName[MAX_PATH], int nScrollRatio,DWORD dwDeviceWidth,DWORD dwDeviceHeight); //creates a background layer in given slot
	HRESULT LoadBackgroundImage(LPVOID lpDevice, DWORD dwTransparent, TCHAR szBitmapName[MAX_PATH], int nScrollRatio,DWORD dwDeviceWidth,DWORD dwDeviceHeight); //creates background layer in next available slot
	CImage* LetPointer(int nImage); //relases a pointer to a background image
};

#endif //__background_h__