#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/SpriteDefinition.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Math/VertexUtils.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Core/StringUtils.hpp"

BitmapFont::BitmapFont(char const* fontFilePathNameWithNoExtension, Texture& fontTexture)
	: m_fontGlyphsSpriteSheet(SpriteSheet(fontTexture, IntVec2(16, 16))),
	  m_fontFilePathNameWithNoExtension(fontFilePathNameWithNoExtension)
{
	
}

Texture& BitmapFont::GetTexture()
{
	// TODO: insert return statement here
	return m_fontGlyphsSpriteSheet.GetTexture();
}

void BitmapFont::AddVertsForText2D(std::vector<Vertex_PCU>& vertexArray, Vec2 const& textMins, float cellHeight, std::string const& text, Rgba8 const& tint, float cellAspect)
{
	float xCoord = textMins.x;
	float yCoord = textMins.y;
	float xOffset = cellHeight * cellAspect;
	float yOffset = cellHeight;

	for (int letterIndex = 0; letterIndex < text.length(); letterIndex++)
	{
		int letterCode = text[letterIndex];
		AddVertsForAABB2D(vertexArray, xCoord, yCoord, xCoord + xOffset, yCoord + yOffset, m_fontGlyphsSpriteSheet.GetSpriteUVs(letterCode), tint);
		xCoord += xOffset;
	}
}

void BitmapFont::AddVertsForTextInBox2D(std::vector<Vertex_PCU>& vertexArray, std::string const& text, AABB2 const& box, float cellHeight, Rgba8 const& tint, float cellAspectScale, Vec2 const& alignment, TextBoxMode mode, int maxGlyphsToDraw)
{
	Vec2 minPoint = box.m_mins;
	Vec2 maxPoint = box.m_maxs;
	float textWidth = cellHeight * cellAspectScale;
	float textHeight = cellHeight;
	float boxWidth = box.GetDimensions().x;
	float boxHeight = box.GetDimensions().y;

	Strings lines = SplitStringOnDelimiter(text, '\n');

	int numLines = static_cast<int>(lines.size());
	float paraHeight = numLines * textHeight;
	float heightOffset = boxHeight - paraHeight;

	int glyphsRemaining = maxGlyphsToDraw;
	int longestIndex = 0;
	for (int i = 0; i < numLines; i++)
	{
		if (lines[i].size() > lines[longestIndex].size())
		{
			longestIndex = i;
		}
	}

	float lineWidth = lines[longestIndex].size() * textWidth;
	float widthOffset = boxWidth - lineWidth;
	float widthAdjust = 1.0f;

	if (mode == SHRINK_TO_FIT)
	{
		if (heightOffset < 0 || widthOffset < 0)
		{
			if (heightOffset < widthOffset)
			{
				textHeight = textHeight * (boxHeight / paraHeight);
				widthOffset = boxWidth - textWidth * (boxHeight / paraHeight);
				heightOffset = boxHeight - textHeight * numLines;
			}
			else
			{
				textHeight = textHeight * (boxWidth / lineWidth);
				heightOffset = boxHeight - textHeight * numLines;
				widthAdjust = (boxWidth / lineWidth);
			}
		}
	}

	for (int i = 0; i < numLines && glyphsRemaining > 0; i++)
	{
		widthOffset = boxWidth - lines[i].size() * textWidth * widthAdjust;
		float yCoord = minPoint.y + (numLines - 1 - i) * textHeight + alignment.y * heightOffset;
		float xCoord = minPoint.x + alignment.x * widthOffset;

		if (glyphsRemaining - static_cast<int>(lines[i].size()) < 0)
		{
			AddVertsForText2D(vertexArray, Vec2(xCoord, yCoord), textHeight, lines[i].substr(0,glyphsRemaining), tint, cellAspectScale);
			glyphsRemaining = 0;
		}
		else
		{
			AddVertsForText2D(vertexArray, Vec2(xCoord, yCoord), textHeight, lines[i], tint, cellAspectScale);
		}
		glyphsRemaining = glyphsRemaining - static_cast<int>(lines[i].size());
	}
}

float BitmapFont::GetTextWidth(float cellHeight, std::string const& text, float cellAspect)
{
	float width = cellHeight * cellAspect * text.length();
	return width;
}

void BitmapFont::AddVertsForText3DAtOriginXForward(std::vector<Vertex_PCU>& verts, float cellHeight, std::string const& text, Rgba8 const& tint, float cellAspect, Vec2 const& alignment, int maxGlyphsToDraw)
{
	alignment;
	maxGlyphsToDraw;
	AddVertsForText2D(verts, Vec2(), cellHeight, text, tint, cellAspect);
	//We know the added verts are the number of characters * 6
	int endIndex = (int)verts.size() - 1;
	for (int charIndex = 0; charIndex < int(text.size()); charIndex++)
	{
		for (int i = 0; i < 6; i++)
		{
			Vertex_PCU original = verts[endIndex - (i + charIndex*6)];
			verts[endIndex - (i + charIndex*6)] = Vertex_PCU(Vec3(0.f, original.m_position.x, original.m_position.y), original.m_color, original.m_uvTexCoords);
		}
	}
}

float BitmapFont::GetGlyphAspect(int glyphUnicode) const
{
	//Todo: Add proper compatibility with real aspect ratios later
	glyphUnicode;
	return 1.0f;
}
