#if __DXVERSION__ == 7
#include "RendererImage.h"
#include <ddraw.h>
#include "img_lib/img_lib.h"

static const DWORD RENDER_IMAGE_TRANSPARENT_COLOR = 0xFFFF00FF;

struct SgRendererImage::sgData
{
	IDirectDrawSurface7*       Surface;
	sgRendererImageCreateParms CreateParms;
	sgRendererData             RendererData;
	HBITMAP                    Bitmap; //Bitmap compatible version of image
};

SgRendererImage::SgRendererImage( const sgRendererImageCreateParms* CreateParms , const sgRendererData* RendererData )
{
	m_D = new sgData;
	m_D->CreateParms  = *CreateParms;
	m_D->RendererData = *RendererData;

	//Generate surface descriptors and create surface
	DDSURFACEDESC2 ddsd;
	memset( &ddsd , 0 , sizeof(ddsd) );
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth = m_D->CreateParms.Width;
	ddsd.dwHeight = m_D->CreateParms.Height;

	if( ddsd.dwWidth == IMAGE_CREATE_DEFAULT )
	{
		ddsd.dwWidth = m_D->CreateParms.Width;
	}

	if( ddsd.dwHeight == IMAGE_CREATE_DEFAULT )
	{
		ddsd.dwHeight = m_D->CreateParms.Height;
	}

	//create the surface
	HRESULT Res = m_D->RendererData.Dd->CreateSurface( &ddsd , &m_D->Surface , NULL );

	if( FAILED(Res) )
		return;

	//Set transparent color 
	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue = ddck.dwColorSpaceHighValue = RENDER_IMAGE_TRANSPARENT_COLOR;
	m_D->Surface->SetColorKey(DDCKEY_SRCBLT, &ddck);

	Restore();
}

SgRendererImage::~SgRendererImage()
{
	if( m_D->Surface )m_D->Surface->Release();
	delete m_D;
}

void SgRendererImage::CreateBitmap()
{
	int SrcWidth = m_D->CreateParms.BmWidth;
	int SrcHeight = m_D->CreateParms.BmHeight;
	int SrcX = m_D->CreateParms.BmX;
	int SrcY = m_D->CreateParms.BmY;
	int DestWidth = m_D->CreateParms.Width;
	int DestHeight = m_D->CreateParms.Height;

	HANDLE File = CreateFileA(m_D->CreateParms.BmFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if( INVALID_HANDLE_VALUE == File )
	{
		return;
	}

	DWORD DataSize = GetFileSize( File , NULL );
	sg_uint8* Data = new sg_uint8[DataSize];
	if( !Data )
	{
		CloseHandle( File );
		return;
	}
	DWORD SizeRead = 0;
	BOOL ReadStatus = ReadFile( File , Data , DataSize , &SizeRead , NULL );
	CloseHandle( File );
	if( !ReadStatus || SizeRead != DataSize )
	{
		delete [] Data;
		return;
	}

	HIMG Img = IMG_OpenMemory( &Data[m_D->CreateParms.BmFileOffset] , DataSize-m_D->CreateParms.BmFileOffset );
	delete [] Data;
	Data = 0;
	if( IMG_NULL == Img )
	{
		return;
	}

	IMG_DESC Desc;
	IMG_GetDesc( Img , &Desc );

	if( SrcWidth == IMAGE_CREATE_DEFAULT )
	{
		SrcWidth = Desc.Width;
	}

	if( SrcHeight == IMAGE_CREATE_DEFAULT )
	{
		SrcHeight = Desc.Height;
	}

	if( SrcX == IMAGE_CREATE_DEFAULT )
	{
		SrcX = 0;
	}

	if( SrcY == IMAGE_CREATE_DEFAULT )
	{
		SrcY = 0;
	}

	if( DestWidth == IMAGE_CREATE_DEFAULT )
	{
		DestWidth = m_D->CreateParms.Width;
	}

	if( DestHeight == IMAGE_CREATE_DEFAULT )
	{
		DestHeight = m_D->CreateParms.Height;
	}

	DDSURFACEDESC2 LockDesc;
	memset( &LockDesc , 0 , sizeof(LockDesc) );
	LockDesc.dwSize = sizeof( LockDesc );	
	if( SUCCEEDED( m_D->Surface->Lock( NULL , &LockDesc , DDLOCK_WAIT , NULL ) ) )
	{
		if( LockDesc.ddpfPixelFormat.dwRGBBitCount == 32 )
		{
			IMG_DEST_RECT DestRect;
			DestRect.nFormat = IMGFMT_A8R8G8B8;
			DestRect.nWidth  = LockDesc.dwWidth;
			DestRect.nHeight = LockDesc.dwHeight;
			DestRect.nPitch  = LockDesc.lPitch;
			DestRect.nOrient = IMGORIENT_TOPLEFT;
			DestRect.pImage  = LockDesc.lpSurface;

			DestRect.rcCopy.top    = 0;
			DestRect.rcCopy.left   = 0;
			DestRect.rcCopy.bottom = LockDesc.dwHeight;
			DestRect.rcCopy.right  = LockDesc.dwWidth;

			IMG_RECT SrcRect;
			SrcRect.top = SrcY;
			SrcRect.bottom = SrcHeight+SrcY;
			SrcRect.left = SrcX;
			SrcRect.right = SrcWidth+SrcX;

			img_bool Copied = IMG_CopyBits( Img , &DestRect , IMGFILTER_NONE , &SrcRect , 0xFF );
		}
		m_D->Surface->Unlock( NULL );
	}

	IMG_Delete( Img );
	Img = 0;
}

void SgRendererImage::CreateColor()
{
	//check to make sure surface exists
	if (!m_D->Surface)return;

	DDBLTFX ddbltfx;
	memset(&ddbltfx, 0, sizeof(ddbltfx));
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwFillColor = m_D->CreateParms.Color.Color;

	m_D->Surface->Blt(NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx);
}

void SgRendererImage::Restore()
{
	switch (m_D->CreateParms.Type)
	{
	case RENDERER_IMAGE_BITMAP: CreateBitmap(); break;
	case RENDERER_IMAGE_COLOR : CreateColor(); break;
	}
}

void SgRendererImage::Draw( int x , int y )
{
	//IDirectDraw7* Dd = m_D->RendererData.Dd;
	IDirectDrawSurface7* Bb = m_D->RendererData.BackSurface;
	if (!Bb || !m_D->Surface)return;

	RECT rcSrc, rcDest;

	DDSURFACEDESC2 ddBufferDesc;
	ZeroMemory(&ddBufferDesc, sizeof(ddBufferDesc));
	ddBufferDesc.dwSize = sizeof(DDSURFACEDESC2);
	if (FAILED(Bb->GetSurfaceDesc(&ddBufferDesc)))return;

	int nBufferWidth = ddBufferDesc.dwWidth;
	int nBufferHeight = ddBufferDesc.dwHeight;

	//if we don't need to clip we do a regular blt
	if (
		//withing the width
		(x >= 0) &&
		((m_D->CreateParms.Width + x)<nBufferWidth) &&
		//within the height
		(y >= 0) &&
		((m_D->CreateParms.Height + y)<nBufferHeight)
		)
	{
		RECT rcSrc, rcDest;

		rcDest.left = x;
		rcDest.right = x + m_D->CreateParms.Width;
		rcDest.top = y;
		rcDest.bottom = y + m_D->CreateParms.Height;

		rcSrc.top = rcSrc.left = 0;
		rcSrc.bottom = m_D->CreateParms.Height;
		rcSrc.right = m_D->CreateParms.Width;

		Bb->Blt(&rcDest, m_D->Surface, &rcSrc, DDBLT_KEYSRC | DDBLT_WAIT, NULL);

		return ;
	}

	//if the image is off screen we do no blt
	if (x>(int)nBufferWidth)return;
	if (y>(int)nBufferHeight)return;
	if ((x + m_D->CreateParms.Width) <= 0)return;
	if ((y + m_D->CreateParms.Height) <= 0)return;

	//if it has been determined that we need to do a clipped blt lets prepare the rectangles

	//prepare destination rectangle
	if (x >= 0)
		rcDest.left = x;
	else
		rcDest.left = 0;

	if ((x + m_D->CreateParms.Width) < nBufferWidth)
		rcDest.right = x + m_D->CreateParms.Width;
	else
		rcDest.right = nBufferWidth;

	if (y >= 0)
		rcDest.top = y;
	else
		rcDest.top = 0;

	if ((y + m_D->CreateParms.Height) < nBufferHeight)
		rcDest.bottom = y + m_D->CreateParms.Height;
	else
		rcDest.bottom = nBufferHeight;


	//prepare src rectangle
	if (x >= 0)
		rcSrc.left = 0;
	else
		rcSrc.left = 0 - x;

	if ((x + m_D->CreateParms.Width) < nBufferWidth)
		rcSrc.right = m_D->CreateParms.Width;
	else
		rcSrc.right = nBufferWidth - x;

	if (y >= 0)
		rcSrc.top = 0;
	else
		rcSrc.top = 0 - y;

	if ((y + m_D->CreateParms.Height) < nBufferHeight)
		rcSrc.bottom = m_D->CreateParms.Height;
	else
		rcSrc.bottom = nBufferHeight - y;


	if (SUCCEEDED(Bb->Blt(&rcDest, m_D->Surface, &rcSrc, DDBLT_KEYSRC | DDBLT_WAIT, NULL))){
		return;
	}
	return;
}
#endif __DXVERSION__ == 7