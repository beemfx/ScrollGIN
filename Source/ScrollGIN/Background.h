/*
	Background.h - Header for background class

	Copyright (c) 2002, Blaine Myers
*/

#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include "defines.h"
#include "../Renderer2/Renderer.h"

enum SCROLLMODE
{
	FOUR_DIRECTIONS, 
	LEFT_RIGHT, 
	UP_DOWN, 
	NO_SCROLL,
};

class SgBackground{
private:
	SgRendererImage* m_cBackgroundImage[MAX_BACKGROUNDS]; //background images
	int m_nScrollRatio[MAX_BACKGROUNDS]; //ratio for each image
	int m_nCurrentImage; //how many images there are
	SCROLLMODE m_nScrollMode; //the current scroll mode
public:
	SgBackground(); //constructor
	~SgBackground(); //destructor
	void Release(); //release
	void SetScrollMode(SCROLLMODE nNewMode); //sets current scroll mode
	void DrawBackgrounds(int x, int y, int dwWidth, int dwHeight); //draw all backgrounds
	void DisplayBackground(int BGLayer, int x, int y, int dwWidth, int dwHeight); //displays chosen background
	bool LoadBackgroundImage(int nImage, const char* szBitmapName, int nScrollRatio,int dwDeviceWidth,int dwDeviceHeight); //creates a background layer in given slot
	bool LoadBackgroundImage(const char* szBitmapName, int nScrollRatio,int dwDeviceWidth,int dwDeviceHeight); //creates background layer in next available slot
	SgRendererImage* LetPointer(int nImage); //relases a pointer to a background image
};

#endif //__background_h__