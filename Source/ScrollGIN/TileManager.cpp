/*
	ScrollGIN TileManager.cpp - manager utility for adding, removing,
	changing, and displaying tiles.

	Copyright (c) 2002, Blaine Myers
*/

#include "error.h"
#include "../ImageLib/ImageLib.h"
#include "TileManager.h"


//Constructor: Sets some default values
CSTileManager::CSTileManager(void){
	m_iCurrentReference=0;
}




//Destructor
CSTileManager::~CSTileManager(void){

}


//Creates a tile surface from file in next available reference
HRESULT CSTileManager::CreateTileFromFile(LPVOID lpDevice, DWORD dwTransparent, TCHAR lpBitmapFilename[MAX_PATH], int x, int y, int width, int height, CMapBoard *map){
	if(SUCCEEDED(CreateTileFromFile(lpDevice, dwTransparent, m_iCurrentReference+1, lpBitmapFilename, x, y, width, height, map))){
		m_iCurrentReference++; //we add after call to function in case it doesn't succeed
		return S_OK;
	}else return E_FAIL;
}




//creates a tile surface from a file in chosen reference
HRESULT CSTileManager::CreateTileFromFile(LPVOID lpDevice, DWORD dwTransparent, int reference, TCHAR lpBitmapFilename[MAX_PATH], int x, int y, int width, int height, CMapBoard *map){	
	
	if(FAILED(m_lpTile[reference-1].CreateImageBM(
		lpDevice,
		dwTransparent,
		lpBitmapFilename, 
		x, 
		y, 
		width, 
		height, 
		map->GetTileDim(), 
		map->GetTileDim(), 
		NULL)))return E_FAIL;
	return S_OK;
}




//Creates a tile surface from a color
HRESULT CSTileManager::CreateTileFromColor(LPVOID lpDevice, DWORD dwTransparent, COLORREF crefTileColor, CMapBoard *map){
	if(SUCCEEDED(CreateTileFromColor(lpDevice, dwTransparent, m_iCurrentReference+1, crefTileColor, map))){
		m_iCurrentReference++; //we add after call to function in case it doesn't succeed
		return S_OK;
	}else return E_FAIL;
}



//creates a tile surface from color in chosen reference
HRESULT CSTileManager::CreateTileFromColor(LPVOID lpDevice, DWORD dwTransparent, int reference, COLORREF crefTileColor, CMapBoard *map){
	
	if(FAILED(m_lpTile[reference-1].CreateImageColor(lpDevice, dwTransparent, crefTileColor, map->GetTileDim(), map->GetTileDim())))return E_FAIL;

	return S_OK;
}




//creates many tile surfaces starting with chosen reference from a library
HRESULT CSTileManager::CreateTilesFromLibrary(LPVOID lpDevice, DWORD dwTransparent, int reference, TCHAR lpLibraryFilename[MAX_PATH], CMapBoard *map){
	//This will use a library to call CreateTilesFromFile functions
	CImageArchive ILibrary;

	HRESULT hr=0;
	hr=ILibrary.LoadArchive(lpLibraryFilename);

	if(FAILED(hr))return E_FAIL;
	
	
	HBITMAP hBitmap=0;
	IMAGEDATA id;
	ZeroMemory(&id, sizeof(IMAGEDATA));

	int i=0;
	DWORD j=0;
	for(i=reference, j=1; j<=ILibrary.GetNumEntries(); i++, j++){
		if( (i>0) && (i<MAX_TILESURFACES)){
			if(ILibrary.GetImageData(j, &id)){

				hBitmap=ILibrary.GetBitmap(id.nBitmap);

				m_lpTile[i-1].CreateImageBMInMemory(
					lpDevice,
					dwTransparent,
					hBitmap, 
					id.nX, 
					id.nY,
					id.nWidthSrc, 
					id.nHeightSrc, 
					map->GetTileDim(), 
					map->GetTileDim(), 
					NULL);
			}else SetError(TEXT("Attempted to create a tile that wasn't in ILB!\n"));

		}
		m_iCurrentReference++;
	}
	
	ILibrary.CloseArchive();

	return S_OK;
}





//creates many tile surfaces starting with first reference from a library
HRESULT CSTileManager::CreateTilesFromLibrary(LPVOID lpDevice, DWORD dwTransparent, TCHAR lpLibraryFilename[MAX_PATH], CMapBoard *map){
	if(SUCCEEDED(CreateTilesFromLibrary(lpDevice, dwTransparent, m_iCurrentReference+1, lpLibraryFilename, map))){
		return S_OK;
	}else return E_FAIL;
}





//Clears all tiles and releases surfaces from database
HRESULT CSTileManager::ClearTileDatabase(void){
	Release();
	m_iCurrentReference=0;
	return S_OK;
}

void CSTileManager::Release(){
	for(int i=0;i<m_iCurrentReference;i++){
		m_lpTile[i].Release();
	}
}





//restores tile surfaces
HRESULT CSTileManager::Restore(void){
	for(int i=0;i<m_iCurrentReference;i++){
		m_lpTile[i].Restore();
	}
	return S_OK;
}





//reloads images into tile surfaces
HRESULT CSTileManager::ReloadImages(void){
	HRESULT hr=S_OK;

	//code should look something like this
	for(int i=0; i<m_iCurrentReference;i++){
		m_lpTile[i].ReloadImageIntoSurface();
	}
	return hr;
}



//places a tile on x, y coordinate of the screen
//remember to convert custom coords to standard before calling this
//function is complete, but has not yet been tested.
HRESULT CSTileManager::PlaceTile(int reference, LPVOID dest, int x, int y){
	//make sure reference is within range
	if((reference<1)||(reference>m_iCurrentReference))return E_FAIL;
	//make sure surfaces exist
	if(!dest)return E_FAIL;

	//make sure within screen, because if it isn't why draw it
	/*
	if(x<-TILEDIMENSION||x>g_nDeviceWidth||
		y<-TILEDIMENSION||y>g_nDeviceHeight)return S_FALSE;
	*/
	
	return m_lpTile[reference-1].DrawPrefered(x, y);
}