/*
	ImageLibrary.cpp - The SgImgLib class

	Copyright (c) 2003, Blaine Myers
*/
#include "ImgLib.h"
#include "bitmapex.h"
#include <stdio.h>

SgImgLib::SgImgLib( bool DontLoadBms )
: m_DontLoadBms( DontLoadBms )
{
	m_nNumImages=0;
	m_nNumBitmaps=0;
	m_pImageData=NULL;

	for(int i=0; i<MAX_BITMAPS; i++)
	{
		m_szBitmapFilenameA[i][0]=0;
		m_hBitmap[i]=NULL;
		m_BmOffsets[i] = 0;
	}
}

SgImgLib::~SgImgLib()
{
	SAFE_DELETE_ARRAY(m_pImageData);
	CloseMainBitmaps();
}

bool SgImgLib::GetImageData(sg_uint32 nEntry, IMAGEDATA *imgData)
{
	if((nEntry<1) || (nEntry>m_nNumImages))return false;

	*imgData = m_pImageData[nEntry-1];

	return true;
}

sg_uint16 SgImgLib::GetNumFrames(sg_uint32 nEntry)
{
	//return 0 if image doesn't exist
	if((nEntry<1) || (nEntry>m_nNumImages))return 0;

	return m_pImageData[nEntry-1].nFrames;
}

bool SgImgLib::GetImageName(char* Out, size_t OutSize , sg_uint32 nEntry )
{
	char szTemp[IMAGE_NAME_LENGTH];

	bool result = FALSE != WideCharToMultiByte(
		CP_ACP, 
		0, 
		m_pImageData[nEntry-1].szImgLabel, 
		-1,
		szTemp,
		IMAGE_NAME_LENGTH,
		NULL,
		NULL);

	strcpy_s( Out, OutSize , szTemp);
	return result;
}

HBITMAP SgImgLib::GetBitmap(sg_uint16 nBitmap)
{
	if((nBitmap<1) || (nBitmap>MAX_BITMAPS))return NULL;
	return m_hBitmap[nBitmap-1];
}

sg_uint32 SgImgLib::GetNumEntries()
{
	return m_nNumImages;
}


bool SgImgLib::OpenBitmap(LPSTR szFilename, sg_uint16 nBitmap)
{
	return OpenBitmapOffset(szFilename, 0, nBitmap);
}

bool SgImgLib::OpenBitmapOffset(LPCSTR szFilename, sg_uint32 nOffset, sg_uint16 nBitmap)
{
	if( (nBitmap<1) || (nBitmap>MAX_BITMAPS) )return false;

	if( m_DontLoadBms )
	{
		m_hBitmap[nBitmap - 1] = 0;
		m_BmOffsets[nBitmap - 1] = nOffset;
		strcpy_s(m_szBitmapFilenameA[nBitmap - 1], countof(m_szBitmapFilenameA[nBitmap - 1]), szFilename);
	}
	else
	{
		DeleteObject(m_hBitmap[nBitmap - 1]);
		m_hBitmap[nBitmap - 1] = LoadBitmapOffset(szFilename, nOffset);

		if (m_hBitmap[nBitmap - 1] == NULL)return false;
		m_BmOffsets[nBitmap - 1] = nOffset;
		strcpy_s(m_szBitmapFilenameA[nBitmap - 1], countof(m_szBitmapFilenameA[nBitmap - 1]), szFilename);
	}
	return true;
}

void SgImgLib::CloseMainBitmaps()
{
	for(int i=0; i<m_nNumBitmaps; i++)
	{
		if( m_hBitmap[i] )DeleteObject(m_hBitmap[i]);
		m_hBitmap[i] = NULL;
	}
	m_nNumBitmaps=0;
}


void SgImgLib::ClearDataBase()
{
	for(sg_uint32 i=0;i<m_nNumImages;i++)
	{
		ZeroMemory(&m_pImageData[i], sizeof(IMAGEDATA));
	}
	m_nNumImages=0;
	m_nNumBitmaps=0;
	CloseMainBitmaps();
	SAFE_DELETE_ARRAY(m_pImageData);
	m_pImageData=NULL;
}


void SgImgLib::CopyImageToDC(HDC hdcDest, sg_uint32 nEntry, int x, int y, BOOL bTransp)
{
	HDC hdcMainBitmap=NULL;

	if( (nEntry<1) || (nEntry>m_nNumImages) )return;

	hdcMainBitmap=CreateCompatibleDC(hdcMainBitmap);
	SelectObject(hdcMainBitmap, m_hBitmap[m_pImageData[nEntry-1].nBitmap-1]);


	if(!bTransp){
		StretchBlt(
			hdcDest, 
			x, 
			y, 
			m_pImageData[nEntry-1].nWidth, 
			m_pImageData[nEntry-1].nHeight, 
			hdcMainBitmap, 
			m_pImageData[nEntry-1].nX,
			m_pImageData[nEntry-1].nY, 
			m_pImageData[nEntry-1].nWidthSrc, 
			m_pImageData[nEntry-1].nHeightSrc, 
			SRCCOPY);
	}else{
		TransparentBlt2(
			hdcDest,
			x,
			y,
			m_pImageData[nEntry-1].nWidth,
			m_pImageData[nEntry-1].nHeight,
			hdcMainBitmap,
			m_pImageData[nEntry-1].nX,
			m_pImageData[nEntry-1].nY, 
			m_pImageData[nEntry-1].nWidthSrc, 
			m_pImageData[nEntry-1].nHeightSrc,
			RGB(255,0,255));

	}

	DeleteDC(hdcMainBitmap);
}

sg_uint16 SgImgLib::GetNumBitmaps()
{
	return m_nNumBitmaps;
}

void SgImgLib::StretchImageToDC(HDC hdcDest, sg_uint32 nEntry, int x, int y, int nWidth, int nHeight, BOOL bTransp)
{
	HDC hdcMainBitmap=NULL;

	if( (nEntry<1) || (nEntry>m_nNumImages) )return;

	hdcMainBitmap=CreateCompatibleDC(hdcMainBitmap);
	SelectObject(hdcMainBitmap, m_hBitmap[m_pImageData[nEntry-1].nBitmap-1]);


	if(!bTransp){
		StretchBlt(
			hdcDest, 
			x, 
			y, 
			nWidth, 
			nHeight, 
			hdcMainBitmap, 
			m_pImageData[nEntry-1].nX,
			m_pImageData[nEntry-1].nY, 
			m_pImageData[nEntry-1].nWidthSrc, 
			m_pImageData[nEntry-1].nHeightSrc, 
			SRCCOPY);
	}else{
		TransparentBlt2(
			hdcDest,
			x,
			y,
			nWidth,
			nHeight,
			hdcMainBitmap,
			m_pImageData[nEntry-1].nX,
			m_pImageData[nEntry-1].nY, 
			m_pImageData[nEntry-1].nWidthSrc, 
			m_pImageData[nEntry-1].nHeightSrc,
			RGB(255,0,255));

	}

	DeleteDC(hdcMainBitmap);
}

void SgImgLib::GetBitmapName(char* Out, size_t OutSize, sg_uint16 nBitmap)
{
	if((nBitmap<1)||(nBitmap>m_nNumBitmaps)){
		strcpy_s(Out, OutSize, "");
	}else{
		strcpy_s(Out, OutSize, m_szBitmapFilenameA[nBitmap-1]);
	}
}
/***************************************************************************

	SgImgLibEdit - For editing an SgImgLib

***************************************************************************/

SgImgLibEdit::SgImgLibEdit()
: SgImgLib( false )
{
	m_pImageData=new IMAGEDATA[DEFAULT_MAX_ENTRIES];
	m_nMaxEntries=DEFAULT_MAX_ENTRIES;
}

SgImgLibEdit::SgImgLibEdit(sg_uint32 nMaxEntries)
: SgImgLib( false )
{
	m_pImageData=new IMAGEDATA[nMaxEntries];
	m_nMaxEntries=nMaxEntries;
}

SgImgLibEdit::~SgImgLibEdit()
{

}

bool SgImgLibEdit::AddBitmap(LPSTR szFilename)
{
	if((m_nNumBitmaps<0) || (m_nNumBitmaps>MAX_BITMAPS))return false;

	if(!OpenBitmap(szFilename, m_nNumBitmaps+1))return false;

	strcpy_s(m_szBitmapFilenameA[m_nNumBitmaps], countof(m_szBitmapFilenameA[m_nNumBitmaps]), szFilename);
	m_nNumBitmaps++;

	return true;
}

bool SgImgLibEdit::AddEntry(
	sg_uint16 x, 
	sg_uint16 y, 
	sg_uint16 nWidthSrc, 
	sg_uint16 nHeightSrc, 
	sg_uint16 nWidth, 
	sg_uint16 nHeight, 
	sg_uint16 nBitmap, 
	sg_uint16 nFrames, 
	LPSTR szImageName
){
	if( (m_nNumImages<0) || (m_nNumImages>m_nMaxEntries) )return false;
	
	if(SetEntry(
		m_nNumImages+1, 
		x, 
		y, 
		nWidthSrc, 
		nHeightSrc, 
		nWidth, 
		nHeight, 
		nBitmap, 
		nFrames, 
		szImageName))
	{
		m_nNumImages++;
		return true;
	}
	
	return false;
}

bool SgImgLibEdit::SetEntry(
	sg_uint32 nReference, 
	sg_uint16 x, 
	sg_uint16 y, 
	sg_uint16 nWidthSrc, 
	sg_uint16 nHeightSrc, 
	sg_uint16 nWidth, 
	sg_uint16 nHeight, 
	sg_uint16 nBitmap, 
	sg_uint16 nFrames, 
	LPSTR szImageName
){
	if((nReference<1) || (nReference>m_nMaxEntries))return false;
	
	m_pImageData[nReference-1].nX=x;
	m_pImageData[nReference-1].nY=y;
	m_pImageData[nReference-1].nWidthSrc=nWidthSrc;
	m_pImageData[nReference-1].nHeightSrc=nHeightSrc;
	m_pImageData[nReference-1].nWidth=nWidth;
	m_pImageData[nReference-1].nHeight=nHeight;
	m_pImageData[nReference-1].nBitmap=nBitmap;
	m_pImageData[nReference-1].nFrames=nFrames;

	//convert image name to wide char
	MultiByteToWideChar(
		CP_ACP,
		MB_PRECOMPOSED,
		szImageName,
		-1,
		m_pImageData[nReference-1].szImgLabel,
		IMAGE_NAME_LENGTH);

	return true;
}

void SgImgLibEdit::SetNumEntries(sg_uint32 nNumEntries)
{
	m_nNumImages=nNumEntries;
}

bool SgImgLibEdit::AutoGenerateData(
	sg_uint16 nBitmap, 
	sg_uint16 nWidth, 
	sg_uint16 nHeight, 
	sg_uint16 nXSpacing, 
	sg_uint16 nYSpacing,
	BOOL bClear
)
{
//bail if main bitmap does not exist
	if((nBitmap<1) || (nBitmap>m_nNumBitmaps))return false;
	if(!m_hBitmap[nBitmap-1])return false;

	int nNumAcross=0;
	int nNumDown=0;
	int nNumTiles=0;
	int nReference=0;

	BITMAP bm;
	GetObject(m_hBitmap[nBitmap-1], sizeof(bm), &bm);
	
	//get how many bitmaps across as well as how many down
	nNumAcross=bm.bmWidth/(nWidth+nXSpacing);
	nNumDown=bm.bmHeight/(nHeight+nYSpacing);
	nNumTiles=nNumAcross*nNumDown;
	if(bClear){
		m_nNumImages=0;
	}
	nReference=m_nNumImages+1;
	for(int down=0;down<nNumDown;down++){
		for(int across=0;across<nNumAcross;across++, nReference++){
			AddEntry(
				across*(nWidth+nXSpacing),
				down*(nHeight+nYSpacing),
				nWidth,
				nHeight,
				nWidth,
				nHeight,
				nBitmap,
				1,
				"");
		}
	}
	return true;
}


bool SgImgLibEdit::SaveData(LPSTR szFilename)
{
	//Declare headers
	IMGLIBHEADER lbHeader;
	IMGDBHEADER dbHeader;

	//fill in library header
	lbHeader.wType=*(sg_uint16*)"IL";
	lbHeader.nVersion=10; //10 is current version
	lbHeader.lReserved1=lbHeader.lReserved2=0;
	lbHeader.nOffsetBytes=sizeof(IMGLIBHEADER);

	//fill in database header
	dbHeader.nEntries=m_nNumImages;
	dbHeader.nNumBMs=m_nNumBitmaps;
	dbHeader.nSizeofEntryData=(m_nNumImages*sizeof(IMAGEDATA));

	//declare handle for file for writing
	HANDLE hFile=INVALID_HANDLE_VALUE;
	DWORD dwBytesWritten=0;

	//open file for writing
	hFile=CreateFileA(
		szFilename,
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		(LPSECURITY_ATTRIBUTES)NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		(HANDLE)NULL);

	if(hFile==INVALID_HANDLE_VALUE)return false;

	//we start by writing the IMGLIBHEAER
	WriteFile(hFile, &lbHeader, sizeof(IMGLIBHEADER), &dwBytesWritten, NULL);
	//write the database header next
	WriteFile(hFile, &dbHeader, sizeof(IMGDBHEADER), &dwBytesWritten, NULL);

	//we now write the bitmap filenamedata
	sg_uint16 nBMNameLen=0;
	sg_char16 szTemp[MAX_PATH];
	for(int i=0; i<m_nNumBitmaps; i++){
		nBMNameLen=(strlen(m_szBitmapFilenameA[i])+1)*sizeof(sg_char16);
		//convert bimtap name to wide char
		MultiByteToWideChar(
			CP_ACP,
			MB_PRECOMPOSED,
			m_szBitmapFilenameA[i],
			-1,
			szTemp,
			IMAGE_NAME_LENGTH);

		//Write the size of the name
		WriteFile(hFile, &nBMNameLen, sizeof(sg_uint16), &dwBytesWritten, NULL);
		//write the actual name
		WriteFile(hFile, szTemp, nBMNameLen, &dwBytesWritten, NULL);
	}

	//now write image data
	WriteFile(hFile, m_pImageData, dbHeader.nSizeofEntryData, &dwBytesWritten, NULL);

	//we are now done, so close file
	CloseHandle(hFile);
	return true;
}


bool SgImgLibEdit::LoadData(LPSTR szFilename)
{
	//Declare headers
	IMGLIBHEADER lbHeader;
	IMGDBHEADER dbHeader;

	//declare file to read
	HANDLE hFile=INVALID_HANDLE_VALUE;

	//open file for reading
	hFile=CreateFileA(
		szFilename,
		GENERIC_READ,
		FILE_SHARE_READ,
		(LPSECURITY_ATTRIBUTES)NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		(HANDLE)NULL);

	DWORD dwBytesRead=0;

	//begin by reading the header
	ReadFile(hFile, &lbHeader, sizeof(IMGLIBHEADER), &dwBytesRead, NULL);

	if(lbHeader.wType != *(sg_uint16*)"IL"){
		CloseHandle(hFile);
		return false;
	}
	if(lbHeader.nVersion != 10){
		CloseHandle(hFile);
		return false;
	}
	//we now know that the file is the correct type so continue
	//read the database header
	ReadFile(hFile, &dbHeader, sizeof(IMGDBHEADER), &dwBytesRead, NULL);
	
	m_nNumImages=dbHeader.nEntries;
	m_nNumBitmaps=dbHeader.nNumBMs;
	//we now read the bitmap filenames
	sg_uint16 nBMNameLen=0;
	sg_char16 szTemp[MAX_PATH];
	for(int i=0; i<m_nNumBitmaps; i++){
		//get size of entry
		ReadFile(hFile, &nBMNameLen, sizeof(sg_uint16), &dwBytesRead, NULL);
		//read entry
		ReadFile(hFile, szTemp, nBMNameLen, &dwBytesRead, NULL);
		//convert to multi-byte
		WideCharToMultiByte(
			CP_ACP, 
			0, 
			szTemp, 
			-1,
			m_szBitmapFilenameA[i],
			MAX_PATH,
			NULL,
			NULL);

	}
	//read image data
	ReadFile(hFile, m_pImageData, dbHeader.nSizeofEntryData, &dwBytesRead, NULL);
	//were done now close the file
	CloseHandle(hFile);
	//now attempt to load bitmaps
	
	sg_uint16 nNumBitmaps=m_nNumBitmaps;
	CloseMainBitmaps();
	for(int i=0; i<nNumBitmaps; i++){
		AddBitmap(m_szBitmapFilenameA[i]);
	}


	return true;
}

bool SgImgLibEdit::BuildLibrary(LPSTR szFilename)
{
	IMGLIBHEADER lbHeader;
	IMGHEADER imHeader;
	ZeroMemory(&lbHeader, sizeof(IMGLIBHEADER));
	ZeroMemory(&imHeader, sizeof(IMGHEADER));

	//prepare the headers
	lbHeader.wType=*(sg_uint16*)"IL";
	lbHeader.nVersion=20;
	lbHeader.lReserved1=0;
	lbHeader.lReserved2=0;
	lbHeader.nOffsetBytes=sizeof(IMGLIBHEADER);

	imHeader.nEntrys=m_nNumImages;
	imHeader.nNumBMs=m_nNumBitmaps;
	imHeader.nSizeofEntryData=(m_nNumImages*sizeof(IMAGEDATA));
	imHeader.nSizeofBitmapInfo=(m_nNumBitmaps*sizeof(BITMAPDATA));

	//get the size of all the bitmaps and read their data
	BITMAPDATA bmData[MAX_BITMAPS];
	ZeroMemory(&bmData, sizeof(BITMAPDATA)*MAX_BITMAPS);

	DWORD dwBytesRead=0, dwBytesWritten=0;

	int i=0, j=0;
	//sg_uint32 dwBitmapSize[MAX_BITMAPS];
	BYTE *btBitmapData[MAX_BITMAPS];
	HANDLE hFile=NULL;

	for(i=0; i<m_nNumBitmaps; i++){
		hFile=CreateFileA(
			m_szBitmapFilenameA[i],
			GENERIC_READ,
			FILE_SHARE_READ,
			(LPSECURITY_ATTRIBUTES)NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			(HANDLE)NULL);

		if(hFile==INVALID_HANDLE_VALUE){
			for(j=0; j<i; j++){
				SAFE_DELETE_ARRAY(btBitmapData[i]);
			}

			CloseHandle(hFile);
			return false;
		}

		bmData[i].dwSize=GetFileSize(hFile, NULL);
		btBitmapData[i]=new BYTE[bmData[i].dwSize];

		if(btBitmapData[i]==NULL){
			CloseHandle(hFile);
			for(j=0; j<i;j++){
				SAFE_DELETE_ARRAY(btBitmapData[i]);
			}
			return false;
		}
		if(!ReadFile(hFile, btBitmapData[i], bmData[i].dwSize, &dwBytesRead, NULL)){
			CloseHandle(hFile);
			for(j=0; j<=i;j++){
				SAFE_DELETE_ARRAY(btBitmapData[i]);
			}
			return false;	
		}
		CloseHandle(hFile);

	}
	//prepare bitmap offsets
	for(i=0; i<m_nNumBitmaps; i++){
		bmData[i].dwOffset=
								sizeof(IMGLIBHEADER)+
								sizeof(IMGHEADER)+
								imHeader.nSizeofEntryData+
								imHeader.nSizeofBitmapInfo;
		for(j=0; j<i; j++){
			bmData[i].dwOffset+=bmData[j].dwSize;
		}

	}
	//we've now read all the bitmaps let's begin writing the file
	hFile=CreateFileA(
		szFilename,
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		(LPSECURITY_ATTRIBUTES)NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		(HANDLE)NULL);

	//if failed to open file clear bitmap data and exit
	if(hFile==INVALID_HANDLE_VALUE){
		CloseHandle(hFile);
		for(j=0; j<m_nNumBitmaps;j++){
			SAFE_DELETE_ARRAY(btBitmapData[i]);
		}
		
		return false;	
	}

	//write the headers
	WriteFile(hFile, &lbHeader, sizeof(IMGLIBHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, &imHeader, sizeof(IMGHEADER), &dwBytesWritten, NULL);
	//write bitmap info
	WriteFile(hFile, &bmData, imHeader.nSizeofBitmapInfo, &dwBytesWritten, NULL);
	//write image data
	WriteFile(hFile, m_pImageData, imHeader.nSizeofEntryData, &dwBytesWritten, NULL);

	//write the bitmaps one by one
	for(i=0; i<m_nNumBitmaps; i++){
		WriteFile(hFile, btBitmapData[i], bmData[i].dwSize, &dwBytesWritten, NULL);
	}

	//were done close file and delete image data
	CloseHandle(hFile);
	for(i=0; i<m_nNumBitmaps; i++){
		SAFE_DELETE_ARRAY(btBitmapData[i]);
	}


	return true;
}


bool SgImgLibEdit::ImportLibrary(LPSTR szFilename)
{
	IMGLIBHEADER lbHeader;
	IMGHEADER imHeader;
	HANDLE hFile;
	BITMAPDATA bmData[MAX_BITMAPS];
	DWORD dwBytesRead=0;

	ZeroMemory(&lbHeader, sizeof(IMGLIBHEADER));
	ZeroMemory(&imHeader, sizeof(IMGHEADER));

	hFile=CreateFileA(
		szFilename,
		GENERIC_READ,
		FILE_SHARE_READ,
		(LPSECURITY_ATTRIBUTES)NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		(HANDLE)NULL);

	if(hFile==INVALID_HANDLE_VALUE){
		return false;
	}

	ReadFile(hFile, &lbHeader, sizeof(IMGLIBHEADER), &dwBytesRead, NULL);
	if(lbHeader.wType!=*(sg_uint16*)"IL"){
		CloseHandle(hFile);
		return false;
	}
	if(lbHeader.nVersion!=20){
		CloseHandle(hFile);
		return false;
	}

	//we know the file is correct so continue reading
	ClearDataBase();

	ReadFile(hFile, &imHeader, sizeof(IMGHEADER), &dwBytesRead, NULL);
	ReadFile(hFile, &bmData, imHeader.nSizeofBitmapInfo, &dwBytesRead, NULL);
	ReadFile(hFile, m_pImageData, imHeader.nSizeofEntryData, &dwBytesRead, NULL);

	m_nNumImages=imHeader.nEntrys;

	CloseHandle(hFile);
	//at this point we've loaded all the data, but
	//we need to extract the bitmap files and load them
	//into the new file
	DWORD dwBytesWritten=0;
	int i=0;
	char szBitmapName[MAX_PATH];
	BYTE *pBitmap=NULL;

	for(i=0; i<imHeader.nNumBMs; i++)
	{
		hFile=CreateFileA(
			szFilename, 
			GENERIC_READ,
			FILE_SHARE_READ,
			(LPSECURITY_ATTRIBUTES)NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			(HANDLE)NULL);

		if(hFile==INVALID_HANDLE_VALUE)return false;

		pBitmap=new BYTE[bmData[i].dwSize];

		SetFilePointer(hFile, bmData[i].dwOffset, NULL, FILE_BEGIN);

		ReadFile(hFile, pBitmap, bmData[i].dwSize, &dwBytesRead, NULL);

		CloseHandle(hFile);

		sprintf_s(szBitmapName, countof(szBitmapName), "%s[%i].bmp", szFilename, i+1);
		hFile=CreateFileA(
			szBitmapName,
			GENERIC_WRITE,
			FILE_SHARE_WRITE,
			(LPSECURITY_ATTRIBUTES)NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			hFile);

		if(hFile==INVALID_HANDLE_VALUE)return false;

		WriteFile(hFile, pBitmap, bmData[i].dwSize, &dwBytesWritten, NULL);

		SAFE_DELETE_ARRAY(pBitmap);

		CloseHandle(hFile);

		AddBitmap(szBitmapName);

	}
	return true;
}

void SgImgLibEdit::ClearDataBase()
{
	CloseMainBitmaps();

	m_nNumImages=0;
	m_nNumBitmaps=0;
	SAFE_DELETE_ARRAY(m_pImageData);
	m_pImageData=new IMAGEDATA[m_nMaxEntries];
	
	for(int i=0; i<MAX_BITMAPS; i++)
	{
		m_szBitmapFilenameA[i][0]=0;
		m_hBitmap[i]=NULL;
		m_BmOffsets[i] = 0;
	}
}

/***************************************************************************

	SgImgLibArchive - For reading a built SgImgLib.

***************************************************************************/

SgImgLibArchive::SgImgLibArchive( bool DontLoadBms )
: SgImgLib( DontLoadBms )
{
	m_nSelectedEntry = 1;

}

SgImgLibArchive::~SgImgLibArchive()
{
	CloseArchive();
}

sg_uint32 SgImgLibArchive::GetSelectedEntry()
{
	return m_nSelectedEntry;
}

void SgImgLibArchive::SetSelectedEntry(sg_uint32 nEntry)
{
	if ((nEntry<1) || (nEntry>m_nNumImages))return;

	m_nSelectedEntry = nEntry;
}

bool SgImgLibArchive::LoadArchive(LPCSTR szFilename)
{
	IMGLIBHEADER lbHeader;
	IMGHEADER imHeader;
	ZeroMemory(&lbHeader, sizeof(IMGLIBHEADER));
	ZeroMemory(&imHeader, sizeof(IMGHEADER));

	HANDLE hFile = NULL;
	int i = 0;

	CloseArchive();

	hFile = CreateFileA(
		szFilename,
		GENERIC_READ,
		FILE_SHARE_READ,
		(LPSECURITY_ATTRIBUTES)NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		(HANDLE)NULL);

	if (hFile == INVALID_HANDLE_VALUE)return false;

	DWORD dwBytesRead = 0;
	ReadFile(hFile, &lbHeader, sizeof(IMGLIBHEADER), &dwBytesRead, NULL);
	if (dwBytesRead < sizeof(IMGLIBHEADER)){
		CloseHandle(hFile);
		return false;
	}
	if (lbHeader.wType != *(sg_uint16*)"IL"){
		CloseHandle(hFile);
		return false;
	}
	if (lbHeader.nVersion != 20){
		CloseHandle(hFile);
		return false;
	}

	//file is correct type so continue
	ReadFile(hFile, &imHeader, sizeof(IMGHEADER), &dwBytesRead, NULL);
	BITMAPDATA bmData[MAX_BITMAPS];
	ReadFile(hFile, &bmData, imHeader.nSizeofBitmapInfo, &dwBytesRead, NULL);
	m_pImageData = new IMAGEDATA[imHeader.nEntrys];
	ReadFile(hFile, m_pImageData, imHeader.nSizeofEntryData, &dwBytesRead, NULL);

	m_nNumImages = imHeader.nEntrys;

	CloseHandle(hFile);


	for (i = 1; i <= imHeader.nNumBMs; i++)
	{
		OpenBitmapOffset(szFilename, bmData[i - 1].dwOffset, i);
		m_nNumBitmaps++;
	}


	return true;
}

void SgImgLibArchive::CloseArchive()
{
	SAFE_DELETE_ARRAY(m_pImageData);
	for (int i = 0; i < m_nNumBitmaps; i++)
	{
		if( m_hBitmap[i] ) DeleteObject(m_hBitmap[i]);
		m_hBitmap[i] = NULL;
		m_BmOffsets[i] = 0;
		m_szBitmapFilenameA[i][0]=0;
	}
	m_nNumImages = 0;
	m_nNumBitmaps = 0;
}
