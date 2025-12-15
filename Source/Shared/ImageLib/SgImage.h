// (c) 2025 Beem Media. All rights reserved.

#pragma once

class SgImage
{
public:
	struct sgPixel
	{
		std::uint8_t A = 0;
		std::uint8_t R = 0;
		std::uint8_t G = 0;
		std::uint8_t B = 0;
	};
	static_assert(sizeof(sgPixel) == 4, "sgPixel should be 32 bits.");

public:
	SgImage() = default;
	SgImage(const sg_char16* Filename, std::size_t Offset = 0, std::size_t FileSize = 0) { LoadSgImage(Filename, Offset, FileSize); }
	SgImage(sg_cpstr Filename, std::size_t Offset = 0, std::size_t FileSize = 0) { LoadSgImage(Filename, Offset, FileSize); }
	SgImage(std::ifstream& File, std::size_t FileSize) { LoadSgImage(File, FileSize); }

	void LoadSgImage(const sg_char16* Filename, std::size_t Offset = 0, std::size_t FileSize = 0);
	void LoadSgImage(sg_cpstr Filename, std::size_t Offset = 0, std::size_t FileSize = 0);
	void LoadSgImage(std::istream& File, std::size_t FileSize);

	void ClearSgImage();

	bool IsLoaded() const { return m_Width > 0 && m_Height > 0; }

	std::uint32_t GetWidth() const { return m_Width; }
	std::uint32_t GetHeight() const { return m_Height; }
	const sgPixel* GetPixels() const { return m_Pixels.data(); }

private:
	std::uint32_t m_Width = 0;
	std::uint32_t m_Height = 0;
	std::vector<sgPixel> m_Pixels;
};
