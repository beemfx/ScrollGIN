#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include <math.h>
#include "img_lib.h"
#include "img_private.h"

/************************
*** Public Functions. ***
************************/

HIMG IMG_FUNC IMG_OpenCallbacks(img_void* stream, IMG_CALLBACKS* lpCB)
{
	HIMG hImage=IMG_NULL;

	if(!hImage)
		hImage=IMG_LoadTGACallbacks(stream, lpCB);
	if(!hImage)
		hImage=IMG_LoadBMPCallbacks(stream, lpCB);
		
	lpCB->close(stream);
	return hImage;
}

HIMG IMG_FUNC IMG_OpenMemory(img_void* pData, img_dword nSize)
{
	HIMG hImage=IMG_NULL;
	img_void* pFile=IMG_NULL;
	IMG_CALLBACKS cb;
	
	pFile=img_file_open(pData, nSize);
	
	if(!pFile)
		return IMG_NULL;
		
	
	cb.close=img_file_close;
	cb.read=img_file_read;
	cb.seek=img_file_seek;
	cb.tell=img_file_tell;
	
	return IMG_OpenCallbacks(pFile, &cb);
}

HIMG IMG_FUNC IMG_OpenA(char* szFilename)
{
	img_dword nSize=0, nRead=0;
	img_void* buffer=IMG_NULL;
	HIMG hTemp=IMG_NULL;
	
	/* Op the file and get it's size. */
	FILE* fin=IMG_NULL;
	fopen_s(&fin, szFilename, "rb");
	if(!fin)
		return IMG_NULL;
	fseek(fin, 0, SEEK_END);
	nSize=ftell(fin);
	fseek(fin, 0, SEEK_SET);

	buffer=malloc(nSize);
	if(!buffer)
	{
		fclose(fin);
		return IMG_NULL;
	}
	
	/* Read the whole file into a buffer. */
	nRead=fread(buffer, 1, nSize, fin);
	fclose(fin);
	if(nRead<nSize)
	{
		free(buffer);
		return IMG_NULL;
	}
	
	/* Call img open memory with buffer, and
		size as parameters to open the image.
	*/
	hTemp=IMG_OpenMemory(buffer, nSize);
	/* The memory must be freed, because it is not
		freed in IMG_OpenMemory. */
	free(buffer);
	
	/* If IMG_OpenMemory failed we will be returning,
		the null from that function so we don't need
		to test here. */
	return hTemp;
}

HIMG IMG_FUNC IMG_OpenW(img_wchar* szFilename)
{
	img_dword nSize=0, nRead=0;
	img_void* buffer=IMG_NULL;
	HIMG hTemp=IMG_NULL;
	
	/* Op the file and get it's size. */
	FILE* fin=IMG_NULL;
	_wfopen_s(&fin, szFilename, L"rb");
	if(!fin)
		return IMG_NULL;
	fseek(fin, 0, SEEK_END);
	nSize=ftell(fin);
	fseek(fin, 0, SEEK_SET);

	buffer=malloc(nSize);
	if(!buffer)
	{
		fclose(fin);
		return IMG_NULL;
	}
	
	/* Read the whole file into a buffer. */
	nRead=fread(buffer, 1, nSize, fin);
	fclose(fin);
	if(nRead<nSize)
	{
		free(buffer);
		return IMG_NULL;
	}
	
	/* Call img open memory with buffer, and
		size as parameters to open the image.
	*/
	hTemp=IMG_OpenMemory(buffer, nSize);
	/* The memory must be freed, because it is not
		freed in IMG_OpenMemory. */
	free(buffer);
	
	/* If IMG_OpenMemory failed we will be returning,
		the null from that function so we don't need
		to test here. */
	return hTemp;
}

img_bool IMG_FUNC IMG_Delete(HIMG hImage)
{
	IMAGE_S* lpImage=(IMAGE_S*)hImage;

	if(!lpImage)
		return IMG_FALSE;

	//Delete the image data, palette, and description.
	IMG_SAFE_FREE(lpImage->pImage);
	IMG_SAFE_FREE(lpImage->pPalette);
	IMG_SAFE_FREE(lpImage->szDescripton);

	//Delete the structure.
	free(hImage);
	return IMG_TRUE;
}

img_bool IMG_FUNC IMG_GetPalette(
	HIMG hImage,
	img_void* lpDataOut)
{
	IMAGE_S* lpImage=(IMAGE_S*)hImage;

	if(!lpDataOut || !lpImage->pPalette)
		return 0;

	memcpy(
		lpDataOut, 
		lpImage->pPalette,//((LPTGAIMAGE)hImage)->ColorMap, 
		lpImage->nPaletteSize);//(((LPTGAIMAGE)hImage)->Header.nCMEntrySize/8)*((LPTGAIMAGE)hImage)->Header.nNumCMEntries);
	
	return 1;
}

#define IMG_scale_float(value, scale) (long)(((float)value)*(scale))
#define IMG_scale_int(value, scale) (long)((((long)value)*(scale))

img_bool IMG_ChangePixelFmt(
	unsigned long* color, 
	IMGFMT prevdepth, 
	IMGFMT newdepth, 
	img_byte extra)
{
	unsigned long r=0, g=0, b=0, a=0;
	unsigned long newcolor=0x00000000l;

	if(!color)
		return 0;

	if(prevdepth==newdepth)
		return 1;

	if(newdepth==IMGFMT_PALETTE)
		return 0;

	switch(prevdepth)
	{
	case IMGFMT_X1R5G5B5:
		/* Using floating point, for the conversion proccess, is more accurate,
			but slower. */
		a=((*color)&0x8000)>>15;
		r=((*color)&0x7C00)>>10;
		g=((*color)&0x03E0)>>5;
		b=((*color)&0x001F)>>0;

		//a*=255;
		a=extra;
		r=IMG_scale_float(r, 255.0f/31.0f);
		g=IMG_scale_float(g, 255.0f/31.0f);
		b=IMG_scale_float(b, 255.0f/31.0f);
		break;
	case IMGFMT_R5G6B5:
		a = extra;
		r = ((*color)&0xF800)>>10;
		g = ((*color)&0x07E0)>>5;
		b = ((*color)&0x001F)>>0;

		a = extra;
		r=IMG_scale_float(r, 255.0f/31.0f);
		g=IMG_scale_float(g, 255.0f/63.0f);
		b=IMG_scale_float(b, 255.0f/31.0f);
	case IMGFMT_A8R8G8B8:
		//Not 100% sure this is getting the right value.
		a=(char)(((*color)&0xFF000000)>>24);
		extra=(char)a;
	case IMGFMT_R8G8B8:
		a=extra;
		r=((*color)&0x00FF0000)>>16;
		g=((*color)&0x0000FF00)>>8;
		b=((*color)&0x000000FF)>>0;
		break;
	case IMGFMT_B8G8R8:
		a=extra;
		b=((*color)&0x00FF0000)>>16;
		g=((*color)&0x0000FF00)>>8;
		r=((*color)&0x000000FF)>>0;
		break;
	case IMGFMT_PALETTE:
		return 0;
		break;
	}

	switch(newdepth)
	{
		case IMGFMT_X1R5G5B5:
			r=IMG_scale_float(r, 31.0f/255.0f);
			g=IMG_scale_float(g, 31.0f/255.0f);
			b=IMG_scale_float(b, 31.0f/255.0f);
			*color=0;
			*color=((a>0?1:0)<<15)|(r<<10)|(g<<5)|(b<<0);
			break;
		case IMGFMT_R5G6B5:
			r=IMG_scale_float(r, 31.0f/255.0f);
			g=IMG_scale_float(g, 63.0f/255.0f);
			b=IMG_scale_float(b, 31.0f/255.0f);
			*color=0;
			*color=(r<<11)|(g<<5)|(b<<0);
			break;
		case IMGFMT_R8G8B8:
		case IMGFMT_A8R8G8B8:
			*color=(a<<24)|(r<<16)|(g<<8)|(b<<0);
			break;
		case IMGFMT_B8G8R8:
			*color=(a<<24)|(b<<16)|(g<<8)|(r<<0);
			break;
	}
	return 1;
}

int IMG_GetPixel(
	HIMG hImage,
	unsigned long* lpPix,
	signed short x,
	signed short y,
	IMGFMT Format,
	unsigned char nExtra)
{
	IMAGE_S* lpImage=hImage;
	IMGORIENT nSrcOrient=lpImage->nOrient;//lpImage?TGA_IMAGE_ORIGIN(lpImage->Header.nImageDesc):0;
	IMGFMT nSrcFormat=0;
	unsigned short iHeight=0, iWidth=0;
	unsigned long nSource=0;
	unsigned long nSourceColor=0, nSourceCMEntry=0;

	if(!lpImage)
		return 0;

	iHeight=lpImage->nHeight;//Header.nHeight;
	iWidth=lpImage->nWidth;//Header.nWidth;

	if(x>=iWidth)
		x=iWidth-1;
	if(y>=iHeight)
		y=iHeight-1;

	if(x<0)
		x=0;
	if(y<0)
		y=0;

	switch(nSrcOrient)
	{
	case IMGORIENT_BOTTOMLEFT:
		nSource=iWidth*(iHeight-y-1) + x;
		break;
	case IMGORIENT_BOTTOMRIGHT:
		nSource=iWidth*(iHeight-1-y) + (iWidth-x-1);
		break;
	case IMGORIENT_TOPLEFT:
		nSource=iWidth*y + x;
		break;
	case IMGORIENT_TOPRIGHT:
		nSource=iWidth*y + (iWidth-x-1);
		break;
	default:
		return 0;
	}

	nSource*=lpImage->nBitDepth/8;//Header.nBitsPerPixel/8;

	memcpy(&nSourceColor, (void*)((unsigned int)lpImage->pImage+nSource), lpImage->nBitDepth/8);
	if(lpImage->nDataFmt==IMGFMT_PALETTE)
	{
		nSourceCMEntry=nSourceColor;
		nSourceColor=0;
		memcpy(&nSourceColor, (void*)((unsigned int)lpImage->pPalette+nSourceCMEntry*(lpImage->nPaletteBitDepth/8)), (lpImage->nPaletteBitDepth/8)); 
		nSrcFormat=lpImage->nPaletteFmt;
	}
	else
		nSrcFormat=lpImage->nDataFmt;
	IMG_ChangePixelFmt(&nSourceColor, nSrcFormat, Format, nExtra);
	if(lpPix)
		*lpPix=nSourceColor;

	return 1;
}


img_bool IMG_GetPixelFilter(
	HIMG hImage,
	unsigned short nSampleLevel,
	unsigned long* lpPix,
	unsigned short x,
	unsigned short y,
	IMGFMT Format,
	unsigned char nExtra)
{
	unsigned long* lpPixels;
	unsigned short over=0, under=0;
	unsigned long i=0;
	unsigned long nCTR=0, nCTG=0, nCTB=0, nCTA=0;
	img_dword nNumPix=0;

	nNumPix=(img_dword)pow((double)2, (double)(nSampleLevel+1));

	lpPixels=malloc(nNumPix*sizeof(unsigned long));

	if(nNumPix<=1 || !lpPixels || nSampleLevel==0)
	{
		if(lpPixels)
			free(lpPixels);

		return IMG_GetPixel(
			hImage,
			lpPix,
			x,
			y,
			Format,
			nExtra);
	}

	for(under=0, i=0; under<(nNumPix/2); under++)
	{
		for(over=0; over<(nNumPix/2); over++, i++)
		{
			if(i>=nNumPix)
				break;
			lpPixels[i]=0;
			IMG_GetPixel(hImage, &lpPixels[i], (short)(x+over/*-nNumPix/4*/), (short)(y+under/*-nNumPix/4*/), IMGFMT_A8R8G8B8, nExtra);
		}
	}

	for(i=0; i<(nNumPix); i++)
	{
		nCTA+=(0xFF000000&lpPixels[i])>>24;
		nCTR+=(0x00FF0000&lpPixels[i])>>16;
		nCTG+=(0x0000FF00&lpPixels[i])>>8;
		nCTB+=(0x000000FF&lpPixels[i])>>0;
	}

	free(lpPixels);

	nCTA/=(nNumPix);
	nCTR/=(nNumPix);
	nCTG/=(nNumPix);
	nCTB/=(nNumPix);
	
	nCTA<<=24;
	nCTR<<=16;
	nCTG<<=8;
	nCTB<<=0;

	if(lpPix)
	{
		*lpPix=(nCTA|nCTR|nCTG|nCTB);
		IMG_ChangePixelFmt(lpPix, IMGFMT_A8R8G8B8, Format, nExtra);
	}
	return 1;
}


img_bool IMG_FUNC IMG_CopyBits(
	HIMG hImage,
	IMG_DEST_RECT* pDest,
	IMGFILTER Filter,
	IMG_RECT* prcSrc,
	img_byte nExtra)
{
	IMAGE_S* lpImage=(IMAGE_S*)hImage;
	img_byte nDestBitDepth=0;
	img_long nSrcWidth=0, nSrcHeight=0;
	float fWidthRatio=0.0f, fHeightRatio=0.0f;
	img_short nFilterLevel=0;
	img_long x=0, y=0;
	img_dword nDest=0;
	img_dword nPix=0, nCMEntry=0;
	float fSrcX=0.0f, fSrcY=0.0f;

	IMG_RECT rcSrc;

	if(!lpImage)
		return IMG_FALSE;

	if(!prcSrc)
	{
		rcSrc.top=rcSrc.left=0;
		rcSrc.right=lpImage->nWidth;
		rcSrc.bottom=lpImage->nHeight;
	}
	else
		rcSrc=*prcSrc;

	//Make sure the source rectangle is valid.
	rcSrc.left=IMG_CLAMP(rcSrc.left, 0, lpImage->nWidth);
	rcSrc.right=IMG_CLAMP(rcSrc.right, 0, lpImage->nWidth);
	rcSrc.top=IMG_CLAMP(rcSrc.top, 0, lpImage->nHeight);
	rcSrc.bottom=IMG_CLAMP(rcSrc.bottom, 0, lpImage->nHeight);
	//Make sure the dest rectangle is valid.
	pDest->rcCopy.left=IMG_CLAMP(pDest->rcCopy.left, 0, (img_long)pDest->nWidth);
	pDest->rcCopy.right=IMG_CLAMP(pDest->rcCopy.right, 0, (img_long)pDest->nWidth);
	pDest->rcCopy.top=IMG_CLAMP(pDest->rcCopy.top, 0, (img_long)pDest->nHeight);
	pDest->rcCopy.bottom=IMG_CLAMP(pDest->rcCopy.bottom, 0, (img_long)pDest->nHeight);
	

	//We need to know the bitdepth of the destination
	//for copying purposes.
	switch(pDest->nFormat)
	{
	case IMGFMT_PALETTE:
		nDestBitDepth=8;
		break;
	case IMGFMT_X1R5G5B5:
	case IMGFMT_R5G6B5:
		nDestBitDepth=16;
		break;
	case IMGFMT_R8G8B8:
		nDestBitDepth=24;
		break;
	case IMGFMT_R8G8B8A8:
	case IMGFMT_A8R8G8B8:
		nDestBitDepth=32;
		break;
	}

	//If the pitch was specified as zero we are going to assume
	//that means that the pitch is the same as the width.
	if(pDest->nPitch==0)
		pDest->nPitch=pDest->nWidth*nDestBitDepth/8;

	nSrcWidth=rcSrc.right-rcSrc.left;
	nSrcHeight=rcSrc.bottom-rcSrc.top;

	//Find the width and height ratio to determine how much stretching should be done.
	fWidthRatio=(float)(nSrcWidth-1)/(float)(pDest->nWidth-1);
	fHeightRatio=(float)(nSrcHeight-1)/(float)(pDest->nHeight-1);

	//If we are doing linear filtering we need to find the average
	//ratio so we can find out how much filtering we need to do.
	if(Filter==IMGFILTER_LINEAR)
	{
		short nAveRatio=(short)(fWidthRatio+fHeightRatio)/2;
		nFilterLevel=0;
		while((nAveRatio/=2)>0)
			nFilterLevel++;
	}
	else
	{
		nFilterLevel=0;
	}


	/* For each pixel for the destination we do the following procedure:*/
	for(y=pDest->rcCopy.top; y<pDest->rcCopy.bottom; y++)
	{
		for(x=pDest->rcCopy.left; x<pDest->rcCopy.right; x++)
		{	
			/* We calculate where the destination pixel will go
				based on what the orientation is. */
			switch(pDest->nOrient)
			{
			case IMGORIENT_BOTTOMLEFT:
				nDest=pDest->nPitch*(pDest->nHeight-(y+1))+x*(nDestBitDepth/8);
				break;
			case IMGORIENT_BOTTOMRIGHT:
				nDest=pDest->nPitch*(pDest->nHeight-1-y) + (pDest->nWidth-x-1)*(nDestBitDepth/8);
				break;
			case IMGORIENT_TOPLEFT:
				nDest=y*pDest->nPitch + x*(nDestBitDepth/8);
				break;
			case IMGORIENT_TOPRIGHT:
				nDest=pDest->nPitch*y + (pDest->nWidth-x-1)*(nDestBitDepth/8);
				break;
			default:
				return 0;
			}
			
			nPix=0x00000000l;
			/* The we calculate where the source pixel would be located.*/
			fSrcX=x*fWidthRatio+rcSrc.left;
			fSrcY=y*fHeightRatio+rcSrc.top;

			/* We get that pixel (the function converts it to the format we want.
				The we copy it to the destination.*/
			IMG_GetPixelFilter(hImage, nFilterLevel, &nPix, (short)fSrcX, (short)fSrcY, pDest->nFormat, nExtra);
			memcpy((void*)((unsigned int)pDest->pImage+nDest), &nPix, nDestBitDepth/8);
		}
	}


	
	return IMG_TRUE;
}

img_bool IMG_FUNC IMG_GetDesc(
	HIMG hImage, 
	IMG_DESC* lpDescript)
{
	IMAGE_S* lpImage=hImage;
	if(!lpImage || !lpDescript)
		return 0;

	lpDescript->Width=lpImage->nWidth;
	lpDescript->Height=lpImage->nHeight;
	lpDescript->Format=lpImage->nDataFmt;
	lpDescript->BitsPerPixel=lpImage->nBitDepth;
	lpDescript->NumPaletteEntries=lpImage->nPaletteEntries;
	lpDescript->PaletteBitDepth=lpImage->nPaletteBitDepth;
	lpDescript->PaletteFormat=lpImage->nPaletteFmt;
	return 1;
}


/*
typedef struct _IMG_CALLBACKS{
	img_int (IMG_FUNC *close)(img_void* stream);
	img_int (IMG_FUNC *seek)(img_void* stream, img_long offset, img_int origin);
	img_long (IMG_FUNC *tell)(img_void* stream);
	img_uint (IMG_FUNC *read)(img_void* buffer, img_uint size, img_uint count, img_void* stream);
}IMG_CALLBACKS, *PIMG_CALLBACKS;
*/