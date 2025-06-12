#pragma once
#include "Engine/Math/Vec3.hpp"

struct Plane3
{
public:
	Plane3();
	Plane3(Plane3 const& copyFrom);
	Plane3(Vec3 const& normal, float distance);
	~Plane3();

	bool	IsPointInFrontOfPlane(Vec3 const& referencePoint) const;
	Vec3	GetNearestPoint(Vec3 const& referencePoint) const;
	Vec3	GetOriginPoint() const;

	Vec3	m_normal;
	float	m_distanceFromOrigin;
};