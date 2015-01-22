#if __DXVERSION__ == 11
#include "RendererImage.h"
#include "RendererD3D11_Private.h"
#include <d3d11.h>
#include "img_lib/img_lib.h"

static const DWORD RENDER_IMAGE_TRANSPARENT_COLOR = 0xFFFF00FF;

struct SgRendererImage::sgData
{
	sgRendererImageCreateParms CreateParms;
	sgRendererData             RendererData;
	ID3D11ShaderResourceView*  ShaderView;
	ID3D11Texture2D*           Texture;
};

SgRendererImage::SgRendererImage( const sgRendererImageCreateParms* CreateParms , const sgRendererData* RendererData )
{
	m_D = new sgData;
	m_D->CreateParms  = *CreateParms;
	m_D->RendererData = *RendererData;
	m_D->Texture = NULL;
	m_D->ShaderView = NULL;
	
	Restore();
}

SgRendererImage::~SgRendererImage()
{

	SAFE_RELEASE( m_D->ShaderView );
	SAFE_RELEASE( m_D->Texture );

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

	if (DestWidth == IMAGE_CREATE_DEFAULT)
	{
		DestWidth = m_D->CreateParms.Width;
	}

	if (DestHeight == IMAGE_CREATE_DEFAULT)
	{
		DestHeight = m_D->CreateParms.Height;
	}

	sg_uint32* Colors = new sg_uint32[ DestWidth*DestHeight ];
	
	IMG_DEST_RECT DestRect;
	DestRect.nFormat = IMGFMT_A8B8R8G8;
	DestRect.nWidth = DestWidth;
	DestRect.nHeight = DestHeight;
	DestRect.nPitch = DestWidth*sizeof(sg_uint32);
	DestRect.nOrient = IMGORIENT_TOPLEFT;
	DestRect.pImage = Colors;

	DestRect.rcCopy.top = 0;
	DestRect.rcCopy.left = 0;
	DestRect.rcCopy.bottom = DestHeight;
	DestRect.rcCopy.right = DestWidth;

	IMG_RECT SrcRect;
	SrcRect.top = SrcY;
	SrcRect.bottom = SrcHeight + SrcY;
	SrcRect.left = SrcX;
	SrcRect.right = SrcWidth + SrcX;

	img_bool Copied = IMG_CopyBits(Img, &DestRect, IMGFILTER_NONE, &SrcRect, 0xFF);
	//Add alpha chanel.
	for( int i=0; i<DestWidth*DestHeight; i++ )
	{
		if( Colors[i] == 0xFFFF00FF )
		{
			Colors[i] = 0;//&= 0x00FFFFFF;
		}
	}

	if( Copied )
	{
		D3D11_TEXTURE2D_DESC Desc;
		memset( &Desc , 0 , sizeof(Desc) );
		Desc.Width = m_D->CreateParms.Width;
		Desc.Height = m_D->CreateParms.Height;
		Desc.MipLevels = 1;
		Desc.ArraySize = 1;
		Desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		Desc.SampleDesc.Count = 1;
		Desc.SampleDesc.Quality = 0;
		Desc.Usage = D3D11_USAGE_DEFAULT;
		Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		Desc.CPUAccessFlags = 0;
		Desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA ResData;
		ResData.pSysMem = Colors;
		ResData.SysMemPitch = DestWidth*sizeof(sg_uint32);
		ResData.SysMemSlicePitch = 0;

		HRESULT Res;
		Res = m_D->RendererData.Dev11->CreateTexture2D( &Desc , &ResData , &m_D->Texture );
		assert( SUCCEEDED(Res) );

		D3D11_SHADER_RESOURCE_VIEW_DESC ResDesc;
		ResDesc.Format = Desc.Format;
		ResDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		ResDesc.Texture2D.MostDetailedMip = 0;
		ResDesc.Texture2D.MipLevels = -1;
		Res = m_D->RendererData.Dev11->CreateShaderResourceView( m_D->Texture , &ResDesc , &m_D->ShaderView );
		assert( SUCCEEDED(Res) );
	}

	delete [] Colors;
	IMG_Delete( Img );
	Img = 0;
}

void SgRendererImage::CreateColor()
{
	assert( false ); //Color not supported!
}

void SgRendererImage::Restore()
{
	SAFE_RELEASE( m_D->ShaderView );
	SAFE_RELEASE( m_D->Texture );

	switch (m_D->CreateParms.Type)
	{
	case RENDERER_IMAGE_BITMAP: CreateBitmap(); break;
	case RENDERER_IMAGE_COLOR : CreateColor(); break;
	}
}

void SgRendererImage::Draw( int x , int y )
{
	m_D->RendererData.Renderer->DrawQuad( m_D->ShaderView , static_cast<float>(x) , static_cast<float>(y) , static_cast<float>(m_D->CreateParms.Width) , static_cast<float>(m_D->CreateParms.Height) );
}

#endif __DXVERSION__ == 11