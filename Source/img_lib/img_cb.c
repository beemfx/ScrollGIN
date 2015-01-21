/* img_cb.c - Callback functions for opening the functions. */
#include "img_private.h"
#include <malloc.h>
#include <memory.h>
//img_lib callback functions.
typedef struct _img_file{
	img_dword nSize;
	img_dword nPointer;
	img_void* pData;
}img_file;

img_void* img_file_open(img_void* pData, img_dword nSize)
{
	img_file* pFile=malloc(sizeof(img_file));
	if(!pFile)
		return IMG_NULL;
		
	memset(pFile, 0, sizeof(img_file));
	pFile->nSize=nSize;
	pFile->pData=pData;
	pFile->nPointer=0;
	return pFile;
}
img_int img_file_close(img_void* stream)
{
	img_file* pFile=stream;
	/* It isn't necessary to free pFile->nData.
		becase that data should be freed elsewhere.*/
	if(pFile)free(pFile);
	return 0;
}

img_int img_file_seek(img_void* stream, img_long offset, img_int origin)
{
	img_file* pFile=stream;
	if(!pFile)
		return 0;
	switch(origin)
	{
	case IMG_SEEK_SET:
		pFile->nPointer=offset;
		break;
	case IMG_SEEK_CUR:
		pFile->nPointer+=offset;
		break;
	case IMG_SEEK_END:
		pFile->nPointer=pFile->nSize+offset;
		break;
	}
	
	pFile->nPointer=IMG_CLAMP(pFile->nPointer, 0, pFile->nSize);
	return pFile->nPointer;
}

img_long img_file_tell(img_void* stream)
{
	img_file* pFile=stream;
	if(!pFile)
		return 0;
	return pFile->nPointer;
}

img_uint img_file_read(img_void* buffer, img_uint size, img_uint count, img_void* stream)
{
	img_dword nBytesToRead=0;
	
	img_file* pFile=stream;
	if(!pFile)
		return 0;
		
	nBytesToRead=count*size;
	if((pFile->nPointer+nBytesToRead)>pFile->nSize)
	{
		nBytesToRead=pFile->nSize-pFile->nPointer;
	}
	
	memcpy(buffer, (void*)((img_uint)pFile->pData+pFile->nPointer), nBytesToRead);
	
	pFile->nPointer+=nBytesToRead;
	return nBytesToRead;
		
}