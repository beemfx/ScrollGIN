/* ScrollGIN.h - Header for ScrollGIN game class.
	Copyright (c) 2004 Blaine Myers. */

#ifndef __SCROLLGIN_H__
#define __SCROLLGIN_H__
#include <windows.h>

#include "ObjMan.h"
#include "TileManager.h"
#include "SpriteManager.h"
#include "View.h"
#include "Input.h"
#include "Timer.h"
#include "Background.h"

class SgScrollGINGame
{
protected:
	//ScrollGIN classes
	SgTileManager m_TileManager;
	SgTimer m_Timer;
	SgViewPort m_Viewport;
	SgInputManager m_Input;
	CMapBoard m_Mapboard;
	SgBackground m_Background;

	SgObjectManager* m_lpObjectManager;

	BOOL m_bInitialized;

	DWORD m_dwWidth;
	DWORD m_dwHeight;
public:
	SgScrollGINGame();
	~SgScrollGINGame();

	int GameInit(
		DWORD dwWidth, 
		DWORD dwHeight,
		SgObjectManager* lpObjMan, 
		HWND hwnd);

	int Shutdown();

	
	int Pause(BOOL bPause);
	int TogglePause();
	int IsPaused();

	int Release();

	int Render();
	int LoadMap(LPTSTR szFilename);

	int PreRenderProcess();

	int IsKeyPressed(int nKey);

	BOOL DrawMapBoard(
		SgViewPort * pViewport, 
		CMapBoard * pMap, 
		SgTileManager * pTileManager,
		SgBackground * pBG);

	BOOL LoadMapBoard(
		LPTSTR szFilename, 
		DWORD dwScreenWidth,
		DWORD dwScreenHeight,
		CMapBoard * pMap, 
		SgBackground * pBG, 
		SgTileManager * pTileMgr,
		SgViewPort * pView);
};

#endif //__SCROLLGIN_H__