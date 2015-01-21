/*
	MapBoard Class - Used for reading a ScrollGIN map

	Coyright (c) 2002, Blaine Myers
*/

#ifndef __MAPBOARD_H__
#define __MAPBOARD_H__

#include <windows.h>

//Note on architecture type:
//the first four bits represent the layer indirect (either blue, red, or green in the editor)
//the last four bits represent the actual value
enum ARCHTYPE
{
	ARC_SPACE=0x00, 
				
	ARC_SOLID=0x01, 
	ARC_SOLIDTOP, ARC_SOLIDBOTTOM, ARC_SOLIDLEFT, ARC_SOLIDRIGHT,
	ARC_SLOPEUPRIGHT, ARC_SLOPEUPLEFT, ARC_SLOPEDOWNLEFT, ARC_SLOPEDOWNRIGHT,
				
	ARC_SOLID1=0x11, 
	ARC_SOLIDTOP1, ARC_SOLIDBOTTOM1, ARC_SOLIDLEFT1, ARC_SOLIDRIGHT1,
	ARC_SLOPEUPRIGHT1, ARC_SLOPEUPLEFT1, ARC_SLOPEDOWNLEFT1, ARC_SLOPEDOWNRIGHT1,
				
	ARC_SOLID2=0x21, 
	ARC_SOLIDTOP2, ARC_SOLIDBOTTOM2, ARC_SOLIDLEFT2, ARC_SOLIDRIGHT2,
	ARC_SLOPEUPRIGHT2, ARC_SLOPEUPLEFT2, ARC_SLOPEDOWNLEFT2, ARC_SLOPEDOWNRIGHT2,
				
	ARC_SOLID3=0x31, 
	ARC_SOLIDTOP3, ARC_SOLIDBOTTOM3, ARC_SOLIDLEFT3, ARC_SOLIDRIGHT3,
	ARC_SLOPEUPRIGHT3, ARC_SLOPEUPLEFT3, ARC_SLOPEDOWNLEFT3, ARC_SLOPEDOWNRIGHT3
};

enum LAYER
{
	LAYER_TILE=0, 
	LAYER_ARCH, 
	LAYER_OBJECT, 
	LAYER_ERROR
};

struct MAPHEADER
{
	WORD		wType;				//Map type, *(WORD*)"SM"
	USHORT	nVersion;			//Map version, 1 for now

	ULONG		lReserved1;			//Reserved1, 0
	ULONG		lReserved2;			//Reserved2, 0

	USHORT	nMapWidth;			//Width, in tiles, of the map
	USHORT	nMapHeight;			//Height, in tiles, of the map
	ULONG		lNumTiles;			//Number of tiles in the map, width*height

	ULONG		lLibraryNameSize;	//Size, in bytes, of the library filename
	ULONG		lBGNameSize;		//Size, in bytes, of the background filename

	ULONG		lTileDataSize;		//Size, in bytes, of the tile data
	ULONG		lArchDataSize;		//Size, in bytes, of the architecture data
	ULONG		lObjectDataSize;	//Size, in bytes, of the object data
};

class CMapBoard
{
protected:
	//Private member variables
	BOOL	m_bMapLoaded;
	BYTE	*m_pTile;	//Visible tile data
	BYTE	*m_pArch;	//Unseen architecture data
	BYTE	*m_pObject;	//Object data, objects, object generators etc
	USHORT	m_nMapWidth;	//Width, in tiles, of the map
	USHORT	m_nMapHeight;	//Height, in tiles, of the map
	DWORD   m_dwTileDim;  //width height of each tile
	char    m_lpMapFilenameA[MAX_PATH];		//Filename of current map
	char    m_lpLibraryFilenameA[MAX_PATH];	//Filename of the library being used
	char    m_lpBGFilenameA[MAX_PATH];		//Filename of the background image

	//Private member functions
	ULONG CoordToPos(int x, int y); //Converts an x,y value to an array position

public:
	//Public member functions
	CMapBoard();
	~CMapBoard();

	USHORT      GetMapWidth();		//Returns m_nMapWidth
	USHORT      GetMapHeight();	//Returns m_nMapHeight
	DWORD       GetTileDim();
	DWORD       SetTileDim(DWORD dwDim);
	BYTE        GetTile(int x, int y);	//Returns value of tile at x,y
	BYTE        GetArch(int x, int y);	//Returns value of architecture at x,y
	BYTE        GetArchType(int x, int y); //layer type (red, green, or blue)
	BYTE        GetArchPiece(int x, int y);//get layer arch piece (1-9 or ARCH_SOLID-ARCH_SLOPEDOWNRIGHT)
	BYTE        GetObject(int x, int y);	//Returns value of object at x,y
	HRESULT     LoadMap(LPCSTR lpMapFilename);	//Loads map of filename into memory
	const char* GetLibraryName()const;
	const char* GetBGName()const;
	void        ClearMap();	//Clear all data out of map
};

class CEditMapBoard: public CMapBoard
{
public:
	HRESULT ClearArch();
	HRESULT ClearTile();
	HRESULT ClearObject();
	HRESULT SetTile(int x, int y, BYTE nNewValue); //Set value of tile at x,y
	HRESULT SetArch(int x, int y, BYTE nNewValue);  //Set value of architecture at x,y
	HRESULT SetArchSmart(int x, int y, BYTE nNewValue); //automatically sets arch using 0 as base
	HRESULT SetObject(int x, int y, BYTE nNewValue); //Set value of object at x,y
	BYTE    GetArchSmart(int x, int y);
	HRESULT SaveMap(LPSTR lpMapFilename); //Save the map to disk
	HRESULT GenerateNewMap(int nWidth, int nHeight, LPSTR lpLibFilename, LPSTR lpBGFilename); //Start a new map with chosen width, height, and library
	HRESULT ChangeMapDimensions(int nNewWidth, int nNewHeight); //Changes the map dimensions, preserving tile, arch, and object data
	void    ChangeBackground(LPCSTR lpBackgroundFilename); //Change the current background
	void    ChangeLibrary(LPCSTR lpLibraryFilename);
};

#endif //__mapboard_h__