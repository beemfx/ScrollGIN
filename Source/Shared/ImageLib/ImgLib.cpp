/*
	ImageLibrary.cpp - The SgImgLib class

	Copyright (c) 2003, Blaine Myers
*/
#include "ImgLib.h"
#include "bitmapex.h"
#include "SgLib/SgFuncs.h"
#include <stdio.h>

SgImgLib::SgImgLib()
{
	
}

SgImgLib::~SgImgLib()
{
	
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

	if (nBitmap > m_SourceImageData.size())
	{
		return false;
	}

	m_SourceImageData[nBitmap - 1].Offset = nOffset;
	m_SourceImageData[nBitmap - 1].FileSize = FileSize;
	m_SourceImageData[nBitmap - 1].Filename = SgFunc_ToWString(szFilename);
	
	return true;
}

sg_uint16 SgImgLib::GetNumBitmaps()
{
	return static_cast<sg_uint16>(m_SourceImageData.size());
}

void SgImgLib::GetBitmapName(char* Out, size_t OutSize, sg_uint16 nBitmap)
{
	if ((nBitmap < 1) || (nBitmap > m_SourceImageData.size())) {
		strcpy_s(Out, OutSize, "");
	}
	else
	{
		std::string Temp = SgFunc_ToMBString(m_SourceImageData[nBitmap - 1].Filename);
		strcpy_s(Out, OutSize, Temp.c_str());
	}
}

/***************************************************************************

	SgImgLibArchive - For reading a built SgImgLib.

***************************************************************************/

SgImgLibArchive::SgImgLibArchive()
	: SgImgLib()
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
	if (lbHeader.wType != IMGLIB_HEADER_ID) {
		CloseHandle(hFile);
		return false;
	}
	if (lbHeader.nVersion != IMGLIB_ILB_VERSION) {
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

	m_SourceImageData.resize(imHeader.nNumBMs);

	for (i = 1; i <= imHeader.nNumBMs; i++)
	{
		OpenBitmapOffset(szFilename, bmData[i - 1].dwOffset, bmData[i - 1].dwSize, i);
	}

	return true;
}

void SgImgLibArchive::CloseArchive()
{
	m_ImageData.resize(0);
	m_SourceImageData.resize(0);
}
