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

void SgScrollGINGame::Pause(bool bPause)
{
	m_Timer.Pause(bPause);
}

void SgScrollGINGame::TogglePause()
{
	m_Timer.TogglePause();
}

bool SgScrollGINGame::IsPaused()const
{
	return TRUE == m_Timer.IsPaused();
}

void SgScrollGINGame::Render()
{
	DrawMapBoard();
	m_lpObjectManager->Draw(&m_Viewport);
}

void SgScrollGINGame::Init( int dwWidth , int dwHeight , SgObjectManager* lpObjMan , HWND hwnd )
{
	if(m_bInitialized)
		return;

	if(lpObjMan==NULL)
		return;


	m_lpObjectManager=lpObjMan;

	m_Viewport.SetViewportDimensions(dwWidth, dwHeight);
	m_Viewport.set_scroll_speed(0, 0);
	m_Input.CreateDevices(hwnd, CD_KEYBOARD|CD_JOYSTICK);
	
	m_Timer.Start();

	m_bInitialized=TRUE;
	m_dwWidth=dwWidth;
	m_dwHeight=dwHeight;

	lpObjMan->SetTimer(&m_Timer);
	lpObjMan->Initialize();
}

void SgScrollGINGame::LoadMap(const char* szFilename)
{
	m_Mapboard.LoadMap(szFilename);
	const char* szLibraryName = m_Mapboard.GetLibraryName();
	
	m_Viewport.set_world_dimensions(m_Mapboard.GetMapWidth()*m_Mapboard.GetTileDim(), m_Mapboard.GetMapHeight()*m_Mapboard.GetTileDim());
	m_Viewport.force_position(0, 0);
	
	//get library and load it
	m_TileManager.Clear();
	m_TileManager.LoadLib(szLibraryName, &m_Mapboard);

	//get background and load it
	const char* szBGName = m_Mapboard.GetBGName();
	m_Background.LoadBackgroundImage(szBGName, 2, m_dwWidth, m_dwHeight);
}

void SgScrollGINGame::Update()
{
	float RawElapsed = m_Timer.GetRawElapsedSec();
	if( RawElapsed < 1.0f/GAME_FPS )
	{
		Sleep(0);
		return;
	}
	m_Timer.Update();
	m_Input.UpdateInputValues();
	m_lpObjectManager->Update(&m_Mapboard,&m_Viewport,&m_Input);
}

bool SgScrollGINGame::IsKeyPressed(int nKey)
{
	return m_Input.GetKeyState(nKey);
}

void SgScrollGINGame::Deinit()
{
	if(m_lpObjectManager)
	{
		m_lpObjectManager->ClearSprites();
		m_lpObjectManager->ClearObjects();
	}

	m_lpObjectManager = 0;

	m_TileManager.Clear();
	m_Background.Destroy();
}

void SgScrollGINGame::DrawMapBoard()
{

	//draw the background
	m_Background.DrawBackgrounds(
		m_Viewport.GetScreenXPos()-VIEW_WIDTH/2, 
		m_Viewport.GetScreenYPos()-VIEW_HEIGHT/2,
		VIEW_WIDTH,
		VIEW_HEIGHT);

	//draw the mapboard

	//The next few lines determine where the drawing should
	//begin and end to avoid drawing tiles that arean't on the the
	//screen (this actually only slightly reduces rendering time, but
	//we do it anyway).
	int nXStart=0, nYStart=0, nXEnd=0, nYEnd=0;
	
	nXStart=(m_Viewport.GetScreenXPos()-VIEW_WIDTH/2)/m_Mapboard.GetTileDim() + 1;
	nYStart=(m_Viewport.GetScreenYPos()-VIEW_HEIGHT/2)/m_Mapboard.GetTileDim() + 1;

	nXEnd=(m_Viewport.GetScreenXPos()+VIEW_WIDTH/2)/m_Mapboard.GetTileDim() + 1;//map->GetMapWidth();
	nYEnd=(m_Viewport.GetScreenYPos()+VIEW_HEIGHT/2)/m_Mapboard.GetTileDim() + 1;//map->GetMapHeight();

	for(int x=nXStart; x<=nXEnd; x++){
		for(int y=nYStart; y<=nYEnd; y++){
			m_TileManager.Draw(
				m_Mapboard.GetTile(x, y), 
				m_Viewport.screenX(x*m_Mapboard.GetTileDim()-m_Mapboard.GetTileDim()), 
				m_Viewport.screenY(y*m_Mapboard.GetTileDim()-m_Mapboard.GetTileDim()));
		}
	}
}
