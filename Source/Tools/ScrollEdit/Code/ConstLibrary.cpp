#include "ImageLib/bitmapex.h"
#include "ConstLibrary.h"

//If transparency is defined, architecture will be blitted with
//translucency.  However the program will only be compatible with
//Windows 98 and higher, or Windows 2000 and higher

#define TRANSPARENCY
#ifdef TRANSPARENCY
#pragma comment(lib, "msimg32.lib")
#endif //transparecty


CConstantArchive::CConstantArchive(LPCTSTR szBitmapName, int nNumEntries, WORD nWidth, WORD nHeight)
	: SgImgLibArchive()
{
	m_ImageData.resize(nNumEntries);

	m_nSelectedEntry=1;

	m_hBitmap=(HBITMAP)LoadImage(GetModuleHandle(NULL), szBitmapName, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
	for(int i=0; i<10; i++){
		m_ImageData[i].nHeight=nHeight;
		m_ImageData[i].nWidth=nWidth;
		m_ImageData[i].nX=i*nWidth;
		m_ImageData[i].nY=0;
	}
	for(int i=0; i<9; i++){
		m_ImageData[i+10].nHeight=nHeight;
		m_ImageData[i+10].nWidth=nWidth;
		m_ImageData[i+10].nX=i*nWidth;
		m_ImageData[i+10].nY=nHeight;
	}
	for(int i=0; i<9; i++){
		m_ImageData[i+10+9].nHeight=nHeight;
		m_ImageData[i+10+9].nWidth=nWidth;
		m_ImageData[i+10+9].nX=i*nWidth;
		m_ImageData[i+10+9].nY=nHeight*2;
	}
}

CConstantArchive::~CConstantArchive(){
	
}

HRESULT CConstantArchive::StretchImageToDC(HDC hdcDest, DWORD nEntry, int x, int y, int nWidth, int nHeight, BOOL bTransp){
	HDC hdcMainBitmap=NULL;
	
	if((nEntry<1)||(nEntry>m_ImageData.size()))return S_FALSE;

	hdcMainBitmap=CreateCompatibleDC(hdcDest);
	SelectObject(hdcMainBitmap, m_hBitmap);

	
	if(!bTransp){
		StretchBlt(hdcDest, x, y, nWidth, nHeight, hdcMainBitmap, 
					m_ImageData[nEntry-1].nX,
					m_ImageData[nEntry-1].nY, 
					m_ImageData[nEntry-1].nWidth, 
					m_ImageData[nEntry-1].nHeight, SRCCOPY);
	
	}else{		
		#ifdef TRANSPARENCY
		//Lets alpha blend
		BLENDFUNCTION bf;
		bf.BlendOp=AC_SRC_OVER;
		bf.AlphaFormat=0;
		bf.BlendFlags=0;
		bf.SourceConstantAlpha=100;

		AlphaBlend(hdcDest, x, y, nWidth, nHeight, hdcMainBitmap,
						m_ImageData[nEntry-1].nX,
						m_ImageData[nEntry-1].nY,
						m_ImageData[nEntry-1].nWidth,
						m_ImageData[nEntry-1].nHeight,
						bf);
		
		#else //TRANSPAREnCY

		TransparentBlt2(
			hdcDest,
			x,
			y,
			nWidth,
			nHeight,
			hdcMainBitmap,
			m_ImageData[nEntry-1].nX,
			m_ImageData[nEntry-1].nY,
			m_ImageData[nEntry-1].nWidth,
			m_ImageData[nEntry-1].nHeight,
			RGB(255, 255, 255));

		#endif //TRANSPARENCY
		
	}
	
	
	DeleteDC(hdcMainBitmap);

	return S_OK;
}
