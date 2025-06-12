#include "Plane3.hpp"
#include "Engine/Math/MathUtils.hpp"

Plane3::Plane3()
{
	m_normal = Vec3(0.f, 0.f, 1.f);
	m_distanceFromOrigin = 0.f;
}

Plane3::Plane3(Plane3 const& copyFrom)
{
	m_normal = copyFrom.m_normal;
	m_distanceFromOrigin = copyFrom.m_distanceFromOrigin;
}

Plane3::Plane3(Vec3 const& normal, float distance)
{
	m_normal = normal;
	m_distanceFromOrigin = distance;
}

Plane3::~Plane3()
{
}

bool Plane3::IsPointInFrontOfPlane(Vec3 const& referencePoint) const
{
	float distanceToRefPlane = DotProduct3D(m_normal, referencePoint);
	return distanceToRefPlane > m_distanceFromOrigin;
}

Vec3 Plane3::GetNearestPoint(Vec3 const& referencePoint) const
{
	float pointAltitude = DotProduct3D(m_normal, referencePoint) - m_distanceFromOrigin;
	return referencePoint - (m_normal * pointAltitude);
}

Vec3 Plane3::GetOriginPoint() const
{
	return m_normal * m_distanceFromOrigin;
}
