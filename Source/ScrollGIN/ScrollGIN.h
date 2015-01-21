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
#include "ScrollGINFunc.h"
#include "Timer.h"

class CScrollGINGame
{
protected:
	//ScrollGIN classes
	CSTileManager m_TileManager;
	CTimerEx m_Timer;
	CViewPort m_Viewport;
	CInputManager m_Input;
	CMapBoard m_Mapboard;
	CBackground m_Background;

	CObjectManager* m_lpObjectManager;

	BOOL m_bInitialized;

	DWORD m_dwWidth;
	DWORD m_dwHeight;

	DWORD m_dwTransparentColor;
public:
	CScrollGINGame();
	~CScrollGINGame();

	int GameInit(
		DWORD dwWidth, 
		DWORD dwHeight,
		DWORD dwTransparentColor, 
		CObjectManager* lpObjMan, 
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
};

#endif //__SCROLLGIN_H__