#include "SpriteSheet.hpp"
#include "Engine/Renderer/SpriteDefinition.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Math/AABB2.hpp"

SpriteSheet::SpriteSheet(Texture& texture, IntVec2 const& simpleGridLayout)
	: m_texture(texture),
	  m_simpleCoords(simpleGridLayout)
{

	float uStep = 1.f / static_cast<float>(simpleGridLayout.x);
	float vStep = 1.f / static_cast<float>(simpleGridLayout.y);

	float correctionU = 1.f / (128.f * texture.GetDimensions().x);
	float correctionV = 1.f / (128.f * texture.GetDimensions().y);

	for (int v = simpleGridLayout.y; v > 0; v--)
	{
		for (int u = 0; u < simpleGridLayout.x; u++)
		{
			Vec2 min = Vec2(uStep * u + correctionU, vStep * (v-1) + correctionV);
			Vec2 max = Vec2(uStep * (u + 1) - correctionU, vStep * v - correctionV);
			int index = -(v - simpleGridLayout.y);
			m_spriteDefs.push_back(SpriteDefinition(*this, index * simpleGridLayout.x + u, min, max));
		}
	}
}

Texture& SpriteSheet::GetTexture() const
{
	return m_texture;
}

int SpriteSheet::GetNumSprites() const
{
	return static_cast<int>(m_spriteDefs.size());
}

SpriteDefinition const& SpriteSheet::GetSpriteDef(int spriteIndex) const
{
	if (spriteIndex > -1 && spriteIndex < m_spriteDefs.size())
	{
		return m_spriteDefs[spriteIndex];
	}
	else
	{
		ERROR_AND_DIE("illegal sprite index");
	}
}

void SpriteSheet::GetSpriteUVs(Vec2& out_uvAtMins, Vec2& out_uvAtMaxs, int spriteIndex) const
{
	m_spriteDefs[spriteIndex].GetUVs(out_uvAtMins, out_uvAtMaxs);
}

void SpriteSheet::GetSpriteUVs(Vec2& out_uvAtMins, Vec2& out_uvAtMaxs, IntVec2 spriteCoords) const
{
	int index = spriteCoords.x + (spriteCoords.y * m_simpleCoords.x);
	GetSpriteUVs(out_uvAtMins, out_uvAtMaxs, index);
}

AABB2 SpriteSheet::GetSpriteUVs(int spriteIndex) const
{
	return m_spriteDefs[spriteIndex].GetUVs();
}

AABB2 SpriteSheet::GetSpriteUVs(IntVec2 spriteCoords) const
{
	int index = spriteCoords.x + (spriteCoords.y * m_simpleCoords.x);
	return GetSpriteUVs(index);
}
