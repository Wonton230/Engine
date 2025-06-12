#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/FloatRange.hpp"
#include "Engine/Core/Rgba8.hpp"
#include <vector>
#pragma once

struct Vertex_PCU;
struct AABB2;

class TileHeatMap
{

public:
	TileHeatMap();
	TileHeatMap(IntVec2 const& dimensions);

	void	SetAllValues(float setTo);
	void	SetTileHeatValue(int tileIndex, float heatValue);
	float	GetTileHeatValue(int tileIndex) const;
	int		GetSize() const;
	float	GetHighestNonSolidValue() const;

	void	AddVertsForDebugDraw(std::vector<Vertex_PCU>& vertexArray, AABB2 totalBounds, FloatRange valueRange = FloatRange(0.f, 1.f), Rgba8 lowColor = Rgba8(0, 0, 0, 100), Rgba8 highColor = Rgba8(255, 255, 255, 100), float specialValue = 999999.f, Rgba8 specialColor = Rgba8(255, 0, 255, 255)) const;

private:
	std::vector<float> m_values;
	IntVec2 m_dimensions;
};