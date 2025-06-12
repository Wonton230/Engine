#pragma once
#include "Engine/Math/Vec2.hpp"

struct Vec2;

struct OBB2
{
public:
	Vec2 m_center;
	Vec2 m_halfDimensions;
	Vec2 m_iBasisNormal;

	OBB2();
	~OBB2();
	explicit OBB2(Vec2 center, Vec2 halfDims, Vec2 iBasisNorm);

	void GetCornerPoints(Vec2* outFourCornersWorldPos) const;
	Vec2 const GetLocalPosFromWorldPos(Vec2 const& worldPos) const;
	Vec2 const GetWorldPosFromLocalPos(Vec2 const& localPos) const;
	void RotateAroundCenter(float rotationDeltaDeg);
};