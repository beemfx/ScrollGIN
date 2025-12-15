// (c) 2025 Beem Media. All rights reserved.

#include "SgWinDCImgLib.h"
#include "ImageLib/bitmapex.h"
#include "SgLib/SgFuncs.h"

bool SgWinDCImgLib::LoadArchive(const char* szFilename)
{
	if (SgImgLibArchive::LoadArchive(szFilename))
	{
		OpenMainBitmaps();
		return true;
	}

	return false;
}

void SgWinDCImgLib::StretchImageToDC(HDC hdcDest, sg_uint32 nEntry, int x, int y, int nWidth, int nHeight, BOOL bTransp)
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

void SgWinDCImgLib::CloseMainBitmaps()
{
	for (auto& Item : m_hBitmap)
	{
		if (Item)
		{
			DeleteObject(Item);
		}
		Item = NULL;
	}
}

void SgWinDCImgLib::OpenMainBitmaps()
{
	CloseMainBitmaps();

	for (std::size_t i = 0; i < m_SourceImageData.size(); i++)
	{
		if (i < countof(m_hBitmap))
		{
			m_hBitmap[i] = LoadBitmapOffset(SgFunc_ToMBString(m_SourceImageData[i].Filename).c_str(), m_SourceImageData[i].Offset, m_SourceImageData[i].FileSize);
		}
	}
}
