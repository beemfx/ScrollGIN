////////////////////////////////////////////////////////////////////////////
//
// SgSprite - The main drawing object for the Renderer.
//
// (c) 2015 Blaine Myers
//
////////////////////////////////////////////////////////////////////////////
#pragma once

enum RENDERER_IMAGE_T
{
	RENDERER_IMAGE_UNK,
	RENDERER_IMAGE_BITMAP,
};

static const int IMAGE_CREATE_DEFAULT = -1;

struct sgRendererImageCreateParms
{
	RENDERER_IMAGE_T Type;
	int              Width;
	int              Height;
	int              BmX;
	int              BmY;
	int              BmWidth;
	int              BmHeight;
	char             BmFile[256];
	int              BmFileOffset;
};

struct sgRendererData
{
	struct IDirectDraw7*        Dd;
	struct IDirectDrawSurface7* BackSurface;
	struct ID3D11Device*        Dev11;
	struct ID3D11DeviceContext* DevContext11;
	class IRendererD3D11*       Renderer;
};

class SgRendererImage
{
public:
	SgRendererImage( const sgRendererImageCreateParms* CreateParms , const sgRendererData* RendererData );
	~SgRendererImage();

	void Draw( int x , int y );
	void Restore();
private:
	void CreateBitmap();
private:
	struct sgData;
	sgData* m_D;
};