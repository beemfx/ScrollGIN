/*
	Background.h - Header for background class

	Copyright (c) 2002, Blaine Myers
*/

#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include "defines.h"
#include "../Renderer2/Renderer.h"
#include <windows.h>

typedef enum tagSCROLLMODE
{
	FOUR_DIRECTIONS=0, 
	LEFT_RIGHT, 
	UP_DOWN, 
	NO_SCROLL
}SCROLLMODE;

class CBackground{
private:
	SgRendererImage* m_cBackgroundImage[MAX_BACKGROUNDS]; //background images
	int m_nScrollRatio[MAX_BACKGROUNDS]; //ratio for each image
	int m_nCurrentImage; //how many images there are
	SCROLLMODE m_nScrollMode; //the current scroll mode
public:
	CBackground(); //constructor
	~CBackground(); //destructor
	void Release(); //release
	void SetScrollMode(SCROLLMODE nNewMode); //sets current scroll mode
	bool DrawBackgrounds(int x, int y, DWORD dwWidth, DWORD dwHeight); //draw all backgrounds
	HRESULT DisplayBackground(int BGLayer, int x, int y, DWORD dwWidth, DWORD dwHeight); //displays chosen background
	HRESULT LoadBackgroundImage(int nImage, TCHAR szBitmapName[MAX_PATH], int nScrollRatio,DWORD dwDeviceWidth,DWORD dwDeviceHeight); //creates a background layer in given slot
	HRESULT LoadBackgroundImage(TCHAR szBitmapName[MAX_PATH], int nScrollRatio,DWORD dwDeviceWidth,DWORD dwDeviceHeight); //creates background layer in next available slot
	SgRendererImage* LetPointer(int nImage); //relases a pointer to a background image
};

#endif //__background_h__