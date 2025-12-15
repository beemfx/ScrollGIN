#ifndef __CONSTLIBRARY_H__
#define __CONSTLIBRARY_H__

#include "ImageLib/ImgLib.h"

class CConstantArchive: public SgImgLibArchive
{
public:
	CConstantArchive(LPCTSTR, int, WORD, WORD);
	~CConstantArchive();
	
	HRESULT StretchImageToDC(HDC hdcDest, DWORD nEntry, int x, int y, int nWidth, int nHeight, BOOL bTransp);

private:
	HBITMAP m_hBitmap = NULL;
};

#endif //__constlibrary_h__