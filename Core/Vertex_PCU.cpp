#include "Vertex_PCU.hpp"

Vertex_PCU::Vertex_PCU(Vec3 pos, Rgba8 col, Vec2 uv)
{
	m_position = pos;
	m_color = col;
	m_uvTexCoords = uv;
}

Vertex_PCU::Vertex_PCU(Vec2 position, Rgba8 color, Vec2 uvTexCoords)
{
	Vec3 temp = Vec3(position.x, position.y, 0);
	m_position = temp;
	m_color = color;
	m_uvTexCoords = uvTexCoords;
}
