/*
	Mapboard.cpp - Functions for CMapBoard class.

	Copyright (c) 2002, Blaine Myers
*/
#include "defines.h"
#include "MapBoard.h"
#include <windows.h>

CMapBoard::CMapBoard()
{
	m_pTile=new BYTE;
	m_pArch=new BYTE;
	m_pObject=new BYTE;

	m_nMapWidth=m_nMapHeight=1;
	m_dwTileDim=TILEDIM;
}

sg_uint32 CMapBoard::GetTileDim()
{
	return m_dwTileDim;
}

sg_uint32 CMapBoard::SetTileDim(sg_uint32 dwDim)
{
	sg_uint32 dwTemp;
	dwTemp=m_dwTileDim;
	m_dwTileDim=dwDim;
	return dwTemp;
}

BYTE CMapBoard::GetArchType(int x, int y)
{
	if(x<1||x>m_nMapWidth)return 0x00;
	if(y<1||y>m_nMapHeight)return 0x00;
	
	BYTE nByte=m_pArch[CoordToPos(x, y)];
	nByte=nByte&0xf0;
	nByte=nByte>>4;
	return nByte;
}

BYTE CMapBoard::GetArchPiece(int x, int y)
{ 
	if(x<1||x>m_nMapWidth)return 0x00;
	if(y<1||y>m_nMapHeight)return 0x00;

	BYTE nByte=m_pArch[CoordToPos(x, y)];
	nByte=nByte&0x0f;
	return (ARCHTYPE)nByte;
}

CMapBoard::~CMapBoard()
{
	SAFE_DELETE_ARRAY(m_pTile);
	SAFE_DELETE_ARRAY(m_pArch);
	SAFE_DELETE_ARRAY(m_pObject);
}

const char* CMapBoard::GetLibraryName()const
{
	return m_lpLibraryFilenameA;
}


const char* CMapBoard::GetBGName()const
{
	return m_lpBGFilenameA;
}

sg_uint16 CMapBoard::GetMapWidth(){
	return m_nMapWidth;
}

sg_uint16 CMapBoard::GetMapHeight(){
	return m_nMapHeight;
}

sg_uint8 CMapBoard::GetTile(int x, int y){
	if(x<1||x>m_nMapWidth)return 0;
	if(y<1||y>m_nMapHeight)return 0;
	return m_pTile[CoordToPos(x, y)];
}

sg_uint8 CMapBoard::GetArch(int x, int y){
	if(x<1||x>m_nMapWidth)return 0;
	if(y<1||y>m_nMapHeight)return 0;
	return m_pArch[CoordToPos(x, y)];
}

sg_uint8 CMapBoard::GetObj(int x, int y){
	if(x<1||x>m_nMapWidth)return 0;
	if(y<1||y>m_nMapHeight)return 0;
	return m_pObject[CoordToPos(x, y)];
}

sg_uint32 CMapBoard::CoordToPos(int x, int y){
	if(x<1||x>m_nMapWidth)return 0;
	if(y<1||y>m_nMapHeight)return 0;
	return y+m_nMapHeight*(x-1)-1;
}

void CMapBoard::ClearMap(){
	//Delete the map data
	SAFE_DELETE_ARRAY(m_pTile);
	SAFE_DELETE_ARRAY(m_pArch);
	SAFE_DELETE_ARRAY(m_pObject);
	
	//Set dimensions to 0
	m_nMapWidth=m_nMapHeight=0;

	//nullify the filenames
	m_lpMapFilenameA[0]=m_lpLibraryFilenameA[0]=m_lpBGFilenameA[0]=NULL;
}

bool CMapBoard::LoadMap(sg_cpstr lpMapFilename)
{
	HANDLE hFile;
	//Open the file
	hFile=CreateFileA(lpMapFilename, GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL,
							OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
	if(hFile==INVALID_HANDLE_VALUE)return false;
	
	DWORD dwBytesRead;
	//Read Header First
	MAPHEADER sMapHeader;
	
	if(!ReadFile(hFile, &sMapHeader, sizeof(sMapHeader), &dwBytesRead, NULL)){
		CloseHandle(hFile);return false;
	}

	//Check for valid file
	if(sMapHeader.wType!=*(sg_uint16*)"SM"){
		CloseHandle(hFile);return false;
	}

	if(sMapHeader.nVersion!=1){
		CloseHandle(hFile);return false;
	}
	
	//We will now assume that it is a valid file, which may not necessarily be true
	
	//In case there is a current map loaded lets clear it
	ClearMap();
	//Lets prepare the data buffers
	m_pTile=new BYTE[sMapHeader.lNumTiles];
	m_pArch=new BYTE[sMapHeader.lNumTiles];
	m_pObject=new BYTE[sMapHeader.lNumTiles];

	sg_char16 szTempLibName[MAX_PATH];
	sg_char16 szTempBGName[MAX_PATH];
	
	//Read and convert lib filename
	ReadFile(hFile, szTempLibName, sMapHeader.lLibraryNameSize, &dwBytesRead, NULL);
	WideCharToMultiByte(CP_ACP, 0, szTempLibName, sMapHeader.lLibraryNameSize/sizeof(sg_char16), m_lpLibraryFilenameA, MAX_PATH, NULL, NULL);
	
	//Read and convert bg filename
	ReadFile(hFile, szTempBGName, sMapHeader.lBGNameSize, &dwBytesRead, NULL);
	WideCharToMultiByte(CP_ACP, 0, szTempBGName, sMapHeader.lBGNameSize/sizeof(sg_char16), m_lpBGFilenameA, MAX_PATH, NULL, NULL);
	//#endif //unicode

	//Read the tile data
	ReadFile(hFile, m_pTile, sMapHeader.lTileDataSize, &dwBytesRead, NULL);
	//Read the architecture data
	ReadFile(hFile, m_pArch, sMapHeader.lArchDataSize, &dwBytesRead, NULL);
	//Read the object data
	ReadFile(hFile, m_pObject, sMapHeader.lObjectDataSize, &dwBytesRead, NULL);

	//Prepare the variables
	m_nMapWidth=sMapHeader.nMapWidth;
	m_nMapHeight=sMapHeader.nMapHeight;
	strcpy_s(m_lpMapFilenameA, countof(m_lpMapFilenameA), lpMapFilename);


	CloseHandle(hFile);
	return true;
}
