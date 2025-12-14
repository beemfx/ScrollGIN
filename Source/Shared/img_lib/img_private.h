#ifndef __IMG_PRIVATE_H__
#define __IMG_PRIVATE_H__
#include "img_lib.h"

//This is the structure of the image format.
//All loaders need to fill this data structure in.
typedef struct _IMAGE_S
{
	img_word  nWidth;          //Image Width
	img_word  nHeight;         //Image Hieght
	img_byte  nBitDepth;       //Color Depth in bits per pixel
	img_byte  nPaletteBitDepth;//Bits per pixel in the palette entries.
	img_word  nPaletteEntries; //Number of palette entries.
	img_dword nDataSize;       //Size of the image data in bytes.
	img_dword nPaletteSize;    //Size of the palette in bytes.
	IMGORIENT nOrient;         //Image Orientation.
	IMGFMT    nDataFmt;        //Image Data Format
	IMGFMT    nPaletteFmt;     //Image Palette Format
	void*     pImage;          //Image Data.
	void*     pPalette;        //Palette Data.
	char*     szDescripton;    //Some user space to put a description of the image.
}IMAGE_S, *PIMAGE_S;

/* The structure for the callback functions used when opening the
	image.*/
typedef struct _IMG_CALLBACKS{
	img_int (IMG_FUNC *close)(img_void* stream);
	img_int (IMG_FUNC *seek)(img_void* stream, img_long offset, img_int origin);
	img_long (IMG_FUNC *tell)(img_void* stream);
	img_uint (IMG_FUNC *read)(img_void* buffer, img_uint size, img_uint count, img_void* stream);
}IMG_CALLBACKS, *PIMG_CALLBACKS;

HIMG IMG_LoadTGACallbacks(img_void* stream, IMG_CALLBACKS* lpCB);
HIMG IMG_LoadBMPCallbacks(img_void* stream, IMG_CALLBACKS* lpCB);

img_void* img_file_open(img_void* pData, img_dword nSize);
img_int img_file_close(img_void* stream);
img_int img_file_seek(img_void* stream, img_long offset, img_int origin);
img_long img_file_tell(img_void* stream);
img_uint img_file_read(img_void* buffer, img_uint size, img_uint count, img_void* stream);

#endif __IMG_PRIVATE_H__