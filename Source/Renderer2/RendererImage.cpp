#include "RendererImage.h"

struct SgRendererImage::sgData
{
	int Width;
	int Height;
};

SgRendererImage::SgRendererImage()
{
	m_D = new sgData;
}

SgRendererImage::~SgRendererImage()
{
	delete m_D;
}