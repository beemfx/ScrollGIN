/*
	ScrollGIN TileManager.cpp - manager utility for adding, removing,
	changing, and displaying tiles.

	Copyright (c) 2002, Blaine Myers
*/

#include "error.h"
#include "../ImageLib/ImageLib.h"
#include "../Renderer2/Renderer.h"
#include "../Renderer2/RendererImage.h"
#include "TileManager.h"


//Constructor: Sets some default values
CSTileManager::CSTileManager(void){
	m_iCurrentReference=0;
}




//Destructor
CSTileManager::~CSTileManager(void){

}

//creates many tile surfaces starting with chosen reference from a library
HRESULT CSTileManager::CreateTilesFromLibrary(int reference, char lpLibraryFilename[MAX_PATH], CMapBoard *map){
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

				sgRendererImageCreateParms Parms;
				memset( &Parms , 0 , sizeof(Parms) );
				Parms.Type = RENDERER_IMAGE_BITMAP;
				Parms.Bitmap = hBitmap;
				Parms.BmX = id.nX;
				Parms.BmY = id.nY;
				Parms.BmWidth = id.nWidthSrc;
				Parms.BmHeight = id.nHeightSrc;
				Parms.Width  = map->GetTileDim();
				Parms.Height = map->GetTileDim();
				m_lpTile[i-1] = Renderer_CreateSprite( &Parms );

			}else SetError(TEXT("Attempted to create a tile that wasn't in ILB!\n"));

		}
		m_iCurrentReference++;
	}
	
	ILibrary.CloseArchive();

	return S_OK;
}





//creates many tile surfaces starting with first reference from a library
HRESULT CSTileManager::CreateTilesFromLibrary(char lpLibraryFilename[MAX_PATH], CMapBoard *map){
	if(SUCCEEDED(CreateTilesFromLibrary(m_iCurrentReference+1, lpLibraryFilename, map))){
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
		Renderer_DestroySprite( m_lpTile[i] );
	}
}


//places a tile on x, y coordinate of the screen
//remember to convert custom coords to standard before calling this
//function is complete, but has not yet been tested.
HRESULT CSTileManager::PlaceTile(int reference, int x, int y){
	//make sure reference is within range
	if((reference<1)||(reference>m_iCurrentReference))return E_FAIL;
	
	m_lpTile[reference-1]->Draw(x, y);
	return S_OK;
}