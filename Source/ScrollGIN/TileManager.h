/*
	tilemanager.h - header for Scrolling SgTileManager class

	Copyright (c) 2002, Blaine Myers
*/
#ifndef __TILEMANAGER_H__
#define __TILEMANAGER_H__

#include "GameConfig.h"
#include "../Renderer2/Renderer.h"
#include "Mapboard/SgMap.h"

class SgTileManager
{
private:
	int              m_iCurrentReference;
	SgRendererImage* m_lpTile[MAX_TILESURFACES];
									
public:
	//Constructor and destructor
	SgTileManager(void);
	~SgTileManager(void);

	bool LoadLib(const char* lpLibraryFilename, SgMap* map);  
	void Clear(void);

	void Draw(int reference, int x, int y);
};

#endif //TileManager.h