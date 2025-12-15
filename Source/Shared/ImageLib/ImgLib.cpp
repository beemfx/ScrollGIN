/*
	ImageLibrary.cpp - The SgImgLib class

	Copyright (c) 2003, Blaine Myers
*/
#include "ImgLib.h"
#include "bitmapex.h"
#include <stdio.h>

SgImgLib::SgImgLib(bool DontLoadBms)
	: m_DontLoadBms(DontLoadBms)
{
	m_nNumBitmaps = 0;

	for (int i = 0; i < MAX_BITMAPS; i++)
	{
		m_szBitmapFilenameA[i][0] = 0;
		m_hBitmap[i] = NULL;
		m_BmOffsets[i] = 0;
	}
}

SgImgLib::~SgImgLib()
{
	CloseMainBitmaps();
}

bool SgImgLib::GetImageData(sg_uint32 nEntry, IMAGEDATA* imgData)
{
	if ((nEntry < 1) || (nEntry > m_ImageData.size()))return false;

	*imgData = m_ImageData[nEntry - 1];

	return true;
}

sg_uint16 SgImgLib::GetNumFrames(sg_uint32 nEntry)
{
	//return 0 if image doesn't exist
	if ((nEntry < 1) || (nEntry > m_ImageData.size()))return 0;

	return m_ImageData[nEntry - 1].nFrames;
}

bool SgImgLib::GetImageName(char* Out, size_t OutSize, sg_uint32 nEntry)
{
	char szTemp[IMAGE_NAME_LENGTH];

	bool result = FALSE != WideCharToMultiByte(
		CP_ACP,
		0,
		m_ImageData[nEntry - 1].szImgLabel,
		-1,
		szTemp,
		IMAGE_NAME_LENGTH,
		NULL,
		NULL);

	strcpy_s(Out, OutSize, szTemp);
	return result;
}

HBITMAP SgImgLib::GetBitmap(sg_uint16 nBitmap)
{
	if ((nBitmap < 1) || (nBitmap > MAX_BITMAPS))return NULL;
	return m_hBitmap[nBitmap - 1];
}

sg_uint32 SgImgLib::GetNumEntries()
{
	return m_ImageData.size();
}


bool SgImgLib::OpenBitmap(LPSTR szFilename, sg_uint16 nBitmap)
{
	return OpenBitmapOffset(szFilename, 0, 0, nBitmap);
}

bool SgImgLib::OpenBitmapOffset(LPCSTR szFilename, sg_uint32 nOffset, sg_uint32 FileSize, sg_uint16 nBitmap)
{
	if ((nBitmap < 1) || (nBitmap > MAX_BITMAPS))return false;

	if (m_DontLoadBms)
	{
		m_hBitmap[nBitmap - 1] = 0;
		m_BmOffsets[nBitmap - 1] = nOffset;
		strcpy_s(m_szBitmapFilenameA[nBitmap - 1], countof(m_szBitmapFilenameA[nBitmap - 1]), szFilename);
	}
	else
	{
		DeleteObject(m_hBitmap[nBitmap - 1]);
		m_hBitmap[nBitmap - 1] = LoadBitmapOffset(szFilename, nOffset, FileSize);

		if (m_hBitmap[nBitmap - 1] == NULL)return false;
		m_BmOffsets[nBitmap - 1] = nOffset;
		strcpy_s(m_szBitmapFilenameA[nBitmap - 1], countof(m_szBitmapFilenameA[nBitmap - 1]), szFilename);
	}
	return true;
}

void SgImgLib::CloseMainBitmaps()
{
	for (int i = 0; i < m_nNumBitmaps; i++)
	{
		if (m_hBitmap[i])DeleteObject(m_hBitmap[i]);
		m_hBitmap[i] = NULL;
	}
	m_nNumBitmaps = 0;
}


void SgImgLib::ClearDataBase()
{
	m_ImageData.resize(0);
	m_nNumBitmaps = 0;
	CloseMainBitmaps();
}


void SgImgLib::CopyImageToDC(HDC hdcDest, sg_uint32 nEntry, int x, int y, BOOL bTransp)
{
	HDC hdcMainBitmap = NULL;

	if ((nEntry < 1) || (nEntry > m_ImageData.size()))return;

	hdcMainBitmap = CreateCompatibleDC(hdcMainBitmap);
	SelectObject(hdcMainBitmap, m_hBitmap[m_ImageData[nEntry - 1].nBitmap - 1]);


	if (!bTransp) {
		StretchBlt(
			hdcDest,
			x,
			y,
			m_ImageData[nEntry - 1].nWidth,
			m_ImageData[nEntry - 1].nHeight,
			hdcMainBitmap,
			m_ImageData[nEntry - 1].nX,
			m_ImageData[nEntry - 1].nY,
			m_ImageData[nEntry - 1].nWidthSrc,
			m_ImageData[nEntry - 1].nHeightSrc,
			SRCCOPY);
	}
	else {
		TransparentBlt2(
			hdcDest,
			x,
			y,
			m_ImageData[nEntry - 1].nWidth,
			m_ImageData[nEntry - 1].nHeight,
			hdcMainBitmap,
			m_ImageData[nEntry - 1].nX,
			m_ImageData[nEntry - 1].nY,
			m_ImageData[nEntry - 1].nWidthSrc,
			m_ImageData[nEntry - 1].nHeightSrc,
			RGB(255, 0, 255));

	}

	DeleteDC(hdcMainBitmap);
}

sg_uint16 SgImgLib::GetNumBitmaps()
{
	return m_nNumBitmaps;
}

void SgImgLib::StretchImageToDC(HDC hdcDest, sg_uint32 nEntry, int x, int y, int nWidth, int nHeight, BOOL bTransp)
{
	HDC hdcMainBitmap = NULL;

	if ((nEntry < 1) || (nEntry > m_ImageData.size()))return;

	hdcMainBitmap = CreateCompatibleDC(hdcMainBitmap);
	SelectObject(hdcMainBitmap, m_hBitmap[m_ImageData[nEntry - 1].nBitmap - 1]);


	if (!bTransp) {
		StretchBlt(
			hdcDest,
			x,
			y,
			nWidth,
			nHeight,
			hdcMainBitmap,
			m_ImageData[nEntry - 1].nX,
			m_ImageData[nEntry - 1].nY,
			m_ImageData[nEntry - 1].nWidthSrc,
			m_ImageData[nEntry - 1].nHeightSrc,
			SRCCOPY);
	}
	else {
		TransparentBlt2(
			hdcDest,
			x,
			y,
			nWidth,
			nHeight,
			hdcMainBitmap,
			m_ImageData[nEntry - 1].nX,
			m_ImageData[nEntry - 1].nY,
			m_ImageData[nEntry - 1].nWidthSrc,
			m_ImageData[nEntry - 1].nHeightSrc,
			RGB(255, 0, 255));

	}

	DeleteDC(hdcMainBitmap);
}

void SgImgLib::GetBitmapName(char* Out, size_t OutSize, sg_uint16 nBitmap)
{
	if ((nBitmap < 1) || (nBitmap > m_nNumBitmaps)) {
		strcpy_s(Out, OutSize, "");
	}
	else {
		strcpy_s(Out, OutSize, m_szBitmapFilenameA[nBitmap - 1]);
	}
}

/***************************************************************************

	SgImgLibArchive - For reading a built SgImgLib.

***************************************************************************/

SgImgLibArchive::SgImgLibArchive(bool DontLoadBms)
	: SgImgLib(DontLoadBms)
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
	if ((nEntry < 1) || (nEntry > m_ImageData.size()))return;

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
	if (dwBytesRead < sizeof(IMGLIBHEADER)) {
		CloseHandle(hFile);
		return false;
	}
	if (lbHeader.wType != *(sg_uint16*)"IL") {
		CloseHandle(hFile);
		return false;
	}
	if (lbHeader.nVersion != 20) {
		CloseHandle(hFile);
		return false;
	}

	//file is correct type so continue
	ReadFile(hFile, &imHeader, sizeof(IMGHEADER), &dwBytesRead, NULL);
	BITMAPDATA bmData[MAX_BITMAPS];
	ReadFile(hFile, &bmData, imHeader.nSizeofBitmapInfo, &dwBytesRead, NULL);
	m_ImageData.resize(imHeader.nEntrys);
	ReadFile(hFile, m_ImageData.data(), imHeader.nSizeofEntryData, &dwBytesRead, NULL);

	CloseHandle(hFile);

	for (i = 1; i <= imHeader.nNumBMs; i++)
	{
		OpenBitmapOffset(szFilename, bmData[i - 1].dwOffset, bmData[i - 1].dwSize, i);
		m_nNumBitmaps++;
	}

	return true;
}

void SgImgLibArchive::CloseArchive()
{
	for (int i = 0; i < m_nNumBitmaps; i++)
	{
		if (m_hBitmap[i]) DeleteObject(m_hBitmap[i]);
		m_hBitmap[i] = NULL;
		m_BmOffsets[i] = 0;
		m_szBitmapFilenameA[i][0] = 0;
	}
	m_ImageData.resize(0);
	m_nNumBitmaps = 0;
}
