/*
	ImageLib.h - Header for Image Library DLL

	Copyright (c) 2003, Blaine Myers
*/

#pragma once

#include <windows.h>

static const int MAX_BITMAPS = 10;
static const int IMAGE_NAME_LENGTH = 15;
static const int DEFAULT_MAX_ENTRIES = 256;

static const sg_uint16 IMGLIB_HEADER_ID = 19529; // "IL"
static const sg_uint16 IMGLIB_IDA_VERSION = 10;
static const sg_uint16 IMGLIB_ILB_VERSION = 20;


struct IMAGEDATA
{
	sg_uint16 nBitmap = 0; //reference to which bitmap contains the image
	sg_uint16 nX = 0; //x origin on reference bitmap
	sg_uint16 nY = 0; //y origin on reference bitmap
	sg_uint16 nWidthSrc = 0; //width on reference bm
	sg_uint16 nHeightSrc = 0; //height on reference bm
	sg_uint16 nWidth = 0; //width of bitmap
	sg_uint16 nHeight = 0; //height of bitmap
	sg_uint16 nFrames = 0; //number of frames following to make up sprite
	sg_char16 szImgLabel[IMAGE_NAME_LENGTH + 1] = { }; //name given to image convert to multibyte for ansi
};

struct IMGLIBHEADER
{
	sg_uint16 wType = 0; //The type of file IMGLIB_HEADER_ID
	sg_uint16 nVersion = 0; //version 10 for database 20 for library build
	sg_uint32 lReserved1 = 0; //reserved
	sg_uint32 lReserved2 = 0; //reserved
	sg_uint16 nOffsetBytes = 0; //how many bytes offset for the data Header
};

struct IMGDBHEADER
{
	sg_uint32 nEntries = 0; //number of entries in database
	sg_uint16 nNumBMs = 0; //the amount of bitmaps
	sg_uint32 nSizeofEntryData = 0; //size of entry data
};

struct IMGHEADER
{
	sg_uint32 nEntrys = 0; //number of entries
	sg_uint16 nNumBMs = 0; //the amount of bitmaps
	sg_uint32 nSizeofEntryData = 0; //size of entry data
	sg_uint32 nSizeofBitmapInfo = 0; //size of bitmap offset and size info
};

struct BITMAPDATA
{
	sg_uint32 dwOffset = 0;
	sg_uint32 dwSize = 0;
};

class SgImgLib
{
protected:
	std::vector<IMAGEDATA> m_ImageData;
	int m_BmOffsets[MAX_BITMAPS] = { };
	HBITMAP m_hBitmap[MAX_BITMAPS] = { };
	char m_szBitmapFilenameA[MAX_BITMAPS][MAX_PATH] = { };
	sg_uint16 m_nNumBitmaps = 0;
	const bool m_DontLoadBms = false;

public:
	SgImgLib(bool DontLoadBms);
	~SgImgLib();

	sg_uint32 GetNumEntries();
	sg_uint16 GetNumBitmaps();
	void GetBitmapName(char* Out, size_t OutSize, sg_uint16 nBitmap);
	int GetBitmapOffset(sg_uint16 nBitmap)const { return ((nBitmap < 1) || (nBitmap > m_nNumBitmaps)) ? 0 : m_BmOffsets[nBitmap - 1]; }
	void CloseMainBitmaps();
	void ClearDataBase();
	void CopyImageToDC(HDC hdcDest, sg_uint32 nEntry, int x, int y, BOOL bTransp);
	void StretchImageToDC(HDC hdcDest, sg_uint32 nEntry, int x, int y, int nWidth, int nHeight, BOOL bTransp);
	bool GetImageName(char* Out, size_t OutSize, sg_uint32 nEntry);
	sg_uint16 GetNumFrames(sg_uint32 nEntry);
	bool GetImageData(sg_uint32 nEntry, IMAGEDATA* imgData);
protected:
	bool OpenBitmap(LPSTR szFilename, sg_uint16 nBitmap);
	bool OpenBitmapOffset(LPCSTR szFilename, sg_uint32 nOffset, sg_uint32 FileSize, sg_uint16 nBitmap);
};

class SgImgLibArchive : public SgImgLib
{
protected:
	sg_uint32 m_nSelectedEntry;
public:
	SgImgLibArchive(bool DontLoadBms);
	~SgImgLibArchive();

	bool LoadArchive(LPCSTR szFilename);
	sg_uint32 GetSelectedEntry();
	void SetSelectedEntry(sg_uint32 nEntry);
	void CloseArchive();
};
