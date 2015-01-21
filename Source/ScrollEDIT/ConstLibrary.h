#ifndef __CONSTLIBRARY_H__
#define __CONSTLIBRARY_H__

#include "ImageLib/ImgLib.h"

class CConstantArchive: public SgImgLibArchive{
private:
public:
	CConstantArchive(LPCTSTR, int, WORD, WORD);
	~CConstantArchive();
	//HRESULT DisplayImage(HDC *hDc, int reference, int x, int y, BOOL bTransp);
	HRESULT StretchImageToDC(HDC hdcDest, DWORD nEntry, int x, int y, int nWidth, int nHeight, BOOL bTransp);

};

#endif //__constlibrary_h__