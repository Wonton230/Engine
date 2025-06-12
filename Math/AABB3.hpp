#pragma once
#include "Engine/Math/Vec3.hpp"

struct AABB3
{
public:
	//member vars
	Vec3 m_mins;
	Vec3 m_maxs;

	//functions
	AABB3() {};
	~AABB3() {};
	AABB3(AABB3 const& copyFrom);
	explicit	AABB3(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
	explicit	AABB3(Vec3 const& mins, Vec3 const& maxs);

	bool		IsPointInside(Vec3 const& point) const;
	//Vec3 const	GetCenter() const;
	//Vec3 const	GetDimensions() const;
	//Vec3 const	GetNearestPoint(Vec3 const& reference) const;
	//Vec3 const	GetPointAtUV(Vec3 const& UV) const;
	//Vec3 const	GetUVForPoint(Vec3 const& point) const;

	//void		Translate(Vec3 const& translation);
	//void		SetCenter(Vec3 const& newCenter);
	//void		SetDimensions(Vec3 const& newDimensions);
	//void		StretchToIncludePoint(Vec3 const& point);
	//void		SetFromText(char const* text);
};