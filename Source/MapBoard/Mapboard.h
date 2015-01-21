/*
	MapBoard Class - Used for reading a ScrollGIN map

	Copyright (c) 2002, Blaine Myers
*/

#ifndef __MAPBOARD_H__
#define __MAPBOARD_H__

static const int MAP_MAX_PATH = 255;
static const int MAP_TILEDIM  = 40;

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
	sg_uint16 wType;				//Map type, *(sg_uint16*)"SM"
	sg_uint16 nVersion;			//Map version, 1 for now
	sg_uint32 lReserved1;			//Reserved1, 0
	sg_uint32 lReserved2;			//Reserved2, 0
	sg_uint16 nMapWidth;			//Width, in tiles, of the map
	sg_uint16 nMapHeight;			//Height, in tiles, of the map
	sg_uint32 lNumTiles;			//Number of tiles in the map, width*height
	sg_uint32 lLibraryNameSize;	//Size, in bytes, of the library filename
	sg_uint32 lBGNameSize;		//Size, in bytes, of the background filename
	sg_uint32 lTileDataSize;		//Size, in bytes, of the tile data
	sg_uint32 lArchDataSize;		//Size, in bytes, of the architecture data
	sg_uint32 lObjectDataSize;	//Size, in bytes, of the object data
};

class CMapBoard
{
protected:
	//Private member variables
	bool      m_bMapLoaded;
	sg_uint8  *m_pTile;	//Visible tile data
	sg_uint8  *m_pArch;	//Unseen architecture data
	sg_uint8  *m_pObject;	//Object data, objects, object generators etc
	sg_uint16 m_nMapWidth;	//Width, in tiles, of the map
	sg_uint16 m_nMapHeight;	//Height, in tiles, of the map
	sg_uint32 m_dwTileDim;  //width height of each tile
	char      m_lpMapFilenameA[MAP_MAX_PATH];		//Filename of current map
	char      m_lpLibraryFilenameA[MAP_MAX_PATH];	//Filename of the library being used
	char      m_lpBGFilenameA[MAP_MAX_PATH];		//Filename of the background image
	//Private member functions
	sg_uint32 CoordToPos(int x, int y); //Converts an x,y value to an array position

public:
	//Public member functions
	CMapBoard();
	~CMapBoard();

	sg_uint16   GetMapWidth();		//Returns m_nMapWidth
	sg_uint16   GetMapHeight();	//Returns m_nMapHeight
	sg_uint32   GetTileDim();
	sg_uint32   SetTileDim(sg_uint32 dwDim);
	sg_uint8    GetTile(int x, int y);	//Returns value of tile at x,y
	sg_uint8    GetArch(int x, int y);	//Returns value of architecture at x,y
	sg_uint8    GetArchType(int x, int y); //layer type (red, green, or blue)
	sg_uint8    GetArchPiece(int x, int y);//get layer arch piece (1-9 or ARCH_SOLID-ARCH_SLOPEDOWNRIGHT)
	sg_uint8    GetObj(int x, int y);	//Returns value of object at x,y
	bool        LoadMap(sg_cpstr lpMapFilename);	//Loads map of filename into memory
	const char* GetLibraryName()const;
	const char* GetBGName()const;
	void        ClearMap();	//Clear all data out of map
};

class CEditMapBoard: public CMapBoard
{
public:
	void     ClearArch();
	void     ClearTile();
	void     ClearObject();
	bool     SetTile(int x, int y, sg_uint8 nNewValue); //Set value of tile at x,y
	bool     SetArch(int x, int y, sg_uint8 nNewValue);  //Set value of architecture at x,y
	bool     SetArchSmart(int x, int y, sg_uint8 nNewValue); //automatically sets arch using 0 as base
	bool     SetObj(int x, int y, sg_uint8 nNewValue); //Set value of object at x,y
	sg_uint8 GetArchSmart(int x, int y);
	bool     SaveMap(sg_cpstr lpMapFilename); //Save the map to disk
	bool     GenerateNewMap(int nWidth, int nHeight, sg_cpstr lpLibFilename, sg_cpstr lpBGFilename); //Start a new map with chosen width, height, and library
	bool     ChangeMapDimensions(int nNewWidth, int nNewHeight); //Changes the map dimensions, preserving tile, arch, and object data
	void     ChangeBackground(sg_cpstr lpBackgroundFilename); //Change the current background
	void     ChangeLibrary(sg_cpstr lpLibraryFilename);
};

#endif //__mapboard_h__