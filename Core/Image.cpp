#include "Image.hpp"
#include "Engine/Core/Rgba8.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "ThirdParty/stb/stb_image.h"

Image::Image()
{
	m_imageFilePath = "";
	m_dimensions = IntVec2(0, 0);
}

Image::~Image()
{

}

Image::Image(const char* imageFilePath)
{
	m_imageFilePath = imageFilePath;

	int width;
	int height;
	int channels;
	stbi_set_flip_vertically_on_load(1);
	unsigned char* rawData = stbi_load(imageFilePath, &width, &height, &channels, 4);

	m_dimensions = IntVec2(width, height);
	m_texelRgba8Data.resize(width * height);

	for (int pixelIndex = 0; pixelIndex < static_cast<int>(m_texelRgba8Data.size()); pixelIndex ++)
	{
		//Might have to make the channels adaptable
		m_texelRgba8Data[pixelIndex] = Rgba8(
			rawData[(pixelIndex * 4) + 0],
			rawData[(pixelIndex * 4) + 1],
			rawData[(pixelIndex * 4) + 2],
			rawData[(pixelIndex * 4) + 3]
		);
	}

	stbi_image_free(rawData);
}

Image::Image(IntVec2 size, Rgba8 color)
{
	m_dimensions = size;
	m_texelRgba8Data.resize(size.x * size.y);

	for (int pixelIndex = 0; pixelIndex < static_cast<int>(m_texelRgba8Data.size()); pixelIndex++)
	{
		m_texelRgba8Data[pixelIndex] = color;
	}
}

IntVec2 Image::GetDimensions() const
{
	return m_dimensions;
}

const std::string& Image::GetImageFilePath() const
{
	return m_imageFilePath;
}

const void* Image::GetRawData() const
{
	return m_texelRgba8Data.data();
}

std::vector<Rgba8> Image::GetDataAsRgba8Vector() const
{
	std::vector<Rgba8> texels;

	//if (m_texelRgba8Data.size() % 4 != 0) {
	//	return texels;
	//}

	size_t numTexels = m_texelRgba8Data.size();
	texels.resize(numTexels);

	std::memcpy(texels.data(), m_texelRgba8Data.data(), 4*m_texelRgba8Data.size());

	return texels;
}
