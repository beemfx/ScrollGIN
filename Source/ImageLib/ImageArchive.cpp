#include "imagelib.h"

CImageArchive::CImageArchive()
{
	m_nSelectedEntry=1;

}

CImageArchive::~CImageArchive()
{
	CloseArchive();
}

sg_uint32 CImageArchive::GetSelectedEntry()
{
	return m_nSelectedEntry;
}

void CImageArchive::SetSelectedEntry(sg_uint32 nEntry)
{
	if((nEntry<1) ||(nEntry>m_nNumImages))return;

	m_nSelectedEntry=nEntry;
}

bool CImageArchive::LoadArchive(LPCSTR szFilename)
{
	IMGLIBHEADER lbHeader;
	IMGHEADER imHeader;
	ZeroMemory(&lbHeader, sizeof(IMGLIBHEADER));
	ZeroMemory(&imHeader, sizeof(IMGHEADER));

	HANDLE hFile=NULL;
	int i=0;

	CloseArchive();

	hFile=CreateFileA(
		szFilename,
		GENERIC_READ,
		FILE_SHARE_READ,
		(LPSECURITY_ATTRIBUTES)NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		(HANDLE)NULL);
	
	if(hFile==INVALID_HANDLE_VALUE)return false;

	DWORD dwBytesRead=0;
	ReadFile(hFile, &lbHeader, sizeof(IMGLIBHEADER), &dwBytesRead, NULL);
	if(dwBytesRead<sizeof(IMGLIBHEADER)){
		CloseHandle(hFile);
		return false;
	}
	if(lbHeader.wType!=*(sg_uint16*)"IL"){
		CloseHandle(hFile);
		return false;
	}
	if(lbHeader.nVersion!=20){
		CloseHandle(hFile);
		return false;
	}

	//file is correct type so continue
	ReadFile(hFile, &imHeader, sizeof(IMGHEADER), &dwBytesRead, NULL);
	BITMAPDATA bmData[MAX_BITMAPS];
	ReadFile(hFile, &bmData, imHeader.nSizeofBitmapInfo, &dwBytesRead, NULL);
	m_pImageData=new IMAGEDATA[imHeader.nEntrys];
	ReadFile(hFile, m_pImageData, imHeader.nSizeofEntryData, &dwBytesRead, NULL);

	m_nNumImages=imHeader.nEntrys;

	CloseHandle(hFile);


	for(i=1; i<=imHeader.nNumBMs; i++)
	{
		OpenBitmapOffset(szFilename, bmData[i-1].dwOffset, i);
		m_nNumBitmaps++;
	}


	return true;
}

void CImageArchive::CloseArchive()
{
	SAFE_DELETE_ARRAY(m_pImageData);
	for(int i=0; i<m_nNumBitmaps; i++){
		DeleteObject(m_hBitmap[i]);
	}
	m_nNumImages=0;
	m_nNumBitmaps=0;
}