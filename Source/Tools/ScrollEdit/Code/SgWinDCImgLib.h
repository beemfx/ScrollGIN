// (c) 2025 Beem Media. All rights reserved.

#pragma once

#include "ImageLib/ImgLib.h"
#include <Windows.h>

class SgWinDCImgLib : public SgImgLibArchive
{
public:
	SgWinDCImgLib() 
		: SgImgLibArchive()
	{
	}

	~SgWinDCImgLib()
	{
		CloseMainBitmaps();
	}

	bool LoadArchive(const char* szFilename);

	void StretchImageToDC(HDC hdcDest, sg_uint32 nEntry, int x, int y, int nWidth, int nHeight, BOOL bTransp);

private:
	HBITMAP m_hBitmap[MAX_BITMAPS] = { };

private:
	void CloseMainBitmaps();
	void OpenMainBitmaps();
};
