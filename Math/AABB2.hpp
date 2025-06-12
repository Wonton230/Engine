#pragma once
#include "Engine/Math/Vec2.hpp"

struct AABB2
{
public:
	//member vars
	Vec2 m_mins;
	Vec2 m_maxs;

	//functions
	AABB2() {};
	~AABB2() {};
	AABB2(AABB2 const& copyFrom);
	explicit	AABB2(float minX, float minY, float maxX, float maxY);
	explicit	AABB2(Vec2 const& mins, Vec2 const& maxs);

	bool		IsPointInside(Vec2 const& point) const;
	Vec2 const	GetCenter() const;
	Vec2 const	GetDimensions() const;
	Vec2 const	GetNearestPoint(Vec2 const& reference) const;
	Vec2 const	GetPointAtUV(Vec2 const& UV) const;
	Vec2 const	GetUVForPoint(Vec2 const& point) const;

	void		Translate(Vec2 const& translation);
	void		SetCenter(Vec2 const& newCenter);
	void		SetDimensions(Vec2 const& newDimensions);
	void		StretchToIncludePoint(Vec2 const& point);
	void		AddPadding(float padding);
	void		HorizontalCut(float topPercentage, AABB2& outBottom);
	void		VerticalCut(float leftPercentage, AABB2& outRight);
	AABB2		MakeBoxCenteredInBox(Vec2 dimensions);

	//Constants
	static const AABB2 ZERO_TO_ONE;
};