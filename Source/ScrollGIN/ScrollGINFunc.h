/*
	ScrollGINFunc.h - Header for ScrollGIN functionality.

	Copyright (c) 2003 Blaine Myers
*/

#ifndef __SCROLLGINFUNC_H__
#define __SCROLLGINFUNC_H__
#include "view.h"
#include "tilemanager.h"
#include "background.h"
#include "../MapBoard/mapboard.h"

//DrawMapBoard() draws the backgrounds and the mapboard as they should appear
BOOL DrawMapBoard(
	LPVOID lpBuffer, 
	CViewPort * pViewport, 
	CMapBoard * pMap, 
	CSTileManager * pTileManager,
	CBackground * pBG);

BOOL LoadMapBoard(
	LPTSTR szFilename, 
	LPVOID lpDD,
	DWORD dwTransparentColor,
	DWORD dwScreenWidth,
	DWORD dwScreenHeight,
	CMapBoard * pMap, 
	CBackground * pBG, 
	CSTileManager * pTileMgr,
	CViewPort * pView);

#endif //__SCROLLGINFUNC_H__