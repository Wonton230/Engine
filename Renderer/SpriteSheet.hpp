#pragma once
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/SpriteDefinition.hpp"
#include <vector>

class Texture;
class SpriteDefinition;
struct IntVec2;
struct Vec2;
struct AABB2;

class SpriteSheet
{
public:
	explicit SpriteSheet(Texture& texture, IntVec2 const& simpleGridLayout);

	Texture&				GetTexture() const;
	int						GetNumSprites() const;
	SpriteDefinition const& GetSpriteDef(int spriteIndex) const;
	void					GetSpriteUVs(Vec2& out_uvAtMins, Vec2& out_uvAtMaxs, int spriteIndex) const;
	void					GetSpriteUVs(Vec2& out_uvAtMins, Vec2& out_uvAtMaxs, IntVec2 spriteCoords) const;
	AABB2					GetSpriteUVs(int spriteIndex) const;
	AABB2					GetSpriteUVs(IntVec2 spriteCoords) const;

protected:
	Texture&						m_texture;
	const IntVec2					m_simpleCoords;
	std::vector<SpriteDefinition>	m_spriteDefs;
};