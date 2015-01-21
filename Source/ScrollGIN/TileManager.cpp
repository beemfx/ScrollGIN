/*
	ScrollGIN TileManager.cpp - manager utility for adding, removing,
	changing, and displaying tiles.

	Copyright (c) 2002, Blaine Myers
*/

#include "../ImageLib/ImageLib.h"
#include "../Renderer2/Renderer.h"
#include "../Renderer2/RendererImage.h"
#include "TileManager.h"


//Constructor: Sets some default values
SgTileManager::SgTileManager(void)
: m_iCurrentReference( 0 )
{

}




//Destructor
SgTileManager::~SgTileManager(void)
{

}

//creates many tile surfaces starting with chosen reference from a library
bool SgTileManager::LoadLib(const char* lpLibraryFilename, SgMap *map){
	//This will use a library to call CreateTilesFromFile functions
	CImageArchive ILibrary;

	bool Res = ILibrary.LoadArchive(lpLibraryFilename);

	if(!(Res))return false;
	
	
	HBITMAP hBitmap=0;
	IMAGEDATA id;
	ZeroMemory(&id, sizeof(IMAGEDATA));

	int i=0;
	DWORD j=0;
	int Start = m_iCurrentReference+1;
	for(i=Start, j=1; j<=ILibrary.GetNumEntries(); i++, j++){
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
			}

		}
		m_iCurrentReference++;
	}
	
	ILibrary.CloseArchive();

	return true;
}

//Clears all tiles and releases surfaces from database
void SgTileManager::Clear()
{
	for(int i=0;i<m_iCurrentReference;i++)
	{
		Renderer_DestroySprite( m_lpTile[i] );
		m_lpTile[i] = 0;
	}

	m_iCurrentReference=0;
}


//places a tile on x, y coordinate of the screen
//remember to convert custom coords to standard before calling this
//function is complete, but has not yet been tested.
void SgTileManager::Draw(int reference, int x, int y)
{
	//make sure reference is within range
	if((reference<1)||(reference>m_iCurrentReference))return;
	
	m_lpTile[reference-1]->Draw(x, y);
}
