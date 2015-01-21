/* img_lib.h - The Image Loader library.
	Copyright (c) 2006 Blaine Myers*/
#ifndef __IMG_LIB_H__
#define __IMG_LIB_H__

#include "img_lib_types.h"

/* Insure that if a c++ app is calling the img_lib functions
	that it knows they were written in C.  This is for VC++
	and is not ANSI C.*/
#ifdef __cplusplus
extern "C"{
#endif /*__cplusplus*/


/*********************************
	The img_lib public functions.
*********************************/

HIMG IMG_FUNC IMG_OpenMemory(img_void* pData, img_dword nSize);
HIMG IMG_FUNC IMG_OpenA(img_char* szFilename);
HIMG IMG_FUNC IMG_OpenW(img_wchar* szFilename);
img_bool IMG_FUNC IMG_Delete(HIMG hImage);

#ifdef _UNICODE
#define IMG_Open IMG_OpenW
#else
#define IMG_Open IMG_OpenA
#endif

img_bool IMG_FUNC IMG_GetDesc(
	HIMG hImage, 
	IMG_DESC* lpDescript);

img_bool IMG_FUNC IMG_CopyBits(
	HIMG hImage,
	IMG_DEST_RECT* pDest,
	IMGFILTER Filter,
	IMG_RECT* rcSrc,
	img_byte nExtra);

img_bool IMG_FUNC IMG_GetPalette(
	HIMG hImage,
	img_void* lpDataOut);

/* Terminate the extern "C" (see above).*/
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*__IMG_LIB_H__*/