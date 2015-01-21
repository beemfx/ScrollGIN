/* ScrollGIN.cpp - The ScrollGIN class.
	Copyright (c) 2004 Blaine Myers */

#include "ScrollGIN.h"

SgScrollGINGame::SgScrollGINGame()
{
	m_lpObjectManager=NULL;
	m_bInitialized=FALSE;
	m_dwWidth=0;
	m_dwHeight=0;
}


SgScrollGINGame::~SgScrollGINGame()
{

}

int SgScrollGINGame::Pause(BOOL bPause)
{
	m_Timer.Pause(bPause);
	return 1;
}
int SgScrollGINGame::TogglePause()
{
	m_Timer.TogglePause();
	return 1;
}
int SgScrollGINGame::IsPaused()
{
	return m_Timer.IsPaused();
}

int SgScrollGINGame::Render()
{
	DrawMapBoard(
		&m_Viewport,
		&m_Mapboard,
		&m_TileManager,
		&m_Background);

	
	if(m_lpObjectManager)
		m_lpObjectManager->Animate(
			&m_Mapboard,
			&m_Viewport,
			&m_Input);

	return 1;
}

int SgScrollGINGame::GameInit(
	DWORD dwWidth, 
	DWORD dwHeight, 
	SgObjectManager* lpObjMan, 
	HWND hwnd)
{
	if(m_bInitialized)
		return 0;

	if(lpObjMan==NULL)
		return 0;


	m_lpObjectManager=lpObjMan;

	m_Viewport.SetViewportDimensions(dwWidth, dwHeight);
	m_Viewport.set_scroll_speed(0, 0);
	m_Input.CreateDevices(hwnd, CD_KEYBOARD|CD_JOYSTICK);
	
	m_Timer.Start();

	m_bInitialized=TRUE;
	m_dwWidth=dwWidth;
	m_dwHeight=dwHeight;

	lpObjMan->ObtainTimer(&m_Timer);
	lpObjMan->Initialize();
	return 1;
}

int SgScrollGINGame::LoadMap(LPTSTR szFilename)
{
	return LoadMapBoard(
		szFilename,
		m_dwWidth,
		m_dwHeight,
		&m_Mapboard,
		&m_Background,
		&m_TileManager,
		&m_Viewport);
}

int SgScrollGINGame::Release()
{
	m_TileManager.Release();
	m_Background.Release();
	if(m_lpObjectManager)
		m_lpObjectManager->Release();

	return 1;
}

int SgScrollGINGame::PreRenderProcess()
{
	m_Input.UpdateInputValues();
	
	return 1;
}

int SgScrollGINGame::IsKeyPressed(int nKey)
{
	return m_Input.GetKeyState(nKey);
}

int SgScrollGINGame::Shutdown()
{
	if(m_lpObjectManager)
		m_lpObjectManager->ClearObjects();
	m_lpObjectManager = 0;
	return 1;
}

BOOL SgScrollGINGame::DrawMapBoard(
	SgViewPort * pViewport, 
	CMapBoard * pMap, 
	SgTileManager * pTileManager,
	SgBackground * pBG)
{

	//draw the background
	pBG->DrawBackgrounds(
		pViewport->GetScreenXPos()-640/2, 
		pViewport->GetScreenYPos()-480/2,
		640,
		480);

	//draw the mapboard

	//The next few lines determine where the drawing should
	//begin and end to avoid drawing tiles that arean't on the the
	//screen (this actually only slightly reduces rendering time, but
	//we do it anyway).
	int nXStart=0, nYStart=0, nXEnd=0, nYEnd=0;
	
	nXStart=(pViewport->GetScreenXPos()-640/2)/pMap->GetTileDim() + 1;
	nYStart=(pViewport->GetScreenYPos()-480/2)/pMap->GetTileDim() + 1;

	nXEnd=(pViewport->GetScreenXPos()+640/2)/pMap->GetTileDim() + 1;//map->GetMapWidth();
	nYEnd=(pViewport->GetScreenYPos()+480/2)/pMap->GetTileDim() + 1;//map->GetMapHeight();

	for(int x=nXStart; x<=nXEnd; x++){
		for(int y=nYStart; y<=nYEnd; y++){
			pTileManager->PlaceTile(
				pMap->GetTile(x, y), 
				pViewport->screenX(x*pMap->GetTileDim()-pMap->GetTileDim()), 
				pViewport->screenY(y*pMap->GetTileDim()-pMap->GetTileDim()));
		}
	}
	return TRUE;
}

BOOL SgScrollGINGame::LoadMapBoard(
	LPTSTR szFilename,
	DWORD dwScreenWidth,
	DWORD dwScreenHeight,
	CMapBoard * pMap,
	SgBackground * pBG,
	SgTileManager * pTileMgr,
	SgViewPort * pView)
{
	char szLibraryName[MAX_PATH];
	char szBGName[MAX_PATH];
	BOOL bResult=TRUE;
	if(FAILED(pMap->LoadMap(szFilename)))return FALSE;
	pMap->GetLibraryName(szLibraryName);
	
	pView->set_world_dimensions(pMap->GetMapWidth()*pMap->GetTileDim(), pMap->GetMapHeight()*pMap->GetTileDim());
	pView->force_position(0, 0);
	
	//get library and load it
	pTileMgr->ClearTileDatabase();
	if(FAILED(pTileMgr->CreateTilesFromLibrary(szLibraryName, pMap)))bResult=FALSE;

	//get background and load it
	pMap->GetBGName(szBGName);
	pBG->LoadBackgroundImage(szBGName, 2, dwScreenWidth, dwScreenHeight);

	return bResult;
}