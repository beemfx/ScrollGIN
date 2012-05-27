/*
	tilemanager.h - header for Scrolling CSTileManager class

	Copyright (c) 2002, Blaine Myers
*/
#ifndef __TILEMANAGER_H__
#define __TILEMANAGER_H__

#include <stdio.h>
#include <tchar.h>
#include "defines.h"
#include "../GFXG7/gfxg7.h"
#include "../MapBoard/mapboard.h"

class CSTileManager{
private:
	//member varialbes
	USHORT m_iCurrentReference;
	CImage m_lpTile[MAX_TILESURFACES];
									
public:
	//Constructor and destructor
	CSTileManager(void);
	~CSTileManager(void);
	///////////////////////////////////////////////////////////////
	///The following public member functions are used to add    ///
	///tiles to the database.                                   ///
	///////////////////////////////////////////////////////////////
	void Release();
	HRESULT CreateTileFromFile(LPVOID lpDevice, DWORD dwTransparent, TCHAR lpBitmapFilename[MAX_PATH], 
								int x, int y, int width, int height, CMapBoard *map);
								//creates a tile from a file in the next available reference.
	HRESULT CreateTileFromFile(LPVOID lpDevice, DWORD dwTransparent, int reference,
								TCHAR lpBitmapFilename[MAX_PATH],
								int x, int y, int width, int height, CMapBoard *map);
								//creates a tile from a file in the chosen reference.
	HRESULT CreateTileFromColor(LPVOID lpDevice, DWORD dwTransparent, COLORREF crefTileColor, CMapBoard *map); //creates a tile from a color in next available reference
	HRESULT CreateTileFromColor(LPVOID lpDevice, DWORD dwTransparent, int reference,
								COLORREF crefTileColor, CMapBoard *map);
								//creates a tile from a color in the chosen reference
	HRESULT CreateTilesFromLibrary(LPVOID lpDevice, DWORD dwTransparent, TCHAR lpLibraryFilename[MAX_PATH], CMapBoard *map);  
									//creates a set of tiles from a library, starting with first reference
	HRESULT CreateTilesFromLibrary(LPVOID lpDevice, DWORD dwTransparent, int reference, TCHAR lpLibraryFilename[MAX_PATH], CMapBoard *map);
									//creates a set of tiles from a libraray, starting with the chosen reference.
	
	////////////////////////////////////////////////////
	//The following are to manage the tile database.////
	////////////////////////////////////////////////////
	HRESULT ClearTileDatabase(void);  //Clears the tile database, resets CurrentReference to 0
	HRESULT Restore(void); //Restores sufaces
	HRESULT ReloadImages(void); //Reloads Images into surfaces
	
	/////////////////////////////////////////////
	//The following write images to a surface.///
	/////////////////////////////////////////////
	HRESULT PlaceTile(int reference, LPVOID dest, int x, int y);
};

#endif //TileManager.h