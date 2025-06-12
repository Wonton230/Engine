#include "TileHeatMap.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Math/VertexUtils.hpp"
#include "Engine/Math/MathUtils.hpp"

TileHeatMap::TileHeatMap()
{

}

TileHeatMap::TileHeatMap(IntVec2 const& dimensions)
{
	int arraySize = dimensions.x * dimensions.y;
	m_dimensions = dimensions;
	for (int i = 0; i < arraySize; i++)
	{
		m_values.push_back(0.f);
	}
}

void TileHeatMap::SetAllValues(float setTo)
{
	if (m_values.size() <= 0.f)
	{
		return;
	}
	else
	{
		for (int i = 0; i < m_values.size(); i++)
		{
			m_values[i] = setTo;
		}
	}
}

void TileHeatMap::SetTileHeatValue(int tileIndex, float heatValue)
{
	m_values[tileIndex] = heatValue;
}

float TileHeatMap::GetTileHeatValue(int tileIndex) const
{
	return m_values[tileIndex];
}

int TileHeatMap::GetSize() const
{
	return m_dimensions.x * m_dimensions.y;
}

float TileHeatMap::GetHighestNonSolidValue() const
{
	float max = 0.f;
	for (int i = 0; i < m_values.size(); i++)
	{
		if (m_values[i] > max && m_values[i] < m_dimensions.x * m_dimensions.y)
		{
			max = m_values[i];
		}
	}
	return max;
}

void TileHeatMap::AddVertsForDebugDraw(std::vector<Vertex_PCU>& vertexArray, AABB2 totalBounds, FloatRange valueRange, Rgba8 lowColor, Rgba8 highColor, float specialValue, Rgba8 specialColor) const
{
	float tileWidth = (totalBounds.m_maxs.x - totalBounds.m_mins.x)/m_dimensions.x;
	float tileHeight = (totalBounds.m_maxs.y - totalBounds.m_mins.y)/m_dimensions.y;

	for (int y = 0; y < m_dimensions.y; y++)
	{
		for (int x = 0; x < m_dimensions.x; x++)
		{
			float heat = m_values[y * m_dimensions.x + x];
			if (heat == specialValue)
			{
				AddVertsForAABB2D(vertexArray, x * tileWidth, y * tileHeight, (x + 1) * tileWidth, (y + 1) * tileHeight, specialColor);
			}
			else
			{
				//calculate color
				float fraction = GetClamped(GetFractionWithinRange(heat, valueRange.m_min, valueRange.m_max), 0.f, 1.f);

				Rgba8 lerpColor = InterpolateColor(lowColor,highColor,fraction);
				AddVertsForAABB2D(vertexArray, x * tileWidth, y * tileHeight, (x + 1) * tileWidth, (y + 1) * tileHeight, lerpColor);
			}	
		}
	}
}
