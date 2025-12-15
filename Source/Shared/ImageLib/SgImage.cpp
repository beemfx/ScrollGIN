// (c) 2025 Beem Media. All rights reserved.


#include "SgImage.h"

#include "img_lib/img_lib.h"

void SgImage::LoadSgImage(const sg_char16* Filename, std::size_t Offset /*= 0*/, std::size_t FileSize /*= 0*/)
{
	ClearSgImage();
	
	std::ifstream File(Filename, std::ios::binary);

	if (!File.is_open())
	{
		return;
	}

	if (Offset == 0 && FileSize == 0)
	{
		File.seekg(0, std::ios::end);
		FileSize = static_cast<std::size_t>(File.tellg());
		File.seekg(0, std::ios::beg);
	}

	File.seekg(Offset, std::ios::cur);
	LoadSgImage(File, FileSize);
}

void SgImage::LoadSgImage(sg_cpstr Filename, std::size_t Offset /*= 0*/, std::size_t FileSize /*= 0*/)
{
	ClearSgImage();

	std::ifstream File(Filename, std::ios::binary);

	if (!File.is_open())
	{
		return;
	}

	if (Offset == 0 && FileSize == 0)
	{
		File.seekg(0, std::ios::end);
		FileSize = static_cast<std::size_t>(File.tellg());
		File.seekg(0, std::ios::beg);
	}

	File.seekg(Offset, std::ios::cur);
	LoadSgImage(File, FileSize);
}

void SgImage::LoadSgImage(std::istream& File, std::size_t FileSize)
{
	ClearSgImage();

	std::vector<std::uint8_t> FileBytes;
	FileBytes.resize(FileSize);
	File.read(reinterpret_cast<char*>(FileBytes.data()), FileSize);
	if (!File)
	{
		return;
	}

	HIMG Img = IMG_OpenMemory(FileBytes.data(), FileBytes.size());
	if (Img)
	{
		IMG_DESC ImgDesc = { };
		IMG_GetDesc(Img, &ImgDesc);
		m_Width = ImgDesc.Width;
		m_Height = ImgDesc.Height;

		if (m_Width > 0 && m_Height > 0)
		{
			m_Pixels.resize(m_Width * m_Height);

			IMG_DEST_RECT DestRc = { };
			DestRc.nFormat = IMGFMT_A8R8G8B8;
			DestRc.nWidth = m_Width;
			DestRc.nHeight = m_Height;
			DestRc.nPitch = m_Width * sizeof(sgPixel);
			DestRc.nOrient = IMGORIENT_TOPLEFT;
			DestRc.pImage = m_Pixels.data();

			DestRc.rcCopy.top = 0;
			DestRc.rcCopy.left = 0;
			DestRc.rcCopy.right = m_Width;
			DestRc.rcCopy.bottom = m_Height;

			IMG_RECT SourceRc = { };
			SourceRc.top = 0;
			SourceRc.left = 0;
			SourceRc.right = m_Width;
			SourceRc.bottom = m_Height;

			const img_bool bCopied = IMG_CopyBits(Img, &DestRc, IMGFILTER_POINT, &SourceRc, 0);
			if (!bCopied)
			{
				ClearSgImage();
			}
		}

		IMG_Delete(Img);
	}
}

void SgImage::ClearSgImage()
{
	m_Width = 0;
	m_Height = 0;
	m_Pixels.resize(0);
}
