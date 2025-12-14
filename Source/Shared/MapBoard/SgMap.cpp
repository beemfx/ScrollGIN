/*
	Mapboard.cpp - Functions for SgMap class.

	Copyright (c) 2002, Blaine Myers
*/

#include "SgMap.h"
#include <windows.h>

SgMap::SgMap()
{
	m_pTile=new sg_uint8;
	m_pArch=new sg_uint8;
	m_pObject=new sg_uint8;

	m_nMapWidth=m_nMapHeight=1;
	m_dwTileDim=MAP_TILEDIM;
}

sg_uint32 SgMap::GetTileDim()
{
	return m_dwTileDim;
}

sg_uint32 SgMap::SetTileDim(sg_uint32 dwDim)
{
	sg_uint32 dwTemp;
	dwTemp=m_dwTileDim;
	m_dwTileDim=dwDim;
	return dwTemp;
}

sg_uint8 SgMap::GetArchType(int x, int y)
{
	if(x<1||x>m_nMapWidth)return 0x00;
	if(y<1||y>m_nMapHeight)return 0x00;
	
	sg_uint8 nByte=m_pArch[CoordToPos(x, y)];
	nByte=nByte&0xf0;
	nByte=nByte>>4;
	return nByte;
}

sg_uint8 SgMap::GetArchPiece(int x, int y)
{ 
	if(x<1||x>m_nMapWidth)return 0x00;
	if(y<1||y>m_nMapHeight)return 0x00;

	sg_uint8 nByte=m_pArch[CoordToPos(x, y)];
	nByte=nByte&0x0f;
	return (ARCHTYPE)nByte;
}

SgMap::~SgMap()
{
	SAFE_DELETE_ARRAY(m_pTile);
	SAFE_DELETE_ARRAY(m_pArch);
	SAFE_DELETE_ARRAY(m_pObject);
}

const char* SgMap::GetLibraryName()const
{
	return m_lpLibraryFilenameA;
}


const char* SgMap::GetBGName()const
{
	return m_lpBGFilenameA;
}

sg_uint16 SgMap::GetMapWidth(){
	return m_nMapWidth;
}

sg_uint16 SgMap::GetMapHeight(){
	return m_nMapHeight;
}

sg_uint8 SgMap::GetTile(int x, int y){
	if(x<1||x>m_nMapWidth)return 0;
	if(y<1||y>m_nMapHeight)return 0;
	return m_pTile[CoordToPos(x, y)];
}

sg_uint8 SgMap::GetArch(int x, int y){
	if(x<1||x>m_nMapWidth)return 0;
	if(y<1||y>m_nMapHeight)return 0;
	return m_pArch[CoordToPos(x, y)];
}

sg_uint8 SgMap::GetObj(int x, int y){
	if(x<1||x>m_nMapWidth)return 0;
	if(y<1||y>m_nMapHeight)return 0;
	return m_pObject[CoordToPos(x, y)];
}

sg_uint32 SgMap::CoordToPos(int x, int y){
	if(x<1||x>m_nMapWidth)return 0;
	if(y<1||y>m_nMapHeight)return 0;
	return y+m_nMapHeight*(x-1)-1;
}

void SgMap::ClearMap(){
	//Delete the map data
	SAFE_DELETE_ARRAY(m_pTile);
	SAFE_DELETE_ARRAY(m_pArch);
	SAFE_DELETE_ARRAY(m_pObject);
	
	//Set dimensions to 0
	m_nMapWidth=m_nMapHeight=0;

	//nullify the filenames
	m_lpMapFilenameA[0]=m_lpLibraryFilenameA[0]=m_lpBGFilenameA[0]=NULL;
}

bool SgMap::LoadMap(sg_cpstr lpMapFilename)
{
	HANDLE hFile;
	//Open the file
	hFile=CreateFileA(lpMapFilename, GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL,
							OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		return false;
	}
	
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
	m_pTile=new sg_uint8[sMapHeader.lNumTiles];
	m_pArch=new sg_uint8[sMapHeader.lNumTiles];
	m_pObject=new sg_uint8[sMapHeader.lNumTiles];

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


/***************************************************************************
	SgMapEdit  - An editable map.
***************************************************************************/

void SgMapEdit::ClearArch()
{
	ZeroMemory(m_pArch, sizeof(*m_pArch)*m_nMapWidth*m_nMapHeight);
}

void SgMapEdit::ClearTile()
{
	ZeroMemory(m_pTile, sizeof(*m_pTile)*m_nMapWidth*m_nMapHeight);
}

void SgMapEdit::ClearObject()
{
	ZeroMemory(m_pObject, sizeof(*m_pObject)*m_nMapWidth*m_nMapHeight);
}

bool SgMapEdit::SetTile(int x, int y, sg_uint8 nNewValue)
{
	if(x<0||x>m_nMapWidth)return false;
	if(y<0||y>m_nMapHeight)return false;

	//We don't need to change the value if it's the same
	if(m_pTile[CoordToPos(x, y)]==nNewValue)return false;

	m_pTile[CoordToPos(x, y)]=nNewValue;
	return true;
}

bool SgMapEdit::SetArch(int x, int y, sg_uint8 nNewValue)
{
	if(x<0||x>m_nMapWidth)return false;
	if(y<0||y>m_nMapWidth)return false;

	//We don't need to change the value if it's the same
	if(m_pArch[CoordToPos(x, y)]==nNewValue)return false;

	m_pArch[CoordToPos(x, y)]=nNewValue;
	return true;
}

sg_uint8 SgMapEdit::GetArchSmart(int x, int y){
	if(x<0||x>m_nMapWidth)return 0;
	if(y<0||y>m_nMapWidth)return 0;

	sg_uint8 nValue=m_pArch[CoordToPos(x, y)];

	if(nValue==0x00)return 1;
	if(GetArchType(x, y)==1){
		return 2+GetArchPiece(x, y)-1;
	}
	else if(GetArchType(x, y)==2){
		return 11+GetArchPiece(x, y)-1;
	}
	else if(GetArchType(x, y)==3){
		return 20+GetArchPiece(x, y)-1;
	}
	else return 0;
}

bool SgMapEdit::SetArchSmart(int x, int y, sg_uint8 nNewValue)
{
	if(x<0||x>m_nMapWidth)return false;
	if(y<0||y>m_nMapWidth)return false;

	//We don't need to change the value if it's the same
	if(m_pArch[CoordToPos(x, y)]==nNewValue)return false;

	sg_uint8 nTempValue=0x00;

	switch(nNewValue){
		case 1: nTempValue=0x00;break;
	
		case 2: nTempValue=0x11;break;
		case 3: nTempValue=0x12;break;
		case 4: nTempValue=0x13;break;
		case 5: nTempValue=0x14;break;
		case 6: nTempValue=0x15;break;
		case 7: nTempValue=0x16;break;
		case 8: nTempValue=0x17;break;
		case 9: nTempValue=0x18;break;
		case 10: nTempValue=0x19;break;

		case 11: nTempValue=0x21;break;
		case 12: nTempValue=0x22;break;
		case 13: nTempValue=0x23;break;
		case 14: nTempValue=0x24;break;
		case 15: nTempValue=0x25;break;
		case 16: nTempValue=0x26;break;
		case 17: nTempValue=0x27;break;
		case 18: nTempValue=0x28;break;
		case 19: nTempValue=0x29;break;

		case 20: nTempValue=0x31;break;
		case 21: nTempValue=0x32;break;
		case 22: nTempValue=0x33;break;
		case 23: nTempValue=0x34;break;
		case 24: nTempValue=0x35;break;
		case 25: nTempValue=0x36;break;
		case 26: nTempValue=0x37;break;
		case 27: nTempValue=0x38;break;
		case 28: nTempValue=0x39;break;
	}

	if(m_pArch[CoordToPos(x, y)]!=nTempValue)
	{
		m_pArch[CoordToPos(x, y)]=nTempValue;
		return true;
	}
	
	return false;
}

bool SgMapEdit::SetObj(int x, int y, sg_uint8 nNewValue)
{
	if(x<0||x>m_nMapWidth)return false;
	if(y<0||y>m_nMapWidth)return false;

	//We don't need to change the value if it's the same
	if(m_pObject[CoordToPos(x, y)]==nNewValue)return false;

	m_pObject[CoordToPos(x, y)]=nNewValue;
	return true;
}

bool SgMapEdit::GenerateNewMap(int nWidth, int nHeight, sg_cpstr lpLibFilename, sg_cpstr lpBGFilename)
{
	if(nWidth<1)return false;
	if(nHeight<1)return false;
	if(lpLibFilename==NULL)return false;
	
	//Now we know were capable of creating a map, so lets do it
	
	//Clear the map
	ClearMap();
	
	//Copy filenames into memory
	strcpy_s(m_lpLibraryFilenameA, countof(m_lpLibraryFilenameA), lpLibFilename);

	//Only copy background if it exist, a background isn't necessary
	if(lpBGFilename!=NULL)
		strcpy_s(m_lpBGFilenameA, countof(m_lpBGFilenameA), lpBGFilename);

	//Set width and height
	m_nMapWidth=nWidth;
	m_nMapHeight=nHeight;

	m_lpMapFilenameA[0]=NULL;

	//Prepare data buffers
	m_pTile=new sg_uint8[nWidth*nHeight];
	m_pArch=new sg_uint8[nWidth*nHeight];
	m_pObject=new sg_uint8[nWidth*nHeight];
	//Make sure the memory was allocated correctly.
	if(!m_pTile)return false;
	if(!m_pArch)return false;
	if(!m_pObject)return false;

	//Clear data buffers in case they aren't
	ZeroMemory(m_pTile, sizeof(*m_pTile)*nWidth*nHeight);
	ZeroMemory(m_pArch, sizeof(*m_pArch)*nWidth*nHeight);
	ZeroMemory(m_pObject, sizeof(*m_pObject)*nWidth*nHeight);

	return true;
}


bool SgMapEdit::SaveMap(sg_cpstr lpMapFilename)
{
	HANDLE hFile;
	hFile=CreateFileA(lpMapFilename, GENERIC_WRITE, FILE_SHARE_WRITE, (LPSECURITY_ATTRIBUTES)NULL,
							CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

	if(hFile==INVALID_HANDLE_VALUE)return false;

	//Now that we have a file, lets prepare to write

	//Lets prepare the file header
	MAPHEADER sMapHeader;
	
	sMapHeader.wType=*(sg_uint16*)"SM";
	sMapHeader.nVersion=1;
	sMapHeader.lReserved1=0;
	sMapHeader.lReserved2=0;
	sMapHeader.nMapWidth=m_nMapWidth;
	sMapHeader.nMapHeight=m_nMapHeight;
	sMapHeader.lNumTiles=m_nMapWidth*m_nMapHeight;
	sMapHeader.lLibraryNameSize=(strlen(m_lpLibraryFilenameA)+1)*sizeof(sg_char16);
	sMapHeader.lBGNameSize=(strlen(m_lpBGFilenameA)+1)*sizeof(sg_char16);
	sMapHeader.lTileDataSize=sizeof(*m_pTile)*sMapHeader.lNumTiles;
	sMapHeader.lArchDataSize=sizeof(*m_pArch)*sMapHeader.lNumTiles;
	sMapHeader.lObjectDataSize=sizeof(*m_pObject)*sMapHeader.lNumTiles;

	DWORD dwBytesWritten;

	//Write header
	WriteFile(hFile, &sMapHeader, sizeof(sMapHeader), &dwBytesWritten, NULL);
	/*
		If we're using the ansi version we want to convert the filenames to Wide characters
		that way maps will be compatible whether generated by the UNICODE or ANSI version.
	*/
	sg_char16 szTempLibName[MAX_PATH];
	sg_char16 szTempBGName[MAX_PATH];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_lpLibraryFilenameA, -1, szTempLibName, MAX_PATH);
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_lpBGFilenameA, -1, szTempBGName, MAX_PATH);
	//Write wide lib name
	WriteFile(hFile, szTempLibName, sMapHeader.lLibraryNameSize, &dwBytesWritten, NULL);
	//Write wide bg name
	WriteFile(hFile, szTempBGName, sMapHeader.lBGNameSize, &dwBytesWritten, NULL);
	
	//Write tile data
	WriteFile(hFile, m_pTile, sMapHeader.lTileDataSize, &dwBytesWritten, NULL);
	//Write the archtitecture data
	WriteFile(hFile, m_pArch, sMapHeader.lArchDataSize, &dwBytesWritten, NULL);
	//Write the object data
	WriteFile(hFile, m_pObject, sMapHeader.lObjectDataSize, &dwBytesWritten, NULL);

	CloseHandle(hFile);
	strcpy_s(m_lpMapFilenameA, countof(m_lpMapFilenameA), lpMapFilename);
	return true;
}

void SgMapEdit::ChangeBackground(sg_cpstr lpBackgroudFilename)
{
	strcpy_s(m_lpBGFilenameA, countof(m_lpBGFilenameA), lpBackgroudFilename);
}

void SgMapEdit::ChangeLibrary(sg_cpstr lpLibraryFilename)
{
	strcpy_s(m_lpLibraryFilenameA, countof(m_lpLibraryFilenameA), lpLibraryFilename);
}

bool SgMapEdit::ChangeMapDimensions(int nNewWidth, int nNewHeight)
{
	if( (nNewWidth<1) || (nNewHeight<1) )return false;
	
	//We need to create temporary buffers to hold our data
	sg_uint8 *pTempTile;
	sg_uint8 *pTempArch;
	sg_uint8 *pTempObject;

	pTempTile=new sg_uint8[m_nMapWidth*m_nMapHeight];
	pTempArch=new sg_uint8[m_nMapWidth*m_nMapHeight];
	pTempObject=new sg_uint8[m_nMapWidth*m_nMapHeight];
	
	//Copy current memory into our temporary mem
	CopyMemory(pTempTile, m_pTile, sizeof(*m_pTile)*m_nMapHeight*m_nMapWidth);
	CopyMemory(pTempArch, m_pArch, sizeof(*m_pArch)*m_nMapHeight*m_nMapWidth);
	CopyMemory(pTempObject, m_pObject, sizeof(*m_pObject)*m_nMapHeight*m_nMapWidth);

	//Lets clear out the current data
	SAFE_DELETE_ARRAY(m_pTile);
	SAFE_DELETE_ARRAY(m_pArch);
	SAFE_DELETE_ARRAY(m_pObject);

	//Now create a buffers with the new appropriate size
	m_pTile=new sg_uint8[nNewWidth*nNewHeight];
	m_pArch=new sg_uint8[nNewWidth*nNewHeight];
	m_pObject=new sg_uint8[nNewWidth*nNewHeight];
	//Clear the memory in these buffers
	ZeroMemory(m_pTile, sizeof(*m_pTile)*nNewWidth*nNewHeight);
	ZeroMemory(m_pArch, sizeof(*m_pArch)*nNewWidth*nNewHeight);
	ZeroMemory(m_pObject, sizeof(*m_pObject)*nNewWidth*nNewHeight);

	//Now lets copy over the data, cropping if the map size is smaller
	//I beleive this algorithm is correct as I have not seen it fail.
	int x;
	int y;
	int i=0;
	for (x=1;x<=nNewWidth;x++){
		for(y=1;y<=nNewHeight;y++, i++){
			if(x<=m_nMapWidth && y<=m_nMapWidth){
				m_pTile[i]=pTempTile[CoordToPos(x, y)];
				m_pArch[i]=pTempArch[CoordToPos(x, y)];
				m_pObject[i]=pTempObject[CoordToPos(x, y)];
			}
		}
	}

	//Now we change the map width and height
	m_nMapWidth=nNewWidth;
	m_nMapHeight=nNewHeight;
	
	//Clear out temp buffers
	SAFE_DELETE_ARRAY(pTempTile);
	SAFE_DELETE_ARRAY(pTempArch);
	SAFE_DELETE_ARRAY(pTempObject);

	return true;
}