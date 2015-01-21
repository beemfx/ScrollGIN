/*
	EditMapboard.cpp - functions for CEditMapBoard class, see
	header for information about each function.

	Coyright (c) 2002, Blaine Myers
*/
#include <windows.h>
#include <stdio.h>
#include "defines.h"
#include "mapboard.h"

HRESULT CEditMapBoard::ClearArch(){
	ZeroMemory(m_pArch, sizeof(*m_pArch)*m_nMapWidth*m_nMapHeight);
	return S_OK;
}

HRESULT CEditMapBoard::ClearTile(){
	ZeroMemory(m_pTile, sizeof(*m_pTile)*m_nMapWidth*m_nMapHeight);
	return S_OK;
}

HRESULT CEditMapBoard::ClearObject(){
	ZeroMemory(m_pObject, sizeof(*m_pObject)*m_nMapWidth*m_nMapHeight);
	return S_OK;
}

HRESULT CEditMapBoard::SetTile(int x, int y, BYTE nNewValue){
	if(x<0||x>m_nMapWidth)return E_FAIL;
	if(y<0||y>m_nMapHeight)return E_FAIL;

	//We don't need to change the value if it's the same
	if(m_pTile[CoordToPos(x, y)]==nNewValue)return S_FALSE;

	m_pTile[CoordToPos(x, y)]=nNewValue;
	return S_OK;
}

HRESULT CEditMapBoard::SetArch(int x, int y, BYTE nNewValue){
	if(x<0||x>m_nMapWidth)return E_FAIL;
	if(y<0||y>m_nMapWidth)return E_FAIL;

	//We don't need to change the value if it's the same
	if(m_pArch[CoordToPos(x, y)]==nNewValue)return S_FALSE;

	m_pArch[CoordToPos(x, y)]=nNewValue;
	return S_OK;
}

BYTE CEditMapBoard::GetArchSmart(int x, int y){
	if(x<0||x>m_nMapWidth)return 0;
	if(y<0||y>m_nMapWidth)return 0;

	BYTE nValue=m_pArch[CoordToPos(x, y)];

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

HRESULT CEditMapBoard::SetArchSmart(int x, int y, BYTE nNewValue){
	if(x<0||x>m_nMapWidth)return E_FAIL;
	if(y<0||y>m_nMapWidth)return E_FAIL;

	//We don't need to change the value if it's the same
	if(m_pArch[CoordToPos(x, y)]==nNewValue)return S_FALSE;

	BYTE nTempValue=0x00;

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
	/*
	if((nNewValue>=2) && (nNewValue<=10)){
		nTempValue=0x11+(2-nNewValue);
	}
	*/
	/*
	if(nNewValue==1)
	{		
		nTempValue=0x00;
	}
	else if((nNewValue>1) && (nNewValue <11))
	{
		nTempValue=0x10+(2-nNewValue);
	}
	*/
	/*
	else if((nNewValue>11) && (nNewValue <20))
	{
		nTempValue=0x20+(20-nNewValue);
	}
	else if((nNewValue>20) &&(nNewValue <30))
	{
		nTempValue=0x30+(30-nNewValue);
	}
	else return E_FAIL;
	*/

	if(m_pArch[CoordToPos(x, y)]!=nTempValue){
		m_pArch[CoordToPos(x, y)]=nTempValue;
		return S_OK;
	}else return S_FALSE;
}

HRESULT CEditMapBoard::SetObject(int x, int y, BYTE nNewValue){
	if(x<0||x>m_nMapWidth)return E_FAIL;
	if(y<0||y>m_nMapWidth)return E_FAIL;

	//We don't need to change the value if it's the same
	if(m_pObject[CoordToPos(x, y)]==nNewValue)return S_FALSE;

	m_pObject[CoordToPos(x, y)]=nNewValue;
	return S_OK;
}

HRESULT CEditMapBoard::GenerateNewMap(int nWidth, int nHeight, LPSTR lpLibFilename, LPSTR lpBGFilename)
{
	if(nWidth<1)return E_FAIL;
	if(nHeight<1)return E_FAIL;
	if(lpLibFilename==NULL)return E_FAIL;
	
	//Now we know were capabe of creating a map, so lets do it
	
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
	m_pTile=new BYTE[nWidth*nHeight];
	m_pArch=new BYTE[nWidth*nHeight];
	m_pObject=new BYTE[nWidth*nHeight];
	//Make sure the memory was allocated correctly.
	if(!m_pTile)return E_FAIL;
	if(!m_pArch)return E_FAIL;
	if(!m_pObject)return E_FAIL;

	//Clear data buffers in case they aren't
	ZeroMemory(m_pTile, sizeof(*m_pTile)*nWidth*nHeight);
	ZeroMemory(m_pArch, sizeof(*m_pArch)*nWidth*nHeight);
	ZeroMemory(m_pObject, sizeof(*m_pObject)*nWidth*nHeight);

	return S_OK;
}


HRESULT CEditMapBoard::SaveMap(LPSTR lpMapFilename)
{
	HANDLE hFile;
	hFile=CreateFileA(lpMapFilename, GENERIC_WRITE, FILE_SHARE_WRITE, (LPSECURITY_ATTRIBUTES)NULL,
							CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

	if(hFile==INVALID_HANDLE_VALUE)return E_FAIL;

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
	return S_OK;
}

void CEditMapBoard::ChangeBackground(LPCSTR lpBackgroudFilename)
{
	strcpy_s(m_lpBGFilenameA, countof(m_lpBGFilenameA), lpBackgroudFilename);
}

void CEditMapBoard::ChangeLibrary(LPCSTR lpLibraryFilename)
{
	strcpy_s(m_lpLibraryFilenameA, countof(m_lpLibraryFilenameA), lpLibraryFilename);
}

HRESULT CEditMapBoard::ChangeMapDimensions(int nNewWidth, int nNewHeight)
{
	if( (nNewWidth<1) || (nNewHeight<1) )return E_FAIL;
	
	//We need to create temporary buffers to hold our data
	BYTE *pTempTile;
	BYTE *pTempArch;
	BYTE *pTempObject;

	pTempTile=new BYTE[m_nMapWidth*m_nMapHeight];
	pTempArch=new BYTE[m_nMapWidth*m_nMapHeight];
	pTempObject=new BYTE[m_nMapWidth*m_nMapHeight];
	
	//Copy current memory into our temporary mem
	CopyMemory(pTempTile, m_pTile, sizeof(*m_pTile)*m_nMapHeight*m_nMapWidth);
	CopyMemory(pTempArch, m_pArch, sizeof(*m_pArch)*m_nMapHeight*m_nMapWidth);
	CopyMemory(pTempObject, m_pObject, sizeof(*m_pObject)*m_nMapHeight*m_nMapWidth);

	//Lets clear out the current data
	SAFE_DELETE_ARRAY(m_pTile);
	SAFE_DELETE_ARRAY(m_pArch);
	SAFE_DELETE_ARRAY(m_pObject);

	//Now create a buffers with the new appropriate size
	m_pTile=new BYTE[nNewWidth*nNewHeight];
	m_pArch=new BYTE[nNewWidth*nNewHeight];
	m_pObject=new BYTE[nNewWidth*nNewHeight];
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

	return S_OK;
}