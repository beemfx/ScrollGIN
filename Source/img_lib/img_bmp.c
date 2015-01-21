#include <malloc.h>
#include <memory.h>
#include "img_private.h"

/* Structures used by bitmap files.*/
typedef struct _bmp_header_s
{
	img_word  nType;
	img_dword nSize;
	img_word  nReserved1;
	img_word  nReserved2;
	img_dword nOffsetBits;
}bmp_header_s, *pbmp_header_s;

typedef struct _bmp_info_header_s
{
	img_dword biHeaderSize;
	img_long biWidth;
	img_long  biHeight;
	img_word  biPlanes;
	img_word  biBitCount;
	img_dword biCompression;
	img_dword biSizeImage;
	img_long  biXPelsPerMeter;
	img_long  biYPelsPerMeter;
	img_dword biClrUsed;
	img_dword biClrImportant;
}bmp_info_header_s, *pbmp_info_header_s;


typedef struct _bmp_color_s{
	img_byte b;
	img_byte g;
	img_byte r;
	img_byte a;
}bmp_color_s, *pbmp_color_s;


/* Function prototyes for bitmap files.*/
img_bool BMP_ReadHeaders(
	void* stream, 
	IMG_CALLBACKS* lpCB, 
	bmp_header_s* pHeader, 
	bmp_info_header_s* pIH);

img_bool BMP_Read24BitData(img_void* stream, IMG_CALLBACKS* pCB, IMAGE_S* lpImage);
img_bool BMP_Read8BitData(img_void* stream, IMG_CALLBACKS* pCB, IMAGE_S* lpImage);
img_bool BMP_Read4BitData(img_void* stream, IMG_CALLBACKS* pCB, IMAGE_S* lpImage);
img_bool BMP_Read1BitData(img_void* stream, IMG_CALLBACKS* pCB, IMAGE_S* lpImage);
img_bool BMP_DecodeRLE8(img_void* stream, IMG_CALLBACKS* pCB, IMAGE_S* lpImage);
img_bool BMP_DecodeRLE4(img_void* stream, IMG_CALLBACKS* pCB, IMAGE_S* lpImage);

/* The loading function.*/
HIMG IMG_LoadBMPCallbacks(img_void* stream, IMG_CALLBACKS* lpCB)
{
	IMAGE_S* lpImage=IMG_NULL;

	bmp_header_s Header;
	bmp_info_header_s InfoHeader;

	memset(&Header, 0, sizeof(bmp_header_s));
	memset(&InfoHeader, 0, sizeof(bmp_info_header_s));
	
	//Seek to the beginning of the file.
	lpCB->seek(stream, 0, IMG_SEEK_SET);
	if(!BMP_ReadHeaders(stream, lpCB, &Header, &InfoHeader))
		return IMG_FALSE;

	lpImage=malloc(sizeof(IMAGE_S));

	if(!lpImage)
		return IMG_NULL;

	memset(lpImage, 0, sizeof(IMAGE_S));
	lpImage->nWidth=(img_word)InfoHeader.biWidth;
	lpImage->nHeight=(img_word)InfoHeader.biHeight;
	lpImage->nBitDepth=(img_byte)InfoHeader.biBitCount;
	lpImage->nOrient=IMGORIENT_BOTTOMLEFT;

	switch(InfoHeader.biBitCount)
	{
	case 24:
		lpImage->nBitDepth=24;
		lpImage->nPaletteBitDepth=0;
		lpImage->nPaletteEntries=0;
		lpImage->nPaletteFmt=IMGFMT_NONE;
		lpImage->nDataFmt=IMGFMT_R8G8B8;
		break;
	case 8:
		lpImage->nBitDepth=8;
		lpImage->nPaletteBitDepth=32;
		lpImage->nPaletteFmt=IMGFMT_A8R8G8B8;
		lpImage->nPaletteEntries=256;
		lpImage->nDataFmt=IMGFMT_PALETTE;
		break;
	//Note that we will be converting all formats less than,
	//1 byte wide to 1 byte wide (8 bit).
	case 4:
		lpImage->nBitDepth=8;
		lpImage->nPaletteBitDepth=32;
		lpImage->nPaletteFmt=IMGFMT_A8R8G8B8;
		lpImage->nPaletteEntries=16;
		lpImage->nDataFmt=IMGFMT_PALETTE;
		break;
	case 2:
		lpImage->nBitDepth=8;
		lpImage->nPaletteBitDepth=32;
		lpImage->nPaletteFmt=IMGFMT_A8R8G8B8;
		lpImage->nPaletteEntries=4;
		lpImage->nDataFmt=IMGFMT_PALETTE;
		break;
	case 1:
		lpImage->nBitDepth=8;
		lpImage->nPaletteBitDepth=32;
		lpImage->nPaletteFmt=IMGFMT_A8R8G8B8;
		lpImage->nPaletteEntries=2;
		lpImage->nDataFmt=IMGFMT_PALETTE;
		break;
	default:
		lpImage->nBitDepth=0;
		lpImage->nPaletteBitDepth=0;
		lpImage->nPaletteFmt=IMGFMT_UNKNOWN;
		lpImage->nPaletteEntries=0;
		lpImage->nDataFmt=IMGFMT_UNKNOWN;
		break;
	}

	if(lpImage->nPaletteFmt==IMGFMT_UNKNOWN && lpImage->nDataFmt==IMGFMT_UNKNOWN)
	{
		free(lpImage);
		return IMG_NULL;
	}

	lpImage->nDataSize=lpImage->nWidth*lpImage->nHeight*lpImage->nBitDepth/8;
	lpImage->nPaletteSize=lpImage->nPaletteEntries*lpImage->nPaletteBitDepth/8;

	//Allocate memory for the palette and read the palette if there is one.
	if(lpImage->nDataFmt==IMGFMT_PALETTE)
	{
		bmp_color_s* pPalette=IMG_NULL;
		img_dword i=0;
		
		lpImage->pPalette=malloc(lpImage->nPaletteSize);
		if(!lpImage->pPalette)
		{
			free(lpImage);
			return IMG_NULL;
		}

		lpCB->read(lpImage->pPalette, 1, lpImage->nPaletteSize, stream);
		//We need to change the alpha channel on our palette images, cause
		//by default bitmaps ar at 0 which is completely transparent.
		pPalette=(bmp_color_s*)lpImage->pPalette;
		for(i=0; i<lpImage->nPaletteEntries; i++)
		{
			pPalette[i].a=0xFF;
		}
	}

	//Now allocate memory for the image and read it, not that we need to convert
	//for file formats.  (We'll allocate an extra scanline, just becuse of some 
	//bitmap formats that don't line up with 1 byte wide (namely 1 bit and 4 bit
	//bitmaps.)
	lpImage->pImage=malloc(lpImage->nDataSize+lpImage->nWidth*lpImage->nBitDepth/8);
	if(!lpImage->pImage)
	{
		IMG_SAFE_FREE(lpImage->pPalette);
		free(lpImage);
		return IMG_FALSE;
	}
	
	//Go to bitmap information.
	lpCB->seek(stream, Header.nOffsetBits, IMG_SEEK_SET);

	//Reading the data gets a little complicated.
	switch(InfoHeader.biBitCount)
	{
	case 24:
		BMP_Read24BitData(stream, lpCB, lpImage);
		break;
	case 8:
		if(InfoHeader.biCompression==1)
			BMP_DecodeRLE8(stream, lpCB, lpImage);
		else
			BMP_Read8BitData(stream, lpCB, lpImage);
			
		break;
	case 4:
		if(InfoHeader.biCompression==2)
			BMP_DecodeRLE4(stream, lpCB, lpImage);
		else
			BMP_Read4BitData(stream, lpCB, lpImage);
		break;
	case 2:
		break;
	case 1:
		BMP_Read1BitData(stream, lpCB, lpImage);
		break;
	}

	return (HIMG)lpImage;
}

/* BMP_ReadHeaders reads the headers, returns IMG_FALSE if the data read
	is not a valid bitmap file.*/
img_bool BMP_ReadHeaders(
	img_void* stream, 
	IMG_CALLBACKS* lpCB, 
	bmp_header_s* pHeader, 
	bmp_info_header_s* pIH)
{
	lpCB->seek(stream, 0, IMG_SEEK_SET);

	lpCB->read(&pHeader->nType, 2, 1, stream);
	lpCB->read(&pHeader->nSize, 4, 1, stream);
	lpCB->read(&pHeader->nReserved1, 2, 1, stream);
	lpCB->read(&pHeader->nReserved2, 2, 1, stream);
	lpCB->read(&pHeader->nOffsetBits, 4, 1, stream);

	/* Make sure this is a valid bitmap. */
	if(pHeader->nType != *(unsigned short*)"BM")
		return IMG_FALSE;

	lpCB->read(&pIH->biHeaderSize, 4, 1, stream);
	lpCB->read(&pIH->biWidth, 4, 1, stream);
	lpCB->read(&pIH->biHeight, 4, 1, stream);
	lpCB->read(&pIH->biPlanes, 2, 1, stream);
	lpCB->read(&pIH->biBitCount, 2, 1, stream);
	lpCB->read(&pIH->biCompression, 4, 1, stream);
	lpCB->read(&pIH->biSizeImage, 4, 1, stream);
	lpCB->read(&pIH->biXPelsPerMeter, 4, 1, stream);
	lpCB->read(&pIH->biYPelsPerMeter, 4, 1, stream);
	lpCB->read(&pIH->biClrUsed, 4, 1, stream);
	lpCB->read(&pIH->biClrImportant, 4, 1, stream);
	/*
	{
	char szTemp[2048];
	sprintf(szTemp, "Header Size: %d\nWidth: %d\nHieght: %d\nPlanes: %d\nBit Count: %d\nCompression: %d\nSize Image: %d\nXPelsPerMeter: %d\nYPelsPerMeter: %d\nClrUsed: %d\nClrImportant: %d",
		pIH->biHeaderSize,
		pIH->biWidth,
		pIH->biHeight,
		pIH->biPlanes,
		pIH->biBitCount,
		pIH->biCompression,
		pIH->biSizeImage,
		pIH->biXPelsPerMeter,
		pIH->biYPelsPerMeter,
		pIH->biClrUsed,
		pIH->biClrImportant);
		
	MessageBoxA(0, szTemp, 0, 0);
	}
	*/
	if((pIH->biHeaderSize != 40) || (pIH->biPlanes!=1))
	{
		return IMG_FALSE;
	}

	return IMG_TRUE;
}

/* The following functions read the image data in various bitdepths,
	they assume that the file pointer for stream is at the beginning
	of the data.  Note that images with a bitdepth less than 8 are
	converted to 8 bit.*/
img_bool BMP_Read24BitData(img_void* stream, IMG_CALLBACKS* pCB, IMAGE_S* lpImage)
{
	img_dword i=0, j=0, nOffset=0;
	
	img_dword nLineWidth=0;
	void* pTempLine=IMG_NULL;


	//Find the width of each scanline.
	nLineWidth=lpImage->nWidth*3;
	while(nLineWidth%4)
		nLineWidth++;

	pTempLine=malloc(nLineWidth);
	if(!pTempLine)
		return IMG_FALSE;

	for(i=0; i<lpImage->nHeight; i++)
	{
		//Read each scanline.
		pCB->read(pTempLine, 1, nLineWidth, stream);
		//And copy it into the image data.
		nOffset=i*lpImage->nWidth*3;
		memcpy(
			(void*)((int)lpImage->pImage+nOffset),
			(int*)pTempLine,
			lpImage->nWidth*3);
	}

	IMG_SAFE_FREE(pTempLine);
	return IMG_TRUE;
}


img_bool BMP_Read8BitData(img_void* stream, IMG_CALLBACKS* pCB, IMAGE_S* lpImage)
{
	img_dword i=0, j=0, nOffset=0;
	
	img_dword nLineWidth=0;
	void* pTempLine=IMG_NULL;


	//Find the width of each scanline.
	nLineWidth=lpImage->nWidth;
	//We need to dword align the image width.
	while(nLineWidth%4)
		nLineWidth++;

	pTempLine=malloc(nLineWidth);
	
	if(!pTempLine)
		return IMG_FALSE;


	for(i=0; i<lpImage->nHeight; i++)
	{
		//Read each scanline.
		pCB->read(pTempLine, 1, nLineWidth, stream);
		//And copy it into the image data.
		nOffset=i*lpImage->nWidth;
	
		memcpy(
			(void*)((int)lpImage->pImage+nOffset),
			(int*)pTempLine,
			lpImage->nWidth);
	}
	
	IMG_SAFE_FREE(pTempLine);
	return IMG_TRUE;
}

//The following code is not solid and could easily cause a crash if
//corrupted data is input.
img_bool BMP_DecodeRLE8(img_void* stream, IMG_CALLBACKS* pCB, IMAGE_S* lpImage)
{
	img_byte* pDest=lpImage->pImage;
	img_byte nCount=0, nCode=0;
	img_dword i=0;
	img_word nLine=0;
	
	while(IMG_TRUE)
	{
		pCB->read(&nCount, 1, 1, stream);
		pCB->read(&nCode, 1, 1, stream);
		
		if(nCount==0) //Escape code.
		{
			if(nCode==0) //End of line.
			{
				nLine++;
				pDest=(img_byte*)lpImage->pImage+nLine*lpImage->nWidth;
				//Technically if there is anything left in the line it should
				//be assigned the background color.
			}
			else if(nCode==1) //End of bitmap code.
			{
				//Break out of loop.
				break;
			}
			else if(nCode==2)
			{
				//Not really sure if this code is correct, haven't encountered a bitmap
				//that uses it.
				img_byte x, y;
				pCB->read(&x, 1, 1, stream);
				pCB->read(&y, 1, 1, stream);
				nLine+=y;
				pDest+=x+y*lpImage->nWidth;
			}
			else //Absolute mode.
			{
				pCB->read(pDest, 1, nCode, stream);
				pDest+=nCode;
				
				//Seek to make sure we're word aligned.
				pCB->seek(stream, nCode%2, IMG_SEEK_CUR);
			}
		}
		else //Encode mode.
		{
			for(i=0; i<nCount; i++)
			{
				*pDest=nCode;
				pDest++;
			}
		}
	}
	return IMG_TRUE;
}


img_bool BMP_Read4BitData(img_void* stream, IMG_CALLBACKS* pCB, IMAGE_S* lpImage)
{
	img_dword i=0, j=0, nOffset=0;
	
	img_dword nLineWidth=0;
	void* pTempLine=IMG_NULL;
	img_byte n2Pix=0;
	img_byte nPix[2]={0, 0};

	//Find the width of each scanline.
	nLineWidth=lpImage->nWidth;
	if(nLineWidth%2)
	{
		nLineWidth/=2;
		nLineWidth++;
	}
	else
		nLineWidth/=2;

	while(nLineWidth%4)
		nLineWidth++;

	pTempLine=malloc(nLineWidth);
	if(!pTempLine)
		return IMG_FALSE;

	for(i=0; i<lpImage->nHeight; i++)
	{
		//Read each scanline.
		pCB->read(pTempLine, 1, nLineWidth, stream);
		//we have to change the pixel format to 8 bits.
		
		nOffset=i*lpImage->nWidth;
		for(j=0; j<lpImage->nWidth; j+=2)
		{
			img_dword nBytesToCopy=0;

			memcpy(&n2Pix, (void*)((int)pTempLine+j/2), 1);
			nPix[0]=(n2Pix&0xF0)>>4;
			nPix[1]=(n2Pix&0x0F)>>0;

			//For images with less than 8 bits we need to make sure 
			//we're not writing past the edge of the scanline.
			nBytesToCopy=2;
			if((j+2)>lpImage->nWidth)
			{
				nBytesToCopy=2-(lpImage->nWidth-j);
			}

			memcpy(
				(void*)((int)lpImage->pImage+nOffset+j), 
				&nPix[0],
				nBytesToCopy);
		}
	}

	IMG_SAFE_FREE(pTempLine);
	return IMG_TRUE;
}

img_bool BMP_DecodeRLE4(img_void* stream, IMG_CALLBACKS* pCB, IMAGE_S* lpImage)
{
	img_byte* pDest=lpImage->pImage;
	img_byte nCount=0, nCode=0;
	img_dword i=0;
	img_word nLine=0;
	
	while(IMG_TRUE)
	{
		pCB->read(&nCount, 1, 1, stream);
		pCB->read(&nCode, 1, 1, stream);
		
		if(nCount==0) //Escape code.
		{
			if(nCode==0) //End of line.
			{
				nLine++;
				pDest=(img_byte*)lpImage->pImage+nLine*lpImage->nWidth;
				//Technically if there is anything left in the line it should
				//be assigned the background color.
			}
			else if(nCode==1) //End of bitmap code.
			{
				//Break out of loop.
				break;
			}
			else if(nCode==2)
			{
				//Not really sure if this code is correct, haven't encountered a bitmap
				//that uses it.
				img_byte x, y;
				pCB->read(&x, 1, 1, stream);
				pCB->read(&y, 1, 1, stream);
				nLine+=y;
				pDest+=x+y*lpImage->nWidth;
			}
			else //Absolute mode.
			{
				
				//This isn't right, it doesn't convert to 8 bit.
				pCB->read(pDest, 1, nCode, stream);
				pDest+=nCode;
				//Seek to make sure we're word aligned.
				pCB->seek(stream, nCode%2, IMG_SEEK_CUR);
				
			}
		}
		else //Encode mode.
		{
			for(i=0; i<nCount; i++)
			{
				if((i%2))
					*pDest=nCode&0x0F;	
				else
					*pDest=(nCode&0xF0)>>4;
					
				pDest++;
			}
		}
	}
	return IMG_TRUE;
}

img_bool BMP_Read1BitData(img_void* stream, IMG_CALLBACKS* pCB, IMAGE_S* lpImage)
{
	img_dword i=0, j=0, nOffset=0;
	
	img_dword nLineWidth=0;
	void* pTempLine=IMG_NULL;
	img_byte n8Pix=0;
	img_byte nPix[8]={0, 0, 0, 0, 0, 0, 0, 0};


	//Find the width of each scanline.
	//nLineWidth=lpImage->nWidth/8;
	nLineWidth=lpImage->nWidth;
	if(nLineWidth%8)
	{
		nLineWidth/=8;
		nLineWidth++;
	}
	else
		nLineWidth/=8;

	while(nLineWidth%4)
		nLineWidth++;

	pTempLine=malloc(nLineWidth);
	if(!pTempLine)
		return IMG_FALSE;

	for(i=0; i<lpImage->nHeight; i++)
	{
		//Read each scanline.
		pCB->read(pTempLine, 1, nLineWidth, stream);
		//we have to change the pixel format to 8 bits.
		
		nOffset=i*lpImage->nWidth;
		for(j=0; j<lpImage->nWidth; j+=8)
		{
			img_dword nBytesToCopy=0;
			memcpy(&n8Pix, (void*)((int)pTempLine+j/8), 1);
			nPix[0]=(n8Pix&0x80)>>7;
			nPix[1]=(n8Pix&0x40)>>6;
			nPix[2]=(n8Pix&0x20)>>5;
			nPix[3]=(n8Pix&0x10)>>4;
			nPix[4]=(n8Pix&0x08)>>3;
			nPix[5]=(n8Pix&0x04)>>2;
			nPix[6]=(n8Pix&0x02)>>1;
			nPix[7]=(n8Pix&0x01)>>0;
	
			//For less than 8 bit images we need to make sure that
			//we're not writing past the end of the scanline.
			nBytesToCopy=8;
			if((j+8)>lpImage->nWidth)
			{
				nBytesToCopy=8-(lpImage->nWidth-j);
			}
			memcpy(
				(void*)((int)lpImage->pImage+nOffset+j), 
				&nPix[0],
				nBytesToCopy);
		}
	}

	IMG_SAFE_FREE(pTempLine);
	return IMG_TRUE;
}

