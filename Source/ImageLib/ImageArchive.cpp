#include "imagelib.h"
#include "defines.h"

CImageArchive::CImageArchive()
{
	m_nSelectedEntry=1;

}

CImageArchive::~CImageArchive()
{
	CloseArchive();
}

DWORD CImageArchive::GetSelectedEntry()
{
	return m_nSelectedEntry;
}

BOOL CImageArchive::SetSelectedEntry(DWORD nEntry)
{
	if((nEntry<1) ||(nEntry>m_nNumImages))return FALSE;

	m_nSelectedEntry=nEntry;
	return TRUE;
}

HRESULT CImageArchive::LoadArchive(LPCSTR szFilename)
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
	
	if(hFile==INVALID_HANDLE_VALUE)return E_FAIL;

	DWORD dwBytesRead=0;
	ReadFile(hFile, &lbHeader, sizeof(IMGLIBHEADER), &dwBytesRead, NULL);
	if(dwBytesRead<sizeof(IMGLIBHEADER)){
		CloseHandle(hFile);
		return E_FAIL;
	}
	if(lbHeader.wType!=*(WORD*)"IL"){
		CloseHandle(hFile);
		return E_FAIL;
	}
	if(lbHeader.nVersion!=20){
		CloseHandle(hFile);
		return E_FAIL;
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


	return S_OK;
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