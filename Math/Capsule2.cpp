#include "Capsule2.hpp"

Capsule2::Capsule2()
{
	m_bone = LineSegment2();
	m_radius = 1;
}

Capsule2::~Capsule2()
{
}

Capsule2::Capsule2(LineSegment2 line, float radius)
{
	m_bone = line;
	m_radius = radius;
}

void Capsule2::Translate(Vec2 translation)
{
	m_bone.Translate(translation);
}

void Capsule2::SetCenter(Vec2 center)
{
	m_bone.SetCenter(center);
}

void Capsule2::RotateAroundCenterDeg(float rotationDeltaDeg)
{
	m_bone.RotateAroundCenterDeg(rotationDeltaDeg);
}

//Find the capsule start theta and end theta of each cap based on the forawrd angle of the start to end line segment