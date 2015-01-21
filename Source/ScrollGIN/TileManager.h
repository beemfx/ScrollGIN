/*
	tilemanager.h - header for Scrolling SgTileManager class

	Copyright (c) 2002, Blaine Myers
*/
#ifndef __TILEMANAGER_H__
#define __TILEMANAGER_H__

#include <stdio.h>
#include "defines.h"
#include "../Renderer2/Renderer.h"
#include "../MapBoard/mapboard.h"

class SgTileManager{
private:
	//member varialbes
	USHORT m_iCurrentReference;
	SgRendererImage* m_lpTile[MAX_TILESURFACES];
									
public:
	//Constructor and destructor
	SgTileManager(void);
	~SgTileManager(void);
	///////////////////////////////////////////////////////////////
	///The following public member functions are used to add    ///
	///tiles to the database.                                   ///
	///////////////////////////////////////////////////////////////
	void Release();
	HRESULT CreateTilesFromLibrary(char lpLibraryFilename[MAX_PATH], CMapBoard *map);  
									//creates a set of tiles from a library, starting with first reference
	HRESULT CreateTilesFromLibrary(int reference, char lpLibraryFilename[MAX_PATH], CMapBoard *map);
									//creates a set of tiles from a libraray, starting with the chosen reference.
	
	////////////////////////////////////////////////////
	//The following are to manage the tile database.////
	////////////////////////////////////////////////////
	HRESULT ClearTileDatabase(void);  //Clears the tile database, resets CurrentReference to 0
	
	/////////////////////////////////////////////
	//The following write images to a surface.///
	/////////////////////////////////////////////
	HRESULT PlaceTile(int reference, int x, int y);
};

#endif //TileManager.h