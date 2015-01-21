/*
	Mapboard.cpp - Functions for CMapBoard class.

	Coyright (c) 2002, Blaine Myers
*/
#include "defines.h"
#include "MapBoard.h"

CMapBoard::CMapBoard(){
	m_pTile=new BYTE;
	m_pArch=new BYTE;
	m_pObject=new BYTE;

	m_nMapWidth=m_nMapHeight=1;
	m_dwTileDim=TILEDIM;
}

DWORD CMapBoard::GetTileDim(){
	return m_dwTileDim;
}

DWORD CMapBoard::SetTileDim(DWORD dwDim){
	DWORD dwTemp;
	dwTemp=m_dwTileDim;
	m_dwTileDim=dwDim;
	return dwTemp;
}

BYTE CMapBoard::GetArchType(int x, int y){
	if(x<1||x>m_nMapWidth)return 0x00;
	if(y<1||y>m_nMapHeight)return 0x00;
	
	BYTE nByte=m_pArch[CoordToPos(x, y)];
	nByte=nByte&0xf0;
	nByte=nByte>>4;
	return nByte;
}

BYTE CMapBoard::GetArchPiece(int x, int y){ 
	if(x<1||x>m_nMapWidth)return 0x00;
	if(y<1||y>m_nMapHeight)return 0x00;

	BYTE nByte=m_pArch[CoordToPos(x, y)];
	nByte=nByte&0x0f;
	return (ARCHTYPE)nByte;
}

CMapBoard::~CMapBoard(){
	SAFE_DELETE_ARRAY(m_pTile);
	SAFE_DELETE_ARRAY(m_pArch);
	SAFE_DELETE_ARRAY(m_pObject);
}

void CMapBoard::GetLibraryNameA(LPSTR lpLibraryName){
	strcpy(lpLibraryName, m_lpLibraryFilenameA);
}

void CMapBoard::GetLibraryNameW(LPWSTR lpLibraryName){
	wcscpy(lpLibraryName, m_lpLibraryFilenameW);
}

void CMapBoard::GetBGNameA(LPSTR lpBGName){
	strcpy(lpBGName, m_lpBGFilenameA);
}

void CMapBoard::GetBGNameW(LPWSTR lpBGName){
	wcscpy(lpBGName, m_lpBGFilenameW);
}

USHORT CMapBoard::GetMapWidth(){
	return m_nMapWidth;
}

USHORT CMapBoard::GetMapHeight(){
	return m_nMapHeight;
}

BYTE CMapBoard::GetTile(int x, int y){
	if(x<1||x>m_nMapWidth)return 0;
	if(y<1||y>m_nMapHeight)return 0;
	return m_pTile[CoordToPos(x, y)];
}

BYTE CMapBoard::GetArch(int x, int y){
	if(x<1||x>m_nMapWidth)return 0;
	if(y<1||y>m_nMapHeight)return 0;
	return m_pArch[CoordToPos(x, y)];
}

BYTE CMapBoard::GetObject(int x, int y){
	if(x<1||x>m_nMapWidth)return 0;
	if(y<1||y>m_nMapHeight)return 0;
	return m_pObject[CoordToPos(x, y)];
}

ULONG CMapBoard::CoordToPos(int x, int y){
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
	m_lpMapFilenameW[0]=m_lpLibraryFilenameW[0]=m_lpBGFilenameW[0]=NULL;
}

HRESULT CMapBoard::LoadMapA(LPCSTR lpMapFilename){
	HANDLE hFile;
	//Open the file
	hFile=CreateFileA(lpMapFilename, GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL,
							OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
	if(hFile==INVALID_HANDLE_VALUE)return E_FAIL;
	
	DWORD dwBytesRead;
	//Read Header First
	MAPHEADER sMapHeader;
	
	if(!ReadFile(hFile, &sMapHeader, sizeof(sMapHeader), &dwBytesRead, NULL)){
		CloseHandle(hFile);return E_FAIL;
	}

	//Check for valid file
	if(sMapHeader.wType!=*(WORD*)"SM"){
		CloseHandle(hFile);return E_FAIL;
	}

	if(sMapHeader.nVersion!=1){
		CloseHandle(hFile);return E_FAIL;
	}
	
	//We will now assume that it is a valid file, which may not necessarily be true
	
	//In case there is a current map loaded lets clear it
	ClearMap();
	//Lets prepare the data buffers
	m_pTile=new BYTE[sMapHeader.lNumTiles];
	m_pArch=new BYTE[sMapHeader.lNumTiles];
	m_pObject=new BYTE[sMapHeader.lNumTiles];

	/*
		See editmaboard comments for information on conversion
	*/
	/*
	#ifdef UNICODE
	//Next we read the library name
	ReadFile(hFile, m_lpLibraryFilename, sMapHeader.lLibraryNameSize, &dwBytesRead, NULL);
	//Read the background name
	ReadFile(hFile, m_lpBGFilename, sMapHeader.lBGNameSize, &dwBytesRead, NULL);
	#else //unicode not defined
	*/
	WCHAR szTempLibName[MAX_PATH];
	WCHAR szTempBGName[MAX_PATH];
	
	//Read and convert lib filename
	ReadFile(hFile, szTempLibName, sMapHeader.lLibraryNameSize, &dwBytesRead, NULL);
	WideCharToMultiByte(CP_ACP, 0, szTempLibName, sMapHeader.lLibraryNameSize/sizeof(WCHAR), m_lpLibraryFilenameA, MAX_PATH, NULL, NULL);
	
	//Read and convert bg filename
	ReadFile(hFile, szTempBGName, sMapHeader.lBGNameSize, &dwBytesRead, NULL);
	WideCharToMultiByte(CP_ACP, 0, szTempBGName, sMapHeader.lBGNameSize/sizeof(WCHAR), m_lpBGFilenameA, MAX_PATH, NULL, NULL);
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
	strcpy(m_lpMapFilenameA, lpMapFilename);


	CloseHandle(hFile);
	return S_OK;
}

HRESULT CMapBoard::LoadMapW(LPCWSTR lpMapFilename){
	HANDLE hFile;
	//Open the file
	hFile=CreateFileW(lpMapFilename, GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL,
							OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
	if(hFile==INVALID_HANDLE_VALUE)return E_FAIL;
	
	DWORD dwBytesRead;
	//Read Header First
	MAPHEADER sMapHeader;
	
	if(!ReadFile(hFile, &sMapHeader, sizeof(sMapHeader), &dwBytesRead, NULL)){
		CloseHandle(hFile);return E_FAIL;
	}

	//Check for valid file
	if(sMapHeader.wType!=*(WORD*)"SM"){
		CloseHandle(hFile);return E_FAIL;
	}

	if(sMapHeader.nVersion!=1){
		CloseHandle(hFile);return E_FAIL;
	}
	
	//We will now assume that it is a valid file, which may not necessarily be true
	
	//In case there is a current map loaded lets clear it
	ClearMap();
	//Lets prepare the data buffers
	m_pTile=new BYTE[sMapHeader.lNumTiles];
	m_pArch=new BYTE[sMapHeader.lNumTiles];
	m_pObject=new BYTE[sMapHeader.lNumTiles];

	/*
		See editmaboard comments for information on conversion
	*/
	//#ifdef UNICODE
	//Next we read the library name
	ReadFile(hFile, m_lpLibraryFilenameW, sMapHeader.lLibraryNameSize, &dwBytesRead, NULL);
	//Read the background name
	ReadFile(hFile, m_lpBGFilenameW, sMapHeader.lBGNameSize, &dwBytesRead, NULL);
	//#else //unicode not defined
	/*
	WCHAR szTempLibName[MAX_PATH];
	WCHAR szTempBGName[MAX_PATH];
	
	//Read and convert lib filename
	ReadFile(hFile, szTempLibName, sMapHeader.lLibraryNameSize, &dwBytesRead, NULL);
	WideCharToMultiByte(CP_ACP, 0, szTempLibName, sMapHeader.lLibraryNameSize/sizeof(WCHAR), m_lpLibraryFilename, MAX_PATH, NULL, NULL);
	
	//Read and convert bg filename
	ReadFile(hFile, szTempBGName, sMapHeader.lBGNameSize, &dwBytesRead, NULL);
	WideCharToMultiByte(CP_ACP, 0, szTempBGName, sMapHeader.lBGNameSize/sizeof(WCHAR), m_lpBGFilename, MAX_PATH, NULL, NULL);
	#endif //unicode
	*/

	//Read the tile data
	ReadFile(hFile, m_pTile, sMapHeader.lTileDataSize, &dwBytesRead, NULL);
	//Read the architecture data
	ReadFile(hFile, m_pArch, sMapHeader.lArchDataSize, &dwBytesRead, NULL);
	//Read the object data
	ReadFile(hFile, m_pObject, sMapHeader.lObjectDataSize, &dwBytesRead, NULL);

	//Prepare the variables
	m_nMapWidth=sMapHeader.nMapWidth;
	m_nMapHeight=sMapHeader.nMapHeight;
	wcscpy(m_lpMapFilenameW, lpMapFilename);


	CloseHandle(hFile);
	return S_OK;
}