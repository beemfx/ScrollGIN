/* ScrollGIN.cpp - The ScrollGIN class.
	Copyright (c) 2004 Blaine Myers */

#include "ScrollGIN.h"
#include "ScrollGINFunc.h"

CScrollGINGame::CScrollGINGame()
{
	m_lpObjectManager=NULL;
	m_lpDD=NULL;
	m_bInitialized=FALSE;
	m_dwWidth=0;
	m_dwHeight=0;

	m_dwTransparentColor=0;
}


CScrollGINGame::~CScrollGINGame()
{
	SAFE_DELETE(m_lpObjectManager);
}

int CScrollGINGame::Pause(BOOL bPause)
{
	m_Timer.Pause(bPause);
	return 1;
}
int CScrollGINGame::TogglePause()
{
	m_Timer.TogglePause();
	return 1;
}
int CScrollGINGame::IsPaused()
{
	return m_Timer.IsPaused();
}

int CScrollGINGame::Render(LPVOID lpBuffer)
{
	DrawMapBoard(
		lpBuffer,
		&m_Viewport,
		&m_Mapboard,
		&m_TileManager,
		&m_Background);

	
	if(m_lpObjectManager)
		m_lpObjectManager->Animate(
			lpBuffer,
			&m_Mapboard,
			&m_Viewport,
			&m_Input);

	return 1;
}

int CScrollGINGame::GameInit(
	DWORD dwWidth, 
	DWORD dwHeight, 
	DWORD dwTransparentColor,
	CObjectManager* lpObjMan, 
	HWND hwnd,
	LPVOID lpDirectDraw)
{
	if(m_bInitialized)
		return 0;

	if(lpObjMan==NULL)
		return 0;

	if(lpDirectDraw==NULL)
		return 0;

	m_lpDD=lpDirectDraw;

	m_lpObjectManager=lpObjMan;

	m_Viewport.SetViewportDimensions(dwWidth, dwHeight);
	m_Viewport.set_scroll_speed(0, 0);
	m_Input.CreateDevices(hwnd, CD_KEYBOARD|CD_JOYSTICK);
	
	m_Timer.Start();

	m_bInitialized=TRUE;
	m_dwWidth=dwWidth;
	m_dwHeight=dwHeight;
	m_dwTransparentColor=dwTransparentColor;

	lpObjMan->ObtainTimer(&m_Timer);
	lpObjMan->Initialize(lpDirectDraw, dwTransparentColor);
	return 1;
}

int CScrollGINGame::LoadMap(LPTSTR szFilename)
{
	return LoadMapBoard(
		szFilename,
		m_lpDD,
		m_dwTransparentColor,
		m_dwWidth,
		m_dwHeight,
		&m_Mapboard,
		&m_Background,
		&m_TileManager,
		&m_Viewport);
}

int CScrollGINGame::Release()
{
	m_TileManager.Release();
	m_Background.Release();
	if(m_lpObjectManager)
		m_lpObjectManager->Release();

	return 1;
}
int CScrollGINGame::Restore()
{
	m_TileManager.Restore();
	m_Background.Restore();
	if(m_lpObjectManager)
		m_lpObjectManager->Restore();

	return 1;
}
int CScrollGINGame::ReloadImages()
{
	m_TileManager.ReloadImages();
	m_Background.ReloadImages();
	if(m_lpObjectManager)
		m_lpObjectManager->ReloadImages();

	return 1;
}

int CScrollGINGame::PreRenderProcess()
{
	m_Input.UpdateInputValues();
	
	return 1;
}

int CScrollGINGame::IsKeyPressed(int nKey)
{
	return m_Input.GetKeyState(nKey);
}

int CScrollGINGame::Shutdown()
{
	if(m_lpObjectManager)
		m_lpObjectManager->ClearObjects();

	SAFE_DELETE(m_lpObjectManager);

	return 1;
}