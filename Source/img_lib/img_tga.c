/* img_tga.c - The functions for loading a TGA into an HIMG.
	Copyright (C) 2006, Blaine Myers. */

#include <string.h>
#include <malloc.h>
#include "img_private.h"

/***************************************
	Definitions and types used by TGAs.
***************************************/
#define TGA_IMAGE_ORIGIN(p)  (((p)&0x30) >> 4)

/*TGA Header*/
typedef struct _TGAHEADER{
	img_byte nIDSize;
	img_byte nColorMapType;
	img_byte nImageType;
	/* Color Map Info. */
	img_word nFirstCMIndex;
	img_word nNumCMEntries;
	img_byte nCMEntrySize;
	/* Image Specifications. */
	img_word nXOrigin;
	img_word nYOrigin;
	img_word nWidth;
	img_word nHeight;
	img_byte nBitsPerPixel;
	img_byte nImageDesc;
}TGAHEADER, *LPTGAHEADER;

/*TGA Footer*/
typedef struct _TGAFOOTER{
	img_dword nExtensionOffset;
	img_dword nDeveloperOffset;
	char szSignature[18];
}TGAFOOTER, *LPTGAFOOTER;



/*********************************************
	Prototypes for some private TGA functions.
*********************************************/
img_bool TGA_ReadHeaderAndFooter(
	img_void* stream, 
	IMG_CALLBACKS* 
	lpCB, TGAHEADER* pHeader, 
	TGAFOOTER* pFooter);

img_bool TGA_ReadRLEData(
	img_void* stream, 
	IMG_CALLBACKS* lpCB, 
	IMAGE_S* lpImage);

HIMG IMG_LoadTGACallbacks(img_void* stream, IMG_CALLBACKS* lpCB)
{
	IMAGE_S* lpImage=NULL;
	TGAHEADER Header;
	TGAFOOTER Footer;
	int bValid=0;
	char szImageID[255];
	int bFooter=0;

	if(!stream || !lpCB)
		return NULL;


	lpImage=malloc(sizeof(IMAGE_S));
	if(!lpImage)
		return IMG_NULL;

	memset(lpImage, 0, sizeof(IMAGE_S));
	memset(&Header, 0, sizeof(TGAHEADER));
	memset(&Footer, 0, sizeof(TGAFOOTER));

	/* Read the header and footer, this funciton also insures that 
		we have a valid tga file, so if it fails there is no TGA.*/
	if(!TGA_ReadHeaderAndFooter(stream, lpCB, &Header, &Footer))
	{
		free(lpImage);
		return IMG_NULL;
	}

	//Copy the data into our image structure.
	lpImage->nWidth=Header.nWidth;
	lpImage->nHeight=Header.nHeight;
	lpImage->nBitDepth=Header.nBitsPerPixel;
	lpImage->nPaletteBitDepth=Header.nCMEntrySize;
	lpImage->nOrient=TGA_IMAGE_ORIGIN(Header.nImageDesc);
	lpImage->nPaletteEntries=Header.nNumCMEntries;
	lpImage->nDataSize=Header.nWidth*Header.nHeight*Header.nBitsPerPixel/8;
	lpImage->nPaletteSize=Header.nNumCMEntries*Header.nCMEntrySize/8;

	
	// We assume a valid file now and read the id.
	lpCB->seek(stream, 18, IMG_SEEK_SET);

	lpCB->read(
		&szImageID,
		1,
		Header.nIDSize,
		stream);

	// Now read the color map data if it exists.
	if(Header.nColorMapType)
	{
		lpImage->pPalette=malloc(lpImage->nPaletteEntries*lpImage->nPaletteBitDepth/8);
		if(lpImage->pPalette==IMG_NULL)
		{
			free(lpImage);
			return NULL;
		}

		lpCB->read(
			lpImage->pPalette,
			1, 
			lpImage->nPaletteEntries*lpImage->nPaletteBitDepth/8,
			stream);
	}

	// Read the image data.
	lpImage->pImage=malloc(lpImage->nWidth*lpImage->nHeight*lpImage->nBitDepth/8);
	if(!lpImage->pImage)
	{
		IMG_SAFE_FREE(lpImage->pPalette);
		IMG_SAFE_FREE(lpImage);
		return IMG_NULL;
	}
	// Read the data.  If the data is compressed we need to decode.
	if((Header.nImageType >= 9) && (Header.nImageType <=11))
	{
		TGA_ReadRLEData(stream, lpCB, lpImage);
	}
	else
	{
		lpCB->read(
			lpImage->pImage,
			1,
			lpImage->nWidth*lpImage->nHeight*lpImage->nBitDepth/8,
			stream);
	}

	switch(Header.nBitsPerPixel)
	{
	case 8:
	{
		lpImage->nDataFmt=IMGFMT_PALETTE;
		switch(Header.nCMEntrySize)
		{
		case 16:
			lpImage->nPaletteFmt=IMGFMT_X1R5G5B5;
			break;
		case 24:
			lpImage->nPaletteFmt=IMGFMT_R8G8B8;
			break;
		case 32:
			lpImage->nPaletteFmt=IMGFMT_A8R8G8B8;
			break;
		default:
			lpImage->nPaletteFmt=IMGFMT_UNKNOWN;
			break;
		}
		break;
	}
	case 16:
		lpImage->nDataFmt=IMGFMT_X1R5G5B5;
		lpImage->nPaletteFmt=IMGFMT_NONE;
		break;
	case 24:
		lpImage->nDataFmt=IMGFMT_R8G8B8;
		lpImage->nPaletteFmt=IMGFMT_NONE;
		break;
	case 32:
		lpImage->nDataFmt=IMGFMT_A8R8G8B8;
		lpImage->nPaletteFmt=IMGFMT_NONE;
		break;
	default:
		lpImage->nDataFmt=IMGFMT_UNKNOWN;
		lpImage->nPaletteFmt=IMGFMT_UNKNOWN;
		break;
	}

	return (HIMG)lpImage;
}



/*************************
*** Private Functions. ***
*************************/

/* TGA_ReadHeaderAndFooter reads the header and footer (if it exists) of the
	TGA file.  It then checks the data found in the header and footer to make
	sure that the file *is* actually a TGA file.  If the file is not a TGA file
	it returns IMG_FALSE, if it is a TGA file it returns IMG_TRUE.*/
img_bool TGA_ReadHeaderAndFooter(
	img_void* stream, 
	IMG_CALLBACKS* lpCB, 
	TGAHEADER* pHeader, 
	TGAFOOTER* pFooter)
{
	img_bool bHasFooter=IMG_FALSE;
	img_bool bValid=IMG_FALSE;

	lpCB->seek(stream, 0, IMG_SEEK_SET);
	/* Read the Header info. */
	lpCB->read(&pHeader->nIDSize, 1, 1, stream);
	lpCB->read(&pHeader->nColorMapType, 1, 1, stream);
	lpCB->read(&pHeader->nImageType, 1, 1, stream);
	/* Read the Color map info. */
	lpCB->read(&pHeader->nFirstCMIndex, 2, 1, stream);
	lpCB->read(&pHeader->nNumCMEntries, 2, 1, stream);
	lpCB->read(&pHeader->nCMEntrySize, 1, 1, stream);
	/* Read the image specifications. */
	lpCB->read(&pHeader->nXOrigin, 2, 1, stream);
	lpCB->read(&pHeader->nYOrigin, 2, 1, stream);
	lpCB->read(&pHeader->nWidth, 2, 1, stream);
	lpCB->read(&pHeader->nHeight, 2, 1, stream);
	lpCB->read(&pHeader->nBitsPerPixel, 1, 1, stream);
	lpCB->read(&pHeader->nImageDesc, 1, 1, stream);

	
	/* Read the footer, and check to seek if the file is a valid tga.*/
	lpCB->seek(stream, -26, IMG_SEEK_END);

	lpCB->read(&pFooter->nExtensionOffset, 4, 1, stream);
	lpCB->read(&pFooter->nDeveloperOffset, 4, 1, stream);
	lpCB->read(&pFooter->szSignature, 1, 18, stream);
	/* If the file has the TRUEVISION-XFILE tag then it is a valid tga file.*/
	if(_strnicmp("TRUEVISION-XFILE.", pFooter->szSignature, 17)==0)
	{
		bHasFooter=IMG_TRUE;
		bValid=IMG_TRUE;
	}
	else
	{
		pFooter->szSignature[0]=0;
		bHasFooter=IMG_FALSE;
	}

	//Do a series of tests to make sure the file
	//is a valid tga file.
	if(pHeader->nColorMapType==1)
	{
		if(
			(pHeader->nImageType==1) || 
			(pHeader->nImageType==9))
			bValid=IMG_TRUE;
		else
			bValid=IMG_FALSE;
	}
	else if(pHeader->nColorMapType==0)
	{
		if( 
			(pHeader->nImageType==0) ||
			(pHeader->nImageType==2) ||
			(pHeader->nImageType==3) ||
			(pHeader->nImageType==10) ||
			(pHeader->nImageType==11))
			bValid=IMG_TRUE;
		else
			bValid=IMG_FALSE;
	}
	else
	{
		bValid=IMG_FALSE;
	}
	//If the bits per pixel is not evenly divided by
	//8 we can't read the file.  In theory 15 bit tga
	//images exist, but I haven't found one, and they
	//aren't supported by this library.
	if(pHeader->nBitsPerPixel%8)
		bValid=IMG_FALSE;

	return bValid;
}


/* TGAReadRLEData reads RLE data out of the file decompressing it as it goes along.
	lpImage->pImage needs to be allocated to the uncompressed size of the file. Or
	the function will probably cause a crash. */
img_bool TGA_ReadRLEData(img_void* stream, IMG_CALLBACKS* lpCB, IMAGE_S* lpImage)
{
	img_byte nBPP=0;
	img_bool bCompressed=0;
	img_byte nLen=0;
	img_byte nCompress=0;
	img_dword nDecodePos=0;
	img_dword j=0;
	void * lpData=NULL;

	nBPP=lpImage->nBitDepth/8;

	/* Allocate some memory for the input data. */
	lpData=malloc(nBPP);
	if(!lpData)
		return IMG_FALSE;

	nDecodePos=0;
	for(nDecodePos=0; nDecodePos<(unsigned long)(nBPP*lpImage->nWidth*lpImage->nHeight); )
	{
		/* Read the encodeing byte. */
		lpCB->read(&nCompress, 1, 1, stream);
		/* The compression mode is the last bit. */
		bCompressed=(nCompress&0x80)>>7;
		/* The length of the data is the first 7 bits plus 1. */
		nLen=(nCompress&0x7F)+1;
		if(bCompressed)
		{
			/* If compressed we decompress the data. */
			/* Read the compressed data. */
			lpCB->read(lpData, 1, nBPP, stream);
			for(j=0; j<nLen; j++)
			{
				/* Copy the data into the appropriate length of image. */
				memcpy((void*)((unsigned int)lpImage->pImage+nDecodePos), lpData, nBPP);
				nDecodePos+=nBPP;
			}
		}
		else
		{
			/* Otherwize we simply read the data. */
			for(j=0; j<nLen; j++)
			{
				lpCB->read(lpData, 1, nBPP, stream);
				memcpy((void*)((unsigned int)lpImage->pImage+nDecodePos), lpData, nBPP);
				nDecodePos+=nBPP;
			}
		}
	}

	/* Free the memory from the input. */
	free(lpData);
	return IMG_TRUE;
}