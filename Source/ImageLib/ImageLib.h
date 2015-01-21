/*
	ImageLib.h - Header for Image Library DLL

	Copyright (c) 2003, Blaine Myers
*/
#ifndef __IMAGELIB_H__
#define __IMAGELIB_H__
#include <windows.h>

static const int MAX_BITMAPS = 10;
static const int IMAGE_NAME_LENGTH=15;
static const int DEFAULT_MAX_ENTRIES=256;

struct IMAGEDATA
{
	sg_uint16 nBitmap; //reference to which bitmap contains the image
	sg_uint16 nX; //x origin on reference bitmap
	sg_uint16 nY; //y origin on reference bitmap
	sg_uint16 nWidthSrc; //width on reference bm
	sg_uint16 nHeightSrc; //height on reference bm
	sg_uint16 nWidth; //width of bitmap
	sg_uint16 nHeight; //height of bitmap
	sg_uint16 nFrames; //number of frames following to make up sprite
	sg_char16 szImgLabel[IMAGE_NAME_LENGTH+1]; //name given to image convert to multibyte for ansi
};

struct IMGLIBHEADER
{
	sg_uint16	wType; //The type of file "IL"
	sg_uint16	nVersion; //version 10 for database 20 for library build
	sg_uint32	lReserved1; //reserved
	sg_uint32	lReserved2; //reserved
	sg_uint16	nOffsetBytes; //how many bytes offset for the data Header
};

struct IMGDBHEADER
{
	sg_uint32 nEntries; //number of entries in database
	sg_uint16 nNumBMs; //the amount of bitmaps
	sg_uint32 nSizeofEntryData; //size of entry data
};


struct IMGHEADER
{
	sg_uint32	nEntrys; //number of entries
	sg_uint16 nNumBMs; //the amount of bitmaps
	sg_uint32	nSizeofEntryData; //size of entry data
	sg_uint32 nSizeofBitmapInfo; //size of bitmap offset and size info
};

struct BITMAPDATA
{
	sg_uint32 dwOffset;
	sg_uint32 dwSize;
};


class CImageLibrary
{
protected:
	sg_uint32 m_nNumImages;
	IMAGEDATA *m_pImageData;
	HBITMAP   m_hBitmap[MAX_BITMAPS];
	char      m_szBitmapFilenameA[MAX_BITMAPS][MAX_PATH];
	sg_uint16 m_nNumBitmaps;
	
public:
	CImageLibrary();
	~CImageLibrary();

	sg_uint32 GetNumEntries();
	sg_uint16 GetNumBitmaps();
	void      GetBitmapName(char* Out, size_t OutSize, sg_uint16 nBitmap);
	HBITMAP   GetBitmap(sg_uint16 nBitmap);
	void      CloseMainBitmaps();
	void      ClearDataBase();
	void      CopyImageToDC(HDC hdcDest, sg_uint32 nEntry, int x, int y, BOOL bTransp);
	void      StretchImageToDC(HDC hdcDest, sg_uint32 nEntry, int x, int y, int nWidth, int nHeight, BOOL bTransp);
	bool      GetImageName(char* Out, size_t OutSize, sg_uint32 nEntry);
	sg_uint16 GetNumFrames(sg_uint32 nEntry);
	bool      GetImageData(sg_uint32 nEntry, IMAGEDATA *imgData);
protected:
	bool      OpenBitmap(LPSTR szFilename, sg_uint16 nBitmap);
	bool      OpenBitmapOffset(LPCSTR szFilename, sg_uint32 nOffset, sg_uint16 nBitmap);
};

class CEditImageLibrary: public CImageLibrary
{
protected:
	sg_uint32 m_nMaxEntries;
public:
	CEditImageLibrary();
	CEditImageLibrary(sg_uint32 nMaxEntries);
	~CEditImageLibrary();

	bool    AddBitmap(LPSTR szFilename);
	bool    AddEntry(sg_uint16 x, sg_uint16 y, sg_uint16 nWidthSrc, sg_uint16 nHeightSrc, sg_uint16 nWidth, sg_uint16 nHeight, sg_uint16 nBitmap, sg_uint16 nFrames, LPSTR szImageName);
	bool    SetEntry(sg_uint32 nReference, sg_uint16 x, sg_uint16 y, sg_uint16 nWidthSrc, sg_uint16 nHeightSrc, sg_uint16 nWidth, sg_uint16 nHeight, sg_uint16 nBitmap, sg_uint16 nFrames, LPSTR szImageName);
	void    SetNumEntries(sg_uint32 nNumEntries);
	bool    AutoGenerateData(sg_uint16 nBitmap, sg_uint16 nWidth, sg_uint16 nHeight, sg_uint16 nXSpacing, sg_uint16 nYSpacing, BOOL bClear);
	void    ClearDataBase();
	bool    SaveData(LPSTR szFilename);
	bool    LoadData(LPSTR szFilename);
	bool    BuildLibrary(LPSTR szFilename);
	bool    ImportLibrary(LPSTR szFilename);
};

class CImageArchive: public CImageLibrary
{
protected:
	sg_uint32 m_nSelectedEntry;
public:
	CImageArchive();
	~CImageArchive();

	bool      LoadArchive(LPCSTR szFilename);
	sg_uint32 GetSelectedEntry();
	void      SetSelectedEntry(sg_uint32 nEntry);
	void      CloseArchive();
};

#endif //__IMAGELIB_H__