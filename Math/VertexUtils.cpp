#include "VertexUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Core/Vertex_PCUTBN.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/LineSegment2.hpp"
#include "Engine/Math/OBB2.hpp"
#include "Engine/Math/OBB3.hpp"
#include "Engine/Math/Capsule2.hpp"
#include "Engine/Math/Triangle2.hpp"
#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Math/Mat44.hpp"

void AddVertsForCapsule2D(std::vector<Vertex_PCU>& verts, Capsule2 const& capsule, Rgba8 const& color)
{
	Vec2 start = capsule.m_bone.m_start;
	Vec2 end = capsule.m_bone.m_end;
	Vec2 perpendicularRadius = (capsule.m_bone.m_end - capsule.m_bone.m_start).GetRotated90Degrees().GetClamped(capsule.m_radius);

	verts.push_back(Vertex_PCU(start + perpendicularRadius, color,Vec2(0,0)));
	verts.push_back(Vertex_PCU(start - perpendicularRadius, color, Vec2(0, 0)));
	verts.push_back(Vertex_PCU(end + perpendicularRadius, color, Vec2(0, 0)));

	verts.push_back(Vertex_PCU(start - perpendicularRadius, color, Vec2(0, 0)));
	verts.push_back(Vertex_PCU(end - perpendicularRadius, color, Vec2(0, 0)));
	verts.push_back(Vertex_PCU(end + perpendicularRadius, color, Vec2(0, 0)));

	int numSidesOnEnd = 30;
	float degreesPerSide = 180.f / numSidesOnEnd;
	Vec2 transferPoint = end - perpendicularRadius;

	for (int i = 0; i < numSidesOnEnd; i++)
	{
		verts.push_back(Vertex_PCU(end, color, Vec2(0, 0)));
		verts.push_back(Vertex_PCU(transferPoint, color, Vec2(0, 0)));

		Vec2 endToTransfer = transferPoint - end;
		endToTransfer.RotateDegrees(degreesPerSide);
		transferPoint = end + endToTransfer;

		verts.push_back(Vertex_PCU(transferPoint, color, Vec2(0, 0)));
	}

	transferPoint = start + perpendicularRadius;

	for (int i = 0; i < numSidesOnEnd; i++)
	{
		verts.push_back(Vertex_PCU(start, color, Vec2(0, 0)));
		verts.push_back(Vertex_PCU(transferPoint, color, Vec2(0, 0)));

		Vec2 startToTransfer = transferPoint - start;
		startToTransfer.RotateDegrees(degreesPerSide);
		transferPoint = start + startToTransfer;

		verts.push_back(Vertex_PCU(transferPoint, color, Vec2(0, 0)));
	}
}

void AddVertsForAABB2D(std::vector<Vertex_PCU>& verts, AABB2 const& aabb, Rgba8 const& color)
{
	Vec2 pointA = aabb.m_mins;
	Vec2 pointB = Vec2(aabb.m_maxs.x, aabb.m_mins.y);
	Vec2 pointC = aabb.m_maxs;
	Vec2 pointD = Vec2(aabb.m_mins.x, aabb.m_maxs.y);

	verts.push_back(Vertex_PCU(pointA, color, Vec2(0, 0)));
	verts.push_back(Vertex_PCU(pointB, color, Vec2(1, 0)));
	verts.push_back(Vertex_PCU(pointC, color, Vec2(1, 1)));

	verts.push_back(Vertex_PCU(pointC, color, Vec2(1, 1)));
	verts.push_back(Vertex_PCU(pointD, color, Vec2(0, 1)));
	verts.push_back(Vertex_PCU(pointA, color, Vec2(0, 0)));
}

void AddVertsForAABB2D(std::vector<Vertex_PCU>& verts, AABB2 const& aabb, AABB2 const& UVs, Rgba8 const& color)
{
	Vec2 pointA = aabb.m_mins;
	Vec2 pointB = Vec2(aabb.m_maxs.x, aabb.m_mins.y);
	Vec2 pointC = aabb.m_maxs;
	Vec2 pointD = Vec2(aabb.m_mins.x, aabb.m_maxs.y);

	verts.push_back(Vertex_PCU(pointA, color, UVs.m_mins));
	verts.push_back(Vertex_PCU(pointB, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y)));
	verts.push_back(Vertex_PCU(pointC, color, UVs.m_maxs));

	verts.push_back(Vertex_PCU(pointC, color, UVs.m_maxs));
	verts.push_back(Vertex_PCU(pointD, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y)));
	verts.push_back(Vertex_PCU(pointA, color, UVs.m_mins));
}

void AddVertsForAABB2D(std::vector<Vertex_PCU>& verts, float minX, float minY, float maxX, float maxY, Rgba8 const& color)
{
	Vec2 pointA = Vec2(static_cast<float>(minX), static_cast<float>(minY));
	Vec2 pointB = Vec2(static_cast<float>(maxX), static_cast<float>(minY));
	Vec2 pointC = Vec2(static_cast<float>(maxX), static_cast<float>(maxY));
	Vec2 pointD = Vec2(static_cast<float>(minX), static_cast<float>(maxY));

	verts.push_back(Vertex_PCU(pointA, color, Vec2(0, 0)));
	verts.push_back(Vertex_PCU(pointB, color, Vec2(1, 0)));
	verts.push_back(Vertex_PCU(pointC, color, Vec2(1, 1)));

	verts.push_back(Vertex_PCU(pointC, color, Vec2(1, 1)));
	verts.push_back(Vertex_PCU(pointD, color, Vec2(0, 1)));
	verts.push_back(Vertex_PCU(pointA, color, Vec2(0, 0)));
}

void AddVertsForAABB2D(std::vector<Vertex_PCU>& verts, float minX, float minY, float maxX, float maxY, AABB2 UVs, Rgba8 const& color)
{
	Vec2 pointA = Vec2(static_cast<float>(minX), static_cast<float>(minY));
	Vec2 pointB = Vec2(static_cast<float>(maxX), static_cast<float>(minY));
	Vec2 pointC = Vec2(static_cast<float>(maxX), static_cast<float>(maxY));
	Vec2 pointD = Vec2(static_cast<float>(minX), static_cast<float>(maxY));

	float minU = UVs.m_mins.x;
	float maxU = UVs.m_maxs.x;
	float minV = UVs.m_mins.y;
	float maxV = UVs.m_maxs.y;

	verts.push_back(Vertex_PCU(pointA, color, Vec2(minU, minV)));
	verts.push_back(Vertex_PCU(pointB, color, Vec2(maxU, minV)));
	verts.push_back(Vertex_PCU(pointC, color, Vec2(maxU, maxV)));

	verts.push_back(Vertex_PCU(pointC, color, Vec2(maxU, maxV)));
	verts.push_back(Vertex_PCU(pointD, color, Vec2(minU, maxV)));
	verts.push_back(Vertex_PCU(pointA, color, Vec2(minU, minV)));
}

void AddVertsForOBB2D(std::vector<Vertex_PCU>& verts, OBB2 const& obb, Rgba8 const& color)
{
	Vec2 points[4];
	obb.GetCornerPoints(points);

	verts.push_back(Vertex_PCU(points[0], color, Vec2(0, 0)));
	verts.push_back(Vertex_PCU(points[1], color, Vec2(1, 0)));
	verts.push_back(Vertex_PCU(points[2], color, Vec2(1, 1)));

	verts.push_back(Vertex_PCU(points[0], color, Vec2(0, 0)));
	verts.push_back(Vertex_PCU(points[2], color, Vec2(1, 1)));
	verts.push_back(Vertex_PCU(points[3], color, Vec2(0, 1)));
}

void AddVertsForDisc2D(std::vector<Vertex_PCU>& verts, Vec2 const& center, float radius, Rgba8 const& color)
{
	int numSides = 60;
	float degreesPerSide = 360.f / numSides;
	Vec2 transferPoint = center + Vec2(radius, 0);

	for (int i = 0; i < numSides; i++)
	{
		verts.push_back(Vertex_PCU(center, color, Vec2(0, 0)));
		verts.push_back(Vertex_PCU(transferPoint, color, Vec2(0, 0)));

		Vec2 centerToTransfer = transferPoint - center;
		centerToTransfer.RotateDegrees(degreesPerSide);
		transferPoint = center + centerToTransfer;

		verts.push_back(Vertex_PCU(transferPoint, color, Vec2(0, 0)));
	}
}

void AddVertsForRing2D(std::vector<Vertex_PCU>& verts, Vec2 const& center, float radius, float thickness, Rgba8 const& color)
{
	int numSides = 60;
	float degreesPerSide = 360.f / numSides;
	Vec2 transferPoint = center + Vec2(radius, 0);

	for (int i = 0; i < numSides; i++)
	{
		Vec2 initialPoint = transferPoint;
		Vec2 centerToTransfer = transferPoint - center;
		centerToTransfer.RotateDegrees(degreesPerSide);
		transferPoint = center + centerToTransfer;

		AddVertsForLineSegment2D(verts, LineSegment2(initialPoint, transferPoint), thickness, color);
	}
}

void AddVertsForLineSegment2D(std::vector<Vertex_PCU>& verts, LineSegment2 line, float thickness, Rgba8 const& color)
{
	Vec2 thicknessOffsetVectorRight = (line.m_end - line.m_start).GetClamped(thickness / 2);
	Vec2 thicknessOffsetVectorUp = thicknessOffsetVectorRight.GetRotated90Degrees();

	Vec2 pointA = line.m_start - thicknessOffsetVectorRight - thicknessOffsetVectorUp;
	Vec2 pointB = line.m_end + thicknessOffsetVectorRight - thicknessOffsetVectorUp;
	Vec2 pointC = line.m_end + thicknessOffsetVectorRight + thicknessOffsetVectorUp;
	Vec2 pointD = line.m_start - thicknessOffsetVectorRight + thicknessOffsetVectorUp;

	verts.push_back(Vertex_PCU(pointA, color, Vec2(0, 0)));
	verts.push_back(Vertex_PCU(pointB, color, Vec2(0, 0)));
	verts.push_back(Vertex_PCU(pointC, color, Vec2(0, 0)));

	verts.push_back(Vertex_PCU(pointC, color, Vec2(0, 0)));
	verts.push_back(Vertex_PCU(pointD, color, Vec2(0, 0)));
	verts.push_back(Vertex_PCU(pointA, color, Vec2(0, 0)));
}

void AddVertsForTriangle2D(std::vector<Vertex_PCU>& verts, Triangle2 triangle, Rgba8 const& color)
{
	verts.push_back(Vertex_PCU(triangle.m_pointsCounterClockwise[0], color, Vec2(0, 0)));
	verts.push_back(Vertex_PCU(triangle.m_pointsCounterClockwise[1], color, Vec2(0, 0)));
	verts.push_back(Vertex_PCU(triangle.m_pointsCounterClockwise[2], color, Vec2(0, 0)));
}

void AddVertsForArrow2D(std::vector<Vertex_PCU>& verts, Vec2 tailPos, Vec2 tipPos, float arrowSize, float lineThickness, Rgba8 color)
{
	AddVertsForLineSegment2D(verts, LineSegment2(tailPos, tipPos), lineThickness, color);
	Vec2 line = tipPos - tailPos;

	Vec2 leftWing = tipPos + Vec2::MakeFromPolarDegrees(line.GetOrientationDegrees() + 135.f, arrowSize);
	Vec2 rightWing = tipPos + Vec2::MakeFromPolarDegrees(line.GetOrientationDegrees() - 135.f, arrowSize);

	AddVertsForLineSegment2D(verts, LineSegment2(tipPos, leftWing), lineThickness, color);
	AddVertsForLineSegment2D(verts, LineSegment2(tipPos, rightWing), lineThickness, color);
}

void AddVertsForCurve2D(std::vector<Vertex_PCU>& verts, std::vector<Vec2> points, float thickness, Rgba8 const& color)
{
	int divisions = (int)points.size();

	for (int i = 0; i < divisions - 1; i++)
	{
		LineSegment2 line = LineSegment2(points[i], points[i+1]);
		AddVertsForLineSegment2D(verts, line, thickness, color);
	}
}

void AddVertsForQuad3D(std::vector<Vertex_PCU>& verts, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Vec3& topLeft, const Rgba8& color, const AABB2& UVs)
{
	Vec3 pointA = bottomLeft;
	Vec3 pointB = bottomRight;
	Vec3 pointC = topRight;
	Vec3 pointD = topLeft;

	verts.push_back(Vertex_PCU(pointA, color, UVs.m_mins));
	verts.push_back(Vertex_PCU(pointB, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y)));
	verts.push_back(Vertex_PCU(pointC, color, UVs.m_maxs));

	verts.push_back(Vertex_PCU(pointA, color, UVs.m_mins));
	verts.push_back(Vertex_PCU(pointC, color, UVs.m_maxs));
	verts.push_back(Vertex_PCU(pointD, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y)));
}

void AddVertsForQuad3D(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Vec3& topLeft, const Rgba8& color, const AABB2& UVs)
{
	verts.push_back(Vertex_PCU(bottomLeft, color, UVs.m_mins));
	verts.push_back(Vertex_PCU(bottomRight, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y)));
	verts.push_back(Vertex_PCU(topRight, color, UVs.m_maxs));
	verts.push_back(Vertex_PCU(topLeft, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y)));

	int nextIndex = (int)verts.size();
	indexes.push_back(nextIndex-4);
	indexes.push_back(nextIndex-3);
	indexes.push_back(nextIndex-2);
	indexes.push_back(nextIndex-4);
	indexes.push_back(nextIndex-2);
	indexes.push_back(nextIndex-1);
}

void AddVertsForQuad3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Vec3& topLeft, const Vec3& normal, const Rgba8& color, const AABB2& UVs)
{
	verts.push_back(Vertex_PCUTBN(bottomLeft, color, UVs.m_mins, Vec3(), Vec3(), normal));
	verts.push_back(Vertex_PCUTBN(bottomRight, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y), Vec3(), Vec3(), normal));
	verts.push_back(Vertex_PCUTBN(topRight, color, UVs.m_maxs, Vec3(), Vec3(), normal));
	verts.push_back(Vertex_PCUTBN(topLeft, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y), Vec3(), Vec3(), normal));

	int nextIndex = (int)verts.size();
	indexes.push_back(nextIndex - 4);
	indexes.push_back(nextIndex - 3);
	indexes.push_back(nextIndex - 2);
	indexes.push_back(nextIndex - 4);
	indexes.push_back(nextIndex - 2);
	indexes.push_back(nextIndex - 1);
}

void AddVertsForQuad3D(std::vector<Vertex_PCUTBN>& verts, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Vec3& topLeft, const Vec3& normal, const Rgba8& color, const AABB2& UVs)
{
	verts.push_back(Vertex_PCUTBN(bottomLeft, color, UVs.m_mins, Vec3(), Vec3(), normal));
	verts.push_back(Vertex_PCUTBN(bottomRight, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y), Vec3(), Vec3(), normal));
	verts.push_back(Vertex_PCUTBN(topRight, color, UVs.m_maxs, Vec3(), Vec3(), normal));

	verts.push_back(Vertex_PCUTBN(topRight, color, UVs.m_maxs, Vec3(), Vec3(), normal));
	verts.push_back(Vertex_PCUTBN(topLeft, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y), Vec3(), Vec3(), normal));
	verts.push_back(Vertex_PCUTBN(bottomLeft, color, UVs.m_mins, Vec3(), Vec3(), normal));
}

void AddVertsForRoundedQuad3D(std::vector<Vertex_PCUTBN>& verts, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Vec3& topLeft, const Vec3& normal, const Rgba8& color, const AABB2& UVs)
{
	Vec3 splitBottom = bottomLeft + ((bottomRight - bottomLeft) * .5f);
	Vec3 splitTop = topLeft + ((topRight - topLeft) * .5f);
	Vec3 leftSideNormal = CrossProduct3D(normal, Vec3(0, 0, 1));
	Vec3 rightSideNormal = -CrossProduct3D(normal, Vec3(0, 0, 1));
	float halfUVX = UVs.m_mins.x + ((UVs.m_maxs.x - UVs.m_mins.x) * .5f);
	//Left Side
	verts.push_back(Vertex_PCUTBN(bottomLeft, color, UVs.m_mins, Vec3(), Vec3(), leftSideNormal));
	verts.push_back(Vertex_PCUTBN(splitBottom, color, Vec2(halfUVX, UVs.m_mins.y), Vec3(), Vec3(), normal));
	verts.push_back(Vertex_PCUTBN(splitTop, color, Vec2(halfUVX, UVs.m_maxs.y), Vec3(), Vec3(), normal));

	verts.push_back(Vertex_PCUTBN(splitTop, color, Vec2(halfUVX, UVs.m_maxs.y), Vec3(), Vec3(), normal));
	verts.push_back(Vertex_PCUTBN(topLeft, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y), Vec3(), Vec3(), leftSideNormal));
	verts.push_back(Vertex_PCUTBN(bottomLeft, color, UVs.m_mins, Vec3(), Vec3(), leftSideNormal));

	//Right Side
	verts.push_back(Vertex_PCUTBN(splitBottom, color, Vec2(halfUVX, UVs.m_mins.y), Vec3(), Vec3(), normal));
	verts.push_back(Vertex_PCUTBN(bottomRight, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y), Vec3(), Vec3(), rightSideNormal));
	verts.push_back(Vertex_PCUTBN(topRight, color, UVs.m_maxs, Vec3(), Vec3(), rightSideNormal));

	verts.push_back(Vertex_PCUTBN(topRight, color, UVs.m_maxs, Vec3(), Vec3(), rightSideNormal));
	verts.push_back(Vertex_PCUTBN(splitTop, color, Vec2(halfUVX, UVs.m_maxs.y), Vec3(), Vec3(), normal));
	verts.push_back(Vertex_PCUTBN(splitBottom, color, Vec2(halfUVX, UVs.m_mins.y), Vec3(), Vec3(), normal));
}

void AddVertsForAABB3D(std::vector<Vertex_PCU>& verts, const AABB3& bounds, const Rgba8& color, const AABB2& UVs)
{
	float width = bounds.m_maxs.x - bounds.m_mins.x;
	float length = bounds.m_maxs.z - bounds.m_mins.z;
	float height = bounds.m_maxs.y - bounds.m_mins.y;

	Vec3 point1 = bounds.m_mins + Vec3(0.f, 0.f, length);
	Vec3 point2 = bounds.m_maxs + Vec3(0.f,-height, 0.f);
	Vec3 point3 = bounds.m_maxs;
	Vec3 point4 = bounds.m_maxs + Vec3(-width, 0.f, 0.f);
	Vec3 point5 = bounds.m_mins + Vec3(width, 0.f, 0.f);
	Vec3 point6 = bounds.m_mins;
	Vec3 point7 = bounds.m_mins + Vec3(0.f, height, 0.f);
	Vec3 point8 = bounds.m_maxs + Vec3(0.f, 0.f, -length);
	AddVertsForQuad3D(verts, point5, point8, point3, point2, color, UVs);
	AddVertsForQuad3D(verts, point7, point6, point1, point4, color, UVs);
	AddVertsForQuad3D(verts, point8, point7, point4, point3, color, UVs);
	AddVertsForQuad3D(verts, point6, point5, point2, point1, color, UVs);
	AddVertsForQuad3D(verts, point1, point2, point3, point4, color, UVs);
	AddVertsForQuad3D(verts, point7, point8, point5, point6, color, UVs);
}

void AddVertsForInverseAABB3D(std::vector<Vertex_PCU>& verts, const AABB3& bounds, const Rgba8& color, const AABB2& UVs)
{
	float width = bounds.m_maxs.x - bounds.m_mins.x;
	float length = bounds.m_maxs.z - bounds.m_mins.z;
	float height = bounds.m_maxs.y - bounds.m_mins.y;

	Vec3 point1 = bounds.m_mins + Vec3(0.f, 0.f, length);
	Vec3 point2 = bounds.m_maxs + Vec3(0.f, -height, 0.f);
	Vec3 point3 = bounds.m_maxs;
	Vec3 point4 = bounds.m_maxs + Vec3(-width, 0.f, 0.f);
	Vec3 point5 = bounds.m_mins + Vec3(width, 0.f, 0.f);
	Vec3 point6 = bounds.m_mins;
	Vec3 point7 = bounds.m_mins + Vec3(0.f, height, 0.f);
	Vec3 point8 = bounds.m_maxs + Vec3(0.f, 0.f, -length);

	// Reverse winding order (note: flip the order of the 4 corners in each quad)
	AddVertsForQuad3D(verts, point8, point5, point2, point3, color, UVs); // +X
	AddVertsForQuad3D(verts, point6, point7, point4, point1, color, UVs); // -X
	AddVertsForQuad3D(verts, point7, point8, point3, point4, color, UVs); // +Y
	AddVertsForQuad3D(verts, point5, point6, point1, point2, color, UVs); // -Y
	AddVertsForQuad3D(verts, point4, point3, point2, point1, color, UVs); // +Z
	AddVertsForQuad3D(verts, point6, point5, point8, point7, color, UVs); // -Z
}

void AddVertsForSkyBoxAABB3D(std::vector<Vertex_PCU>& verts, const AABB3& bounds, const SpriteSheet& sheet, const Rgba8& color)
{
	float width = bounds.m_maxs.x - bounds.m_mins.x;
	float length = bounds.m_maxs.z - bounds.m_mins.z;
	float height = bounds.m_maxs.y - bounds.m_mins.y;

	Vec3 point1 = bounds.m_mins + Vec3(0.f, 0.f, length);
	Vec3 point2 = bounds.m_maxs + Vec3(0.f, -height, 0.f);
	Vec3 point3 = bounds.m_maxs;
	Vec3 point4 = bounds.m_maxs + Vec3(-width, 0.f, 0.f);
	Vec3 point5 = bounds.m_mins + Vec3(width, 0.f, 0.f);
	Vec3 point6 = bounds.m_mins;
	Vec3 point7 = bounds.m_mins + Vec3(0.f, height, 0.f);
	Vec3 point8 = bounds.m_maxs + Vec3(0.f, 0.f, -length);

	AABB2 uvXP = sheet.GetSpriteUVs(IntVec2(1, 1));
	AABB2 uvXN = sheet.GetSpriteUVs(IntVec2(3, 1));
	AABB2 uvYP = sheet.GetSpriteUVs(IntVec2(0, 1));
	AABB2 uvYN = sheet.GetSpriteUVs(IntVec2(2, 1));
	AABB2 uvZP = sheet.GetSpriteUVs(IntVec2(1, 0));
	AABB2 uvZN = sheet.GetSpriteUVs(IntVec2(1, 2));
	// Reverse winding order (note: flip the order of the 4 corners in each quad)
	AddVertsForQuad3D(verts, point8, point5, point2, point3, color, uvXP); // +X
	AddVertsForQuad3D(verts, point6, point7, point4, point1, color, uvXN); // -X
	AddVertsForQuad3D(verts, point7, point8, point3, point4, color, uvYP); // +Y
	AddVertsForQuad3D(verts, point5, point6, point1, point2, color, uvYN); // -Y
	AddVertsForQuad3D(verts, point3, point2, point1, point4, color, uvZP); // +Z
	AddVertsForQuad3D(verts, point6, point5, point8, point7, color, uvZN); // -Z
}

void AddVertsForOBB3D(std::vector<Vertex_PCU>& verts, const OBB3& box, const Rgba8& color, const AABB2& UVs)
{
	Vec3 closeBottomLeft	= Vec3(-box.m_halfDims.x, -box.m_halfDims.y, -box.m_halfDims.z);
	Vec3 closeBottomRight	= Vec3(-box.m_halfDims.x, box.m_halfDims.y, -box.m_halfDims.z);
	Vec3 closeTopLeft		= Vec3(-box.m_halfDims.x, -box.m_halfDims.y, box.m_halfDims.z);
	Vec3 closeTopRight		= Vec3(-box.m_halfDims.x, box.m_halfDims.y, box.m_halfDims.z);
	Vec3 farBottomLeft		= Vec3(box.m_halfDims.x, -box.m_halfDims.y, -box.m_halfDims.z);
	Vec3 farBottomRight		= Vec3(box.m_halfDims.x, box.m_halfDims.y, -box.m_halfDims.z);
	Vec3 farTopLeft			= Vec3(box.m_halfDims.x, -box.m_halfDims.y, box.m_halfDims.z);
	Vec3 farTopRight		= Vec3(box.m_halfDims.x, box.m_halfDims.y, box.m_halfDims.z);

	//Mat44 transformationLocalToWorld = Mat44::MakeTranslation3D(box.m_center);
	Mat44 rotationMatrix = Mat44(box.m_iBasis, box.m_jBasis, box.m_kBasis, box.m_center);
	
	closeBottomLeft		= rotationMatrix.TransformPosition3D(closeBottomLeft);
	closeBottomRight	= rotationMatrix.TransformPosition3D(closeBottomRight);
	closeTopLeft		= rotationMatrix.TransformPosition3D(closeTopLeft);
	closeTopRight		= rotationMatrix.TransformPosition3D(closeTopRight);
	farBottomLeft		= rotationMatrix.TransformPosition3D(farBottomLeft);
	farBottomRight		= rotationMatrix.TransformPosition3D(farBottomRight);
	farTopLeft			= rotationMatrix.TransformPosition3D(farTopLeft);
	farTopRight			= rotationMatrix.TransformPosition3D(farTopRight);

	AddVertsForQuad3D(verts, farBottomLeft, farBottomRight, farTopRight, farTopLeft, color, UVs); // +X
	AddVertsForQuad3D(verts, closeBottomRight, farBottomRight, farBottomLeft, closeBottomLeft, color, UVs); //-Z
	AddVertsForQuad3D(verts, closeTopLeft, farTopLeft, farTopRight, closeTopRight, color, UVs); //+Z
	AddVertsForQuad3D(verts, closeBottomLeft, farBottomLeft, farTopLeft, closeTopLeft, color, UVs); //-Y
	AddVertsForQuad3D(verts, farBottomRight, closeBottomRight, closeTopRight, farTopRight, color, UVs); //+Y
	AddVertsForQuad3D(verts, closeBottomRight, closeBottomLeft, closeTopLeft, closeTopRight, color, UVs); //-X
}

void AddVertsForCylinder3D(std::vector<Vertex_PCU>& verts, const Vec3& start, const Vec3& end, float radius, const Rgba8& color, const AABB2& UVs, int numSlices)
{
	float UVXRange = UVs.m_maxs.x - UVs.m_mins.x;
	//Bfloat UVYRange = UVs.m_maxs.y - UVs.m_mins.y;

	float degreesPerSide = 360.f / numSlices;
	Vec3 fwdNormal = (end - start).GetNormalized();

	Vec3 reference;
	if (abs(fwdNormal.y) < 0.9f)
	{
		reference = Vec3(0, 1, 0);
	}
	else
	{
		reference = Vec3(0, 0, 1);
	}
	Vec3 j = CrossProduct3D(reference, fwdNormal).GetNormalized();
	Vec3 k = CrossProduct3D(fwdNormal, j);

	for (int i = 0; i < numSlices; i++)
	{
		float yaw1 = degreesPerSide * i;
		float yaw2 = degreesPerSide * (i + 1);
		float yaw1Percent = UVXRange * (yaw1 / 360.f);
		float yaw2Percent = UVXRange * (yaw2 / 360.f);

		float uCoordRound1 = (CosDegrees(yaw1) * .5f) + .5f;
		float vCoordRound1 = (SinDegrees(yaw1) * .5f) + .5f;
		float uCoordRound2 = (CosDegrees(yaw2) * .5f) + .5f;
		float vCoordRound2 = (SinDegrees(yaw2) * .5f) + .5f;

		Vec3 discPoint1 = start + (j * CosDegrees(yaw1) + k * SinDegrees(yaw1)) * radius;
		Vec3 discPoint2 = start + (j * CosDegrees(yaw2) + k * SinDegrees(yaw2)) * radius;
		Vec3 discPointTop1 = end + (j * CosDegrees(yaw1) + k * SinDegrees(yaw1)) * radius;
		Vec3 discPointTop2 = end + (j * CosDegrees(yaw2) + k * SinDegrees(yaw2)) * radius;

		// Base
		verts.push_back(Vertex_PCU(start, color, Vec2(0.5f, 0.5f)));
		verts.push_back(Vertex_PCU(discPoint2, color, Vec2(uCoordRound2, vCoordRound2)));
		verts.push_back(Vertex_PCU(discPoint1, color, Vec2(uCoordRound1, vCoordRound1)));

		// Wall
		AddVertsForQuad3D(verts, discPoint1, discPoint2, discPointTop2, discPointTop1, color, AABB2(yaw1Percent, 0.f, yaw2Percent, 1.f));

		// Cap
		verts.push_back(Vertex_PCU(end, color, Vec2(0.5f, 0.5f)));
		verts.push_back(Vertex_PCU(discPointTop1, color, Vec2(uCoordRound1, vCoordRound1)));
		verts.push_back(Vertex_PCU(discPointTop2, color, Vec2(uCoordRound2, vCoordRound2)));
	}
}

void AddVertsForCone3D(std::vector<Vertex_PCU>& verts, const Vec3& start, const Vec3& end, float radius, const Rgba8& color, const AABB2& UVs, int numSlices)
{
	UVs;
	float degreesPerSide = 360.f / numSlices;
	Vec3 fwdNormal = (end - start).GetNormalized();

	Vec3 reference;
	if (abs(fwdNormal.y) < 0.9f)
	{
		reference = Vec3(0, 1, 0);
	}
	else
	{
		reference = Vec3(0, 0, 1);
	}
	Vec3 j = CrossProduct3D(reference, fwdNormal).GetNormalized();
	Vec3 k = CrossProduct3D(fwdNormal, j);

	for (int i = 0; i < numSlices; i++)
	{
		float yaw1 = degreesPerSide * i;
		float yaw2 = degreesPerSide * (i + 1);

		Vec3 discPoint1 = start + (j * CosDegrees(yaw1) + k * SinDegrees(yaw1)) * radius;
		Vec3 discPoint2 = start + (j * CosDegrees(yaw2) + k * SinDegrees(yaw2)) * radius;

		//Base
		verts.push_back(Vertex_PCU(start, color, Vec2(0, 0)));
		verts.push_back(Vertex_PCU(discPoint2, color, Vec2(0, 0)));
		verts.push_back(Vertex_PCU(discPoint1, color, Vec2(0, 0)));

		//Point
		verts.push_back(Vertex_PCU(end, color, Vec2(0, 0)));
		verts.push_back(Vertex_PCU(discPoint1, color, Vec2(0, 0)));
		verts.push_back(Vertex_PCU(discPoint2, color, Vec2(0, 0)));
	}
}

void AddVertsForRoundArrow3D(std::vector<Vertex_PCU>& verts, const Vec3& start, const Vec3& end, float radius, const Rgba8& color, int numSlices)
{
	Vec3 fwdVectorHead = (end - start);
	Vec3 backArrowHeadAdjust = (fwdVectorHead.GetNormalized()) * -radius * 3.0f;
	Vec3 fwdVectorCylinder = fwdVectorHead + backArrowHeadAdjust;

	AddVertsForCylinder3D(verts, start, start + fwdVectorCylinder, radius, color, AABB2::ZERO_TO_ONE, numSlices);
	AddVertsForCone3D(verts, start+fwdVectorCylinder, start+fwdVectorHead, radius * 1.5f, color, AABB2::ZERO_TO_ONE, numSlices);
}

void AddVertsForSphere(std::vector<Vertex_PCU>& verts, const Vec3& center, float radius, const Rgba8& color, const AABB2& UVs, int numSlices, int numStacks)
{
	float horizontalAngleStep = 360.f / numSlices;
	float verticalAngleStep = 180.f / numStacks;
	float UVXRange = UVs.m_maxs.x - UVs.m_mins.x;
	float UVYRange = UVs.m_maxs.y - UVs.m_mins.y;

	for (int stack = 0; stack < numStacks; stack++) {
		float pitch1 = (stack * verticalAngleStep) - 90.f;
		float pitch2 = ((stack + 1) * verticalAngleStep) - 90.f;
		float pitch1Percent = UVYRange * ((pitch1+90.f) / 180.f);
		float pitch2Percent = UVYRange * ((pitch2+90.f) / 180.f);

		for (int slice = 0; slice < numSlices; slice++) {
			float yaw1 = slice * horizontalAngleStep;
			float yaw2 = (slice + 1)  * horizontalAngleStep;
			float yaw1Percent = UVXRange * (yaw1 / 360.f);
			float yaw2Percent = UVXRange * (yaw2 / 360.f);

			Vec3 bottomLeft = center + Vec3::MakeFromPolarDegrees(yaw1, pitch1, radius);
			Vec3 bottomRight = center + Vec3::MakeFromPolarDegrees(yaw2, pitch1, radius);
			Vec3 topLeft = center + Vec3::MakeFromPolarDegrees(yaw1, pitch2, radius);
			Vec3 topRight = center + Vec3::MakeFromPolarDegrees(yaw2, pitch2, radius);

			if (stack == 0) {
				// Bottom cap - single triangle
				verts.push_back(Vertex_PCU(bottomLeft, color, Vec2((yaw1Percent + yaw2Percent )/ 2.f, pitch1Percent)));
				verts.push_back(Vertex_PCU(topRight, color, Vec2(yaw1Percent, pitch2Percent)));
				verts.push_back(Vertex_PCU(topLeft, color, Vec2(yaw2Percent,pitch2Percent)));
			}
			else if (stack == numStacks - 1) {
				// Top cap - single triangle
				verts.push_back(Vertex_PCU(bottomLeft, color, Vec2(yaw1Percent, pitch1Percent)));
				verts.push_back(Vertex_PCU(bottomRight, color, Vec2(yaw2Percent, pitch1Percent)));
				verts.push_back(Vertex_PCU(topLeft, color, Vec2((yaw1Percent + yaw2Percent) / 2.f, pitch2Percent)));
			}
			else {
				//Middle stacks - quad
				AddVertsForQuad3D(verts, bottomLeft, bottomRight, topRight, topLeft, color, AABB2(yaw1Percent,pitch1Percent,yaw2Percent,pitch2Percent));
			}
		}
	}
}

void AddVertsForLitAABB3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, const AABB3& bounds, const Rgba8& color, const AABB2& UVs)
{
	float width = bounds.m_maxs.x - bounds.m_mins.x;
	float length = bounds.m_maxs.z - bounds.m_mins.z;
	float height = bounds.m_maxs.y - bounds.m_mins.y;

	Vec3 point1 = bounds.m_mins + Vec3(0.f, 0.f, length);
	Vec3 point2 = bounds.m_maxs + Vec3(0.f, -height, 0.f);
	Vec3 point3 = bounds.m_maxs;
	Vec3 point4 = bounds.m_maxs + Vec3(-width, 0.f, 0.f);
	Vec3 point5 = bounds.m_mins + Vec3(width, 0.f, 0.f);
	Vec3 point6 = bounds.m_mins;
	Vec3 point7 = bounds.m_mins + Vec3(0.f, height, 0.f);
	Vec3 point8 = bounds.m_maxs + Vec3(0.f, 0.f, -length);

	AddVertsForQuad3D(verts, indexes, point5, point8, point3, point2, Vec3(1, 0, 0), color, UVs); // +X
	AddVertsForQuad3D(verts, indexes, point7, point6, point1, point4, Vec3(-1, 0, 0), color, UVs); // -X
	AddVertsForQuad3D(verts, indexes, point8, point7, point4, point3, Vec3(0, 1, 0), color, UVs); // +Y
	AddVertsForQuad3D(verts, indexes, point6, point5, point2, point1, Vec3(0, -1, 0), color, UVs); // -Y
	AddVertsForQuad3D(verts, indexes, point1, point2, point3, point4, Vec3(0, 0, 1), color, UVs); // +Z
	AddVertsForQuad3D(verts, indexes, point7, point8, point5, point6, Vec3(0, 0, -1), color, UVs); // -Z
}

void AddVertsForLitOBB3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, const OBB3& box, const Rgba8& color, const AABB2& UVs)
{
	Vec3 closeBottomLeft = Vec3(-box.m_halfDims.x, -box.m_halfDims.y, -box.m_halfDims.z);
	Vec3 closeBottomRight = Vec3(-box.m_halfDims.x, box.m_halfDims.y, -box.m_halfDims.z);
	Vec3 closeTopLeft = Vec3(-box.m_halfDims.x, -box.m_halfDims.y, box.m_halfDims.z);
	Vec3 closeTopRight = Vec3(-box.m_halfDims.x, box.m_halfDims.y, box.m_halfDims.z);
	Vec3 farBottomLeft = Vec3(box.m_halfDims.x, -box.m_halfDims.y, -box.m_halfDims.z);
	Vec3 farBottomRight = Vec3(box.m_halfDims.x, box.m_halfDims.y, -box.m_halfDims.z);
	Vec3 farTopLeft = Vec3(box.m_halfDims.x, -box.m_halfDims.y, box.m_halfDims.z);
	Vec3 farTopRight = Vec3(box.m_halfDims.x, box.m_halfDims.y, box.m_halfDims.z);

	//Mat44 transformationLocalToWorld = Mat44::MakeTranslation3D(box.m_center);
	Mat44 rotationMatrix = Mat44(box.m_iBasis, box.m_jBasis, box.m_kBasis, box.m_center);

	closeBottomLeft = rotationMatrix.TransformPosition3D(closeBottomLeft);
	closeBottomRight = rotationMatrix.TransformPosition3D(closeBottomRight);
	closeTopLeft = rotationMatrix.TransformPosition3D(closeTopLeft);
	closeTopRight = rotationMatrix.TransformPosition3D(closeTopRight);
	farBottomLeft = rotationMatrix.TransformPosition3D(farBottomLeft);
	farBottomRight = rotationMatrix.TransformPosition3D(farBottomRight);
	farTopLeft = rotationMatrix.TransformPosition3D(farTopLeft);
	farTopRight = rotationMatrix.TransformPosition3D(farTopRight);

	AddVertsForQuad3D(verts, indexes, farBottomLeft, farBottomRight, farTopRight, farTopLeft, box.m_iBasis, color, UVs);
	AddVertsForQuad3D(verts, indexes, closeBottomRight, farBottomRight, farBottomLeft, closeBottomLeft, -box.m_kBasis, color, UVs);
	AddVertsForQuad3D(verts, indexes, closeTopLeft, farTopLeft, farTopRight, closeTopRight, box.m_kBasis, color, UVs);
	AddVertsForQuad3D(verts, indexes, closeBottomLeft, farBottomLeft, farTopLeft, closeTopLeft, -box.m_jBasis, color, UVs);
	AddVertsForQuad3D(verts, indexes, farBottomRight, closeBottomRight, closeTopRight, farTopRight, box.m_jBasis, color, UVs);
	AddVertsForQuad3D(verts, indexes, closeBottomRight, closeBottomLeft, closeTopLeft, closeTopRight, -box.m_iBasis, color, UVs);
}

void AddVertsForLitCylinder3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, const Vec3& start, const Vec3& end, float radius, const Rgba8& color, const AABB2& UVs, int numSlices)
{
	float UVXRange = UVs.m_maxs.x - UVs.m_mins.x;
	//Bfloat UVYRange = UVs.m_maxs.y - UVs.m_mins.y;

	float degreesPerSide = 360.f / numSlices;
	Vec3 fwdNormal = (end - start).GetNormalized();

	Vec3 reference;
	if (abs(fwdNormal.y) < 0.9f)
	{
		reference = Vec3(0, 1, 0);
	}
	else
	{
		reference = Vec3(0, 0, 1);
	}
	Vec3 j = CrossProduct3D(reference, fwdNormal).GetNormalized();
	Vec3 k = CrossProduct3D(fwdNormal, j);

	for (int i = 0; i < numSlices; i++)
	{
		float yaw1 = degreesPerSide * i;
		float yaw2 = degreesPerSide * (i + 1);
		float yaw1Percent = UVXRange * (yaw1 / 360.f);
		float yaw2Percent = UVXRange * (yaw2 / 360.f);

		float uCoordRound1 = (CosDegrees(yaw1) * .5f) + .5f;
		float vCoordRound1 = (SinDegrees(yaw1) * .5f) + .5f;
		float uCoordRound2 = (CosDegrees(yaw2) * .5f) + .5f;
		float vCoordRound2 = (SinDegrees(yaw2) * .5f) + .5f;

		Vec3 discPoint1 = start + (j * CosDegrees(yaw1) + k * SinDegrees(yaw1)) * radius;
		Vec3 discPoint2 = start + (j * CosDegrees(yaw2) + k * SinDegrees(yaw2)) * radius;
		Vec3 discPointTop1 = end + (j * CosDegrees(yaw1) + k * SinDegrees(yaw1)) * radius;
		Vec3 discPointTop2 = end + (j * CosDegrees(yaw2) + k * SinDegrees(yaw2)) * radius;

		Vec3 circularNormal = (((discPointTop2 - discPointTop1) / 2.f) - end).GetNormalized();

		// Base
		verts.push_back(Vertex_PCUTBN(start, color, Vec2(0.5f, 0.5f), Vec3(), Vec3(), -fwdNormal));
		verts.push_back(Vertex_PCUTBN(discPoint2, color, Vec2(uCoordRound2, vCoordRound2), Vec3(), Vec3(), -fwdNormal));
		verts.push_back(Vertex_PCUTBN(discPoint1, color, Vec2(uCoordRound1, vCoordRound1), Vec3(), Vec3(), -fwdNormal));
		int nextIndex = (int)verts.size();

		indexes.push_back(nextIndex - 3);
		indexes.push_back(nextIndex - 2);
		indexes.push_back(nextIndex - 1);

		// Wall
		Vec3 normal = Vec3();
		verts.push_back(Vertex_PCUTBN(discPoint1, color, Vec2(yaw1Percent, 0.f), Vec3(), Vec3(), (discPoint1 - start).GetNormalized()));
		verts.push_back(Vertex_PCUTBN(discPoint2, color, Vec2(yaw2Percent, 0.f), Vec3(), Vec3(), (discPoint2 - start).GetNormalized()));
		verts.push_back(Vertex_PCUTBN(discPointTop2, color, Vec2(yaw2Percent, 1.f), Vec3(), Vec3(), (discPointTop2 - end).GetNormalized()));
		verts.push_back(Vertex_PCUTBN(discPointTop1, color, Vec2(yaw1Percent, 1.f), Vec3(), Vec3(), (discPointTop1 - end).GetNormalized()));

		nextIndex = (int)verts.size();
		indexes.push_back(nextIndex - 4);
		indexes.push_back(nextIndex - 3);
		indexes.push_back(nextIndex - 2);
		indexes.push_back(nextIndex - 4);
		indexes.push_back(nextIndex - 2);
		indexes.push_back(nextIndex - 1);

		// Cap
		verts.push_back(Vertex_PCUTBN(end, color, Vec2(0.5f, 0.5f), Vec3(), Vec3(), fwdNormal));
		verts.push_back(Vertex_PCUTBN(discPointTop1, color, Vec2(uCoordRound1, vCoordRound1), Vec3(), Vec3(), fwdNormal));
		verts.push_back(Vertex_PCUTBN(discPointTop2, color, Vec2(uCoordRound2, vCoordRound2), Vec3(), Vec3(), fwdNormal));

		nextIndex = (int)verts.size();
		indexes.push_back(nextIndex - 3);
		indexes.push_back(nextIndex - 2);
		indexes.push_back(nextIndex - 1);
	}
}

void AddVertsForLitCone3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, const Vec3& start, const Vec3& end, float radius, const Rgba8& color, const AABB2& UVs, int numSlices)
{
	UVs;
	float degreesPerSide = 360.f / numSlices;
	Vec3 fwdNormal = (end - start).GetNormalized();

	Vec3 reference;
	if (abs(fwdNormal.y) < 0.9f)
	{
		reference = Vec3(0, 1, 0);
	}
	else
	{
		reference = Vec3(0, 0, 1);
	}
	Vec3 j = CrossProduct3D(reference, fwdNormal).GetNormalized();
	Vec3 k = CrossProduct3D(fwdNormal, j);

	for (int i = 0; i < numSlices; i++)
	{
		float yaw1 = degreesPerSide * i;
		float yaw2 = degreesPerSide * (i + 1);

		Vec3 discPoint1 = start + (j * CosDegrees(yaw1) + k * SinDegrees(yaw1)) * radius;
		Vec3 discPoint2 = start + (j * CosDegrees(yaw2) + k * SinDegrees(yaw2)) * radius;

		Vec3 discPoint3 = start + (j * CosDegrees((yaw1 + yaw2) * .5f) + k * SinDegrees((yaw1 + yaw2) * .5f)) * radius;

		//Base
		verts.push_back(Vertex_PCUTBN(start, color, Vec2(0, 0), Vec3(), Vec3(), -fwdNormal));
		verts.push_back(Vertex_PCUTBN(discPoint2, color, Vec2(0, 0), Vec3(), Vec3(), -fwdNormal));
		verts.push_back(Vertex_PCUTBN(discPoint1, color, Vec2(0, 0), Vec3(), Vec3(), -fwdNormal));
		int nextIndex = (int)verts.size();
		indexes.push_back(nextIndex - 3);
		indexes.push_back(nextIndex - 2);
		indexes.push_back(nextIndex - 1);

		//Point
		Vec3 p3Normal = CrossProduct3D((end - discPoint3), discPoint2 - discPoint1).GetNormalized();
		verts.push_back(Vertex_PCUTBN(end, color, Vec2(0, 0), Vec3(), Vec3(), -p3Normal));
		Vec3 p1Normal = CrossProduct3D((end - discPoint1), discPoint2 - discPoint1).GetNormalized();
		Vec3 p2Normal = CrossProduct3D((end - discPoint2), discPoint2 - discPoint1).GetNormalized();
		verts.push_back(Vertex_PCUTBN(discPoint1, color, Vec2(0, 0), Vec3(), Vec3(), -p1Normal));
		verts.push_back(Vertex_PCUTBN(discPoint2, color, Vec2(0, 0), Vec3(), Vec3(), -p2Normal));
		nextIndex = (int)verts.size();
		indexes.push_back(nextIndex - 3);
		indexes.push_back(nextIndex - 2);
		indexes.push_back(nextIndex - 1);
	}
}

//void AddVertsForLitRoundArrow3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, const Vec3& start, const Vec3& end, float radius, const Rgba8& color, int numSlices)
//{
//}

void AddVertsForLitSphere(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, const Vec3& center, float radius, const Rgba8& color, const AABB2& UVs, int numSlices, int numStacks)
{
	float horizontalAngleStep = 360.f / numSlices;
	float verticalAngleStep = 180.f / numStacks;
	float UVXRange = UVs.m_maxs.x - UVs.m_mins.x;
	float UVYRange = UVs.m_maxs.y - UVs.m_mins.y;

	for (int stack = 0; stack < numStacks; stack++) {
		float pitch1 = (stack * verticalAngleStep) - 90.f;
		float pitch2 = ((stack + 1) * verticalAngleStep) - 90.f;
		float pitch1Percent = UVYRange * ((pitch1 + 90.f) / 180.f);
		float pitch2Percent = UVYRange * ((pitch2 + 90.f) / 180.f);

		for (int slice = 0; slice < numSlices; slice++) {
			float yaw1 = slice * horizontalAngleStep;
			float yaw2 = (slice + 1) * horizontalAngleStep;
			float yaw1Percent = UVXRange * (yaw1 / 360.f);
			float yaw2Percent = UVXRange * (yaw2 / 360.f);

			Vec3 bottomLeft = center + Vec3::MakeFromPolarDegrees(yaw1, pitch1, radius);
			Vec3 bottomRight = center + Vec3::MakeFromPolarDegrees(yaw2, pitch1, radius);
			Vec3 topLeft = center + Vec3::MakeFromPolarDegrees(yaw1, pitch2, radius);
			Vec3 topRight = center + Vec3::MakeFromPolarDegrees(yaw2, pitch2, radius);

			if (stack == 0) {
				// Bottom cap - single triangle
				verts.push_back(Vertex_PCUTBN(bottomLeft, color, Vec2((yaw1Percent + yaw2Percent) * .5f, pitch1Percent), Vec3(), Vec3(), (bottomLeft - center).GetNormalized()));
				verts.push_back(Vertex_PCUTBN(topRight, color, Vec2(yaw1Percent, pitch2Percent), Vec3(), Vec3(), (topRight - center).GetNormalized()));
				verts.push_back(Vertex_PCUTBN(topLeft, color, Vec2(yaw2Percent, pitch2Percent), Vec3(), Vec3(), (topLeft - center).GetNormalized()));
				int nextIndex = (int)verts.size();
				indexes.push_back(nextIndex - 3);
				indexes.push_back(nextIndex - 2);
				indexes.push_back(nextIndex - 1);
			}
			else if (stack == numStacks - 1) {
				// Top cap - single triangle
				verts.push_back(Vertex_PCUTBN(bottomLeft, color, Vec2(yaw1Percent, pitch1Percent), Vec3(), Vec3(), (bottomLeft - center).GetNormalized()));
				verts.push_back(Vertex_PCUTBN(bottomRight, color, Vec2(yaw2Percent, pitch1Percent), Vec3(), Vec3(), (bottomRight - center).GetNormalized()));
				verts.push_back(Vertex_PCUTBN(topLeft, color, Vec2((yaw1Percent + yaw2Percent) * .5f, pitch2Percent), Vec3(), Vec3(), (topLeft - center).GetNormalized()));
				int nextIndex = (int)verts.size();
				indexes.push_back(nextIndex - 3);
				indexes.push_back(nextIndex - 2);
				indexes.push_back(nextIndex - 1);
			}
			else {
				//Middle stacks - quad
				verts.push_back(Vertex_PCUTBN(bottomLeft, color, Vec2(yaw1Percent, pitch1Percent), Vec3(), Vec3(), (bottomLeft - center).GetNormalized()));
				verts.push_back(Vertex_PCUTBN(bottomRight, color, Vec2(yaw2Percent, pitch1Percent), Vec3(), Vec3(), (bottomRight - center).GetNormalized()));
				verts.push_back(Vertex_PCUTBN(topRight, color, Vec2(yaw2Percent, pitch2Percent), Vec3(), Vec3(), (topRight - center).GetNormalized()));
				verts.push_back(Vertex_PCUTBN(topLeft, color, Vec2(yaw1Percent, pitch2Percent), Vec3(), Vec3(), (topLeft - center).GetNormalized()));
				int nextIndex = (int)verts.size();
				indexes.push_back(nextIndex - 4);
				indexes.push_back(nextIndex - 3);
				indexes.push_back(nextIndex - 2);
				indexes.push_back(nextIndex - 4);
				indexes.push_back(nextIndex - 2);
				indexes.push_back(nextIndex - 1);
			}
		}
	}
}

void TransformVertexArray(std::vector<Vertex_PCU>& verts, Vec3 const& translation, float scale, float rotationDegrees)
{
	for (int i = 0; i < verts.size(); i++)
	{
		verts[i].m_position *= scale;
		verts[i].m_position = verts[i].m_position.GetRotatedAboutZDegrees(rotationDegrees);
		verts[i].m_position += translation;
	}
}

void TransformVertexArray3D(std::vector<Vertex_PCU>& verts, const Mat44& transformation)
{
	for (int i = 0; i < (int)verts.size(); i++)
	{
		verts[i].m_position = transformation.TransformPosition3D(verts[i].m_position);
	}
}

AABB2 GetVertexBounds2D(std::vector<Vertex_PCU>& verts)
{
	float minX = 0.f;
	float minY = 0.f;
	float maxX = 0.f;
	float maxY = 0.f;
	for (int i = 0; i < (int)verts.size(); i++)
	{
		Vec3 currentVec = verts[i].m_position;
		if (currentVec.x < minX)
		{
			minX = currentVec.x;
		}
		if (currentVec.y < minY)
		{
			minY = currentVec.y;
		}
		if (currentVec.x > maxX)
		{
			maxX = currentVec.x;
		}
		if (currentVec.y > maxY)
		{
			maxY = currentVec.y;
		}
	}

	return AABB2(minX,minY,maxX,maxY);
}