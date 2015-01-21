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
	DrawMapBoard();

	m_lpObjectManager->Animate(&m_Mapboard,&m_Viewport,&m_Input);

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
	char szLibraryName[MAX_PATH];
	char szBGName[MAX_PATH];
	BOOL bResult=TRUE;
	if(FAILED(m_Mapboard.LoadMap(szFilename)))return FALSE;
	m_Mapboard.GetLibraryName(szLibraryName);
	
	m_Viewport.set_world_dimensions(m_Mapboard.GetMapWidth()*m_Mapboard.GetTileDim(), m_Mapboard.GetMapHeight()*m_Mapboard.GetTileDim());
	m_Viewport.force_position(0, 0);
	
	//get library and load it
	m_TileManager.ClearTileDatabase();
	if(FAILED(m_TileManager.CreateTilesFromLibrary(szLibraryName, &m_Mapboard)))bResult=FALSE;

	//get background and load it
	m_Mapboard.GetBGName(szBGName);
	m_Background.LoadBackgroundImage(szBGName, 2, m_dwWidth, m_dwHeight);

	return bResult;
}

int SgScrollGINGame::Release()
{
	m_TileManager.Release();
	m_Background.Release();
	if(m_lpObjectManager)
		m_lpObjectManager->Release();

	return 1;
}

void SgScrollGINGame::Update()
{
	m_Input.UpdateInputValues();
}

bool SgScrollGINGame::IsKeyPressed(int nKey)
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

void SgScrollGINGame::DrawMapBoard()
{

	//draw the background
	m_Background.DrawBackgrounds(
		m_Viewport.GetScreenXPos()-640/2, 
		m_Viewport.GetScreenYPos()-480/2,
		640,
		480);

	//draw the mapboard

	//The next few lines determine where the drawing should
	//begin and end to avoid drawing tiles that arean't on the the
	//screen (this actually only slightly reduces rendering time, but
	//we do it anyway).
	int nXStart=0, nYStart=0, nXEnd=0, nYEnd=0;
	
	nXStart=(m_Viewport.GetScreenXPos()-640/2)/m_Mapboard.GetTileDim() + 1;
	nYStart=(m_Viewport.GetScreenYPos()-480/2)/m_Mapboard.GetTileDim() + 1;

	nXEnd=(m_Viewport.GetScreenXPos()+640/2)/m_Mapboard.GetTileDim() + 1;//map->GetMapWidth();
	nYEnd=(m_Viewport.GetScreenYPos()+480/2)/m_Mapboard.GetTileDim() + 1;//map->GetMapHeight();

	for(int x=nXStart; x<=nXEnd; x++){
		for(int y=nYStart; y<=nYEnd; y++){
			m_TileManager.PlaceTile(
				m_Mapboard.GetTile(x, y), 
				m_Viewport.screenX(x*m_Mapboard.GetTileDim()-m_Mapboard.GetTileDim()), 
				m_Viewport.screenY(y*m_Mapboard.GetTileDim()-m_Mapboard.GetTileDim()));
		}
	}
}
