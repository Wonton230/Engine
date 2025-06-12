#include "Engine/Math/Vec3.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Math/Vec2.hpp"
#pragma once

struct Vertex_PCU
{
public:
	Vec3 m_position;
	Rgba8 m_color;
	Vec2 m_uvTexCoords;
public:
	// Construction/Destruction
	~Vertex_PCU() {}												// destructor (do nothing)
	Vertex_PCU() {}												// default constructor (do nothing)	
	explicit Vertex_PCU(Vec3 position, Rgba8 color, Vec2 uvTexCoords);
	explicit Vertex_PCU(Vec2 position, Rgba8 color, Vec2 uvTexCoords);
};