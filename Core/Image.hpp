#pragma once
#include <string>
#include <vector>
#include "Engine/Math/IntVec2.hpp"

struct Rgba8;

class Image
{
	friend class Renderer;

public:
	Image();
	~Image();
	Image(const char* imageFilePath);
	Image(IntVec2 size, Rgba8 color);

	IntVec2 GetDimensions() const;
	const std::string& GetImageFilePath() const;
	const void* GetRawData() const;
	std::vector<Rgba8> GetDataAsRgba8Vector() const;

private:
	std::string m_imageFilePath;
	IntVec2 m_dimensions;
	std::vector<Rgba8> m_texelRgba8Data;
};