/*
	ScrollGINFunc.cpp - Functions for ScrollGIN technology.

	Copyright (c) 2003 Blaine Myers
*/

#include "ScrollGINFunc.h"


BOOL DrawMapBoard(
	LPVOID lpBuffer, 
	CViewPort * pViewport, 
	CMapBoard * pMap, 
	CSTileManager * pTileManager,
	CBackground * pBG)
{
	//if the buffer doesn't exist we exit because
	//we can't do anything
	if(!lpBuffer)return FALSE;

	DDSURFACEDESC2 ddsd;
	ddsd.dwSize=sizeof(DDSURFACEDESC2);
	ddsd.dwFlags=DDSD_WIDTH|DDSD_HEIGHT;

	((LPDIRECTDRAWSURFACE7)lpBuffer)->GetSurfaceDesc(&ddsd);

	DWORD nWidth=ddsd.dwWidth;
	DWORD nHeight=ddsd.dwHeight;


	//draw the background
	pBG->DrawBackgrounds(
		lpBuffer, 
		pViewport->GetScreenXPos()-640/2, 
		pViewport->GetScreenYPos()-480/2,
		nWidth,
		nHeight);

	//draw the mapboard

	//The next few lines determine where the drawing should
	//begin and end to avoid drawing tiles that arean't on the the
	//screen (this actually only slightly reduces rendering time, but
	//we do it anyway).
	int nXStart=0, nYStart=0, nXEnd=0, nYEnd=0;
	
	nXStart=(pViewport->GetScreenXPos()-nWidth/2)/pMap->GetTileDim() + 1;
	nYStart=(pViewport->GetScreenYPos()-nHeight/2)/pMap->GetTileDim() + 1;

	nXEnd=(pViewport->GetScreenXPos()+nWidth/2)/pMap->GetTileDim() + 1;//map->GetMapWidth();
	nYEnd=(pViewport->GetScreenYPos()+nHeight/2)/pMap->GetTileDim() + 1;//map->GetMapHeight();

	for(int x=nXStart; x<=nXEnd; x++){
		for(int y=nYStart; y<=nYEnd; y++){
			pTileManager->PlaceTile(
				pMap->GetTile(x, y), 
				lpBuffer, 
				pViewport->screenX(x*pMap->GetTileDim()-pMap->GetTileDim()), 
				pViewport->screenY(y*pMap->GetTileDim()-pMap->GetTileDim()));
		}
	}
	return TRUE;
}

BOOL LoadMapBoard(
	LPTSTR szFilename, 
	LPVOID lpDD,
	DWORD dwTransparentColor,
	DWORD dwScreenWidth,
	DWORD dwScreenHeight,
	CMapBoard * pMap, 
	CBackground * pBG, 
	CSTileManager * pTileMgr,
	CViewPort * pView)
{
	TCHAR szLibraryName[MAX_PATH];
	TCHAR szBGName[MAX_PATH];
	BOOL bResult=TRUE;
	if(FAILED(pMap->LoadMap(szFilename)))return FALSE;
	pMap->GetLibraryName(szLibraryName);
	
	pView->set_world_dimensions(pMap->GetMapWidth()*pMap->GetTileDim(), pMap->GetMapHeight()*pMap->GetTileDim());
	pView->force_position(0, 0);
	
	//get library and load it
	pTileMgr->ClearTileDatabase();
	if(FAILED(pTileMgr->CreateTilesFromLibrary(lpDD, dwTransparentColor, szLibraryName, pMap)))bResult=FALSE;

	//get background and load it
	pMap->GetBGName(szBGName);
	pBG->LoadBackgroundImage(lpDD, dwTransparentColor, szBGName, 2, dwScreenWidth, dwScreenHeight);

	return bResult;
}