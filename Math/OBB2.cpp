#include "OBB2.hpp"
#include "Engine/Math/MathUtils.hpp"

OBB2::OBB2()
{
	m_center = Vec2(0, 0);
	m_halfDimensions = Vec2(1, 1);
	m_iBasisNormal = Vec2(1, 0);
}

OBB2::~OBB2()
{
}

OBB2::OBB2(Vec2 center, Vec2 halfDims, Vec2 iBasisNorm)
{
	m_center = center;
	m_halfDimensions = halfDims;
	m_iBasisNormal = iBasisNorm;
}

void OBB2::GetCornerPoints(Vec2* outFourCornersWorldPos) const
{
	outFourCornersWorldPos[0] = m_center - m_iBasisNormal * m_halfDimensions.x - m_iBasisNormal.GetRotated90Degrees() * m_halfDimensions.y;
	outFourCornersWorldPos[1] = m_center + m_iBasisNormal * m_halfDimensions.x - m_iBasisNormal.GetRotated90Degrees() * m_halfDimensions.y;
	outFourCornersWorldPos[2] = m_center + m_iBasisNormal * m_halfDimensions.x + m_iBasisNormal.GetRotated90Degrees() * m_halfDimensions.y;
	outFourCornersWorldPos[3] = m_center - m_iBasisNormal * m_halfDimensions.x + m_iBasisNormal.GetRotated90Degrees() * m_halfDimensions.y;
}

Vec2 const OBB2::GetLocalPosFromWorldPos(Vec2 const& worldPos) const
{
	float iCoord = DotProduct2D(worldPos, m_iBasisNormal);
	float jCoord = DotProduct2D(worldPos, m_iBasisNormal.GetRotated90Degrees());
	return Vec2(iCoord, jCoord);
}

Vec2 const OBB2::GetWorldPosFromLocalPos(Vec2 const& localPos) const
{
	Vec2 worldCoord = m_center + m_iBasisNormal * localPos.x + m_iBasisNormal.GetRotated90Degrees() * localPos.y;
	return worldCoord;
}

void OBB2::RotateAroundCenter(float rotationDeltaDeg)
{
	rotationDeltaDeg;
}
