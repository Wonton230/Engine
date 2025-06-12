#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Vec4.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Math/LineSegment2.hpp"
#include "Engine/Math/OBB2.hpp"
#include "Engine/Math/Capsule2.hpp"
#include "Engine/Math/Triangle2.hpp"
#include "Engine/Math/Mat44.hpp"
#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Math/OBB3.hpp"
#include "Engine/Math/Plane3.hpp"

Mat44 GetBillboardTransform(BillBoardType billboardType, Mat44 const& targetTransform, const Vec3& billboardPositionBottomLeft, const Vec2& billboardSize)
{
	if (billboardType == BillBoardType::COUNT || billboardType == BillBoardType::NONE)
	{
		return Mat44();
	}

	Vec3 targetPosition;
	Vec3 boardToTargetIbasis;
	Vec3 boardToTargetJbasis;
	Vec3 boardToTargetKbasis;
	Mat44 returned;
	Mat44 temp;
	Mat44 centerOffset;

	switch (billboardType)
	{
	case BillBoardType::FULL_FACING:
		targetPosition = targetTransform.GetTranslation3D();
		boardToTargetIbasis = (targetPosition - billboardPositionBottomLeft) / (targetPosition - billboardPositionBottomLeft).GetLength();
		boardToTargetJbasis;
		boardToTargetKbasis;
		if (DotProduct3D(boardToTargetIbasis, Vec3(0, 0, 1)) < 1)
		{
			boardToTargetJbasis = CrossProduct3D(Vec3(0, 0, 1), boardToTargetIbasis);
			boardToTargetJbasis = boardToTargetJbasis / boardToTargetJbasis.GetLength();
			boardToTargetKbasis = CrossProduct3D(boardToTargetIbasis, boardToTargetJbasis);
		}
		else
		{
			boardToTargetKbasis = CrossProduct3D(Vec3(0, 1, 0), boardToTargetIbasis);
			boardToTargetKbasis = boardToTargetKbasis / boardToTargetKbasis.GetLength();
			boardToTargetJbasis = CrossProduct3D(boardToTargetKbasis, boardToTargetIbasis);
		}
		temp.SetIJK3D(boardToTargetIbasis, boardToTargetJbasis, boardToTargetKbasis);
		centerOffset = Mat44::MakeTranslation3D(Vec3(0, -billboardSize.x * 0.5f, -billboardSize.y * 0.5f));
		returned = Mat44::MakeTranslation3D(billboardPositionBottomLeft);
		returned.Append(temp);
		returned.Append(centerOffset);
		return returned;
		break;
	case BillBoardType::FULL_OPPOSING:
		temp.SetIJK3D(-targetTransform.GetIBasis3D(), -targetTransform.GetJBasis3D(), targetTransform.GetKBasis3D());
		centerOffset = Mat44::MakeTranslation3D(Vec3(0, -billboardSize.x * 0.5f, -billboardSize.y * 0.5f));
		returned = Mat44::MakeTranslation3D(billboardPositionBottomLeft);
		returned.Append(temp);
		returned.Append(centerOffset);
		return returned;
		break;
	case BillBoardType::WORLD_UP_FACING:
		targetPosition = targetTransform.GetTranslation3D();
		boardToTargetIbasis = targetPosition - billboardPositionBottomLeft;
		boardToTargetIbasis.z = 0.f;
		boardToTargetIbasis = boardToTargetIbasis / boardToTargetIbasis.GetLength();
		boardToTargetKbasis = Vec3(0, 0, 1);
		boardToTargetJbasis = CrossProduct3D(boardToTargetKbasis, boardToTargetIbasis);
		temp.SetIJK3D(boardToTargetIbasis, boardToTargetJbasis, boardToTargetKbasis);
		centerOffset = Mat44::MakeTranslation3D(Vec3(0, -billboardSize.x * 0.5f, -billboardSize.y * 0.5f));
		returned = Mat44::MakeTranslation3D(billboardPositionBottomLeft);
		returned.Append(temp);
		returned.Append(centerOffset);
		return returned;
		break;
	case BillBoardType::WORLD_UP_OPPOSING:
		boardToTargetIbasis = -targetTransform.GetIBasis3D();
		boardToTargetIbasis.z = 0.f;
		boardToTargetIbasis = boardToTargetIbasis / boardToTargetIbasis.GetLength();
		boardToTargetKbasis = Vec3(0, 0, 1);
		boardToTargetJbasis = CrossProduct3D(boardToTargetKbasis, boardToTargetIbasis);
		temp.SetIJK3D(boardToTargetIbasis, boardToTargetJbasis, boardToTargetKbasis);
		centerOffset = Mat44::MakeTranslation3D(Vec3(0, -billboardSize.x * 0.5f, -billboardSize.y * 0.5f));
		returned = Mat44::MakeTranslation3D(billboardPositionBottomLeft);
		returned.Append(temp);
		returned.Append(centerOffset);
		return returned;
		break;
	}
	return Mat44();
}

//Angle Functions ///////////////////////////////////////////////////////////////////////////////////////////
float ConvertDegreesToRadians(float degrees)
{
	return degrees * (3.14159265f / 180.f);
}

float ConvertRadiansToDegrees(float radians)
{
	return radians * (180.f / 3.14159265f);
}

float CosDegrees(float degrees)
{
	float rads = ConvertDegreesToRadians(degrees);
	return cosf(rads);
}

float SinDegrees(float degrees)
{
	float rads = ConvertDegreesToRadians(degrees);
	return sinf(rads);
}

float Atan2Degrees(float y, float x)
{
	//atan2f returns in rads and we want it in degrees
	return ConvertRadiansToDegrees(atan2f(y, x));
}

float GetShortestAngularDispDegrees(float startDeg, float endDeg)
{
	float displacement = endDeg - startDeg;
	while (displacement > 180)
		displacement -= 360;
	while (displacement < -180)
		displacement += 360;

	return displacement;
}

float GetTurnedTowardDegrees(float currentDeg, float targetDeg, float maxDeltaDeg)
{
	float turnDeg = GetShortestAngularDispDegrees(currentDeg, targetDeg);
	while (currentDeg >= 360)
		currentDeg -= 360;
	return currentDeg + GetClamped(turnDeg, -maxDeltaDeg, maxDeltaDeg);
}

float GetAngleDegreesBetweenVectors2D(Vec2 const& vectorA, Vec2 const& vectorB)
{
	float dotProductNormalized = DotProduct2D(vectorA.GetNormalized(), vectorB.GetNormalized());
	return ConvertRadiansToDegrees(acosf(dotProductNormalized));
}



float GetDistance2D(Vec2 const& posA, Vec2 const& posB)
{
	float distX = posB.x - posA.x;
	float distY = posB.y - posA.y;
	return sqrtf((distX * distX) + (distY * distY));
}

float GetDistanceSquared2D(Vec2 const& posA, Vec2 const& posB)
{
	float distX = posB.x - posA.x;
	float distY = posB.y - posA.y;
	return (distX * distX) + (distY * distY);
}

float GetDistance3D(Vec3 const& posA, Vec3 const& posB)
{
	float distX = posB.x - posA.x;
	float distY = posB.y - posA.y;
	float distZ = posB.z - posA.z;
	return sqrtf((distX * distX) + (distY * distY) + (distZ * distZ));
}

float GetDistanceXY3D(Vec3 const& posA, Vec3 const& posB)
{
	float distX = posB.x - posA.x;
	float distY = posB.y - posA.y;
	return sqrtf((distX * distX) + (distY * distY));
}

float GetDistanceSquared3D(Vec3 const& posA, Vec3 const& posB)
{
	float distX = posB.x - posA.x;
	float distY = posB.y - posA.y;
	float distZ = posB.z - posA.z;
	return (distX * distX) + (distY * distY) + (distZ * distZ);
}

float GetDistanceXYSquared3D(Vec3 const& posA, Vec3 const& posB)
{
	float distX = posB.x - posA.x;
	float distY = posB.y - posA.y;
	return (distX * distX) + (distY * distY);
}

int GetTaxicabDistance2D(IntVec2 const& pointA, IntVec2 const& pointB)
{
	IntVec2 intVectorAB = pointB - pointA;
	return intVectorAB.GetTaxicabLength();
}

float GetProjectedLength2D(Vec2 const& vectorProjecting, Vec2 const& vectorProjectedOn)
{
	Vec2 normalizedProjectedOn = vectorProjectedOn.GetNormalized();
	float projectionLength = DotProduct2D(vectorProjecting, normalizedProjectedOn);
	return projectionLength;
}

Vec2 GetProjectedOnto2D(Vec2 const& vectorProjecting, Vec2 const& vectorProjectedOn)
{
	Vec2 normalizedProjectedOn = vectorProjectedOn.GetNormalized();
	float projectionLength = DotProduct2D(vectorProjecting, normalizedProjectedOn);
	return projectionLength * normalizedProjectedOn;
}

void TransformPosition2D(Vec2& pos, float scale, float rotationDegrees, Vec2 const& translation)
{
	pos *= scale;
	pos.RotateDegrees(rotationDegrees);
	pos += translation;
}

void TransformPosition2D(Vec2& pos, Vec2 const& iBasis, Vec2 const& jBasis, Vec2 const& translation)
{
	pos = (iBasis * pos.x + jBasis * pos.y) + translation;
}

void TransformPositionXY3D(Vec3& pos, float scaleXY, float zRotationDegrees, Vec2 const& translationXY)
{
	pos = pos.GetRotatedAboutZDegrees(zRotationDegrees);
	pos.x *= scaleXY;
	pos.y *= scaleXY;
	pos += Vec3(translationXY.x, translationXY.y, 0.f);
}

void TransformPositionXY3D(Vec3& pos, Vec2 const& iBasis, Vec2 const& jBasis, Vec2 const& translationXY)
{
	Vec2 IJVector = (iBasis * pos.x + jBasis * pos.y) + translationXY;
	pos = Vec3(IJVector.x, IJVector.y, pos.z);
}

void TransformVertexArrayXY3D(int numVerts, Vertex_PCU* pos, float scaleXY, float zRotationDegrees, Vec2 const& translationXY)
{
	for (int i = 0; i < numVerts; i++)
	{
		Vec3& current = pos[i].m_position;
		TransformPositionXY3D(current, scaleXY, zRotationDegrees, translationXY);
	}
}



Vec2 Lerp(Vec2 start, Vec2 end, float fraction)
{
	return start + (end - start) * fraction;
}

float Interpolate(float start, float end, float fraction)
{
	return (start * (1 - fraction)) + (end * fraction);
}

float GetFractionWithinRange(float value, float start, float end)
{
	if (end - start == 0)
	{
		return 0.f;
	}
	return (value - start) / (end - start);
}

float RangeMap(float value, float inStart, float inEnd, float outStart, float outEnd)
{
	float fraction = GetFractionWithinRange(value, inStart, inEnd);
	return Interpolate(outStart, outEnd, fraction);
}

float RangeMapClamped(float value, float inStart, float inEnd, float outStart, float outEnd)
{
	float fraction = GetFractionWithinRange(GetClamped(value, inStart, inEnd), inStart, inEnd);
	return Interpolate(outStart, outEnd, fraction);
}

float GetClamped(float value, float min, float max)
{
	if (value >= max)
		return max;
	if (value <= min)
		return min;
	return value;
}

float GetClampedZeroToOne(float value)
{
	return GetFractionWithinRange(GetClamped(value, 0.f, 1.f), 0.f, 1.f);
}

int RoundDownToInt(float value)
{
	if (value >= 0)
		return (int)value;
	else
		return (int)value - 1;
}

float DotProduct2D(Vec2 const& vecA, Vec2 const& vecB)
{
	return vecA.x * vecB.x + vecA.y * vecB.y;
}

float DotProduct3D(Vec3 const& vecA, Vec3 const& vecB)
{
	return vecA.x * vecB.x + vecA.y * vecB.y + vecA.z * vecB.z;
}

float DotProduct4D(Vec4 const& vecA, Vec4 const& vecB)
{
	return vecA.x * vecB.x + vecA.y * vecB.y + vecA.z * vecB.z + vecA.w * vecB.w;
}

float CrossProduct2D(Vec2 const& firstVec, Vec2 const& secondVec)
{
	return firstVec.x * secondVec.y - firstVec.y * secondVec.x;
}

Vec3 CrossProduct3D(Vec3 const& firstVec, Vec3 const& secondVec)
{
	float x = firstVec.y * secondVec.z - firstVec.z * secondVec.y;
	float y = firstVec.z * secondVec.x - firstVec.x * secondVec.z;
	float z = firstVec.x * secondVec.y - firstVec.y * secondVec.x;
	return Vec3(x,y,z);
}

float NormalizeByte(unsigned char byteValue)
{
	float floatValue = RangeMap(static_cast<float>(byteValue), 0.f, 255.f, 0.f, 256.f);
	floatValue = floatValue / 256.f;
	return floatValue;
}

unsigned char DenormalizeByte(float zeroToOne)
{
	float bucketValue = RangeMap(zeroToOne, 0.f, 1.f, 0.f, 256.f);
	float byteValue = RangeMap(bucketValue, 0.f, 256.f, -.49f, 255.49f);
	byteValue = round(byteValue);
	if (byteValue == 256)
	{
		byteValue = 255;
	}
	return static_cast<unsigned char>(byteValue);
}



bool DoDiscsOverlap2D(Vec2 const& center1, float const& rad1, Vec2 const& center2, float const& rad2)
{
	return GetDistanceSquared2D(center1, center2) <= (rad2 + rad1) * (rad2 + rad1);
}

bool DoSpheresOverlap3D(Vec3 const& center1, float const& rad1, Vec3 const& center2, float const& rad2)
{
	return GetDistanceSquared3D(center1, center2) <= (rad2 + rad1) * (rad2 + rad1);
}

bool DoAABBsOverlap3D(AABB3 box1, AABB3 box2)
{
	FloatRange b1X = FloatRange(box1.m_mins.x, box1.m_maxs.x);
	FloatRange b1Y = FloatRange(box1.m_mins.y, box1.m_maxs.y);
	FloatRange b1Z = FloatRange(box1.m_mins.z, box1.m_maxs.z);
	FloatRange b2X = FloatRange(box2.m_mins.x, box2.m_maxs.x);
	FloatRange b2Y = FloatRange(box2.m_mins.y, box2.m_maxs.y);
	FloatRange b2Z = FloatRange(box2.m_mins.z, box2.m_maxs.z);
	if (b1X.IsOverlappingWith(b2X) && b1Y.IsOverlappingWith(b2Y) && b1Z.IsOverlappingWith(b2Z))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool DoZCylindersOverlap3D(Vec3 center1, float radius1, FloatRange zRange1, Vec3 center2, float radius2, FloatRange zRange2)
{
	if (DoDiscsOverlap2D(Vec2(center1.x, center1.y), radius1, Vec2(center2.x, center2.y), radius2) && zRange1.IsOverlappingWith(zRange2))
	{
		return true;
	}
	return false;
}

bool DoesSphereAABBOverlap3D(Vec3 const& center, float const& radius, AABB3 box)
{
	Vec3 closestPointOnBox = GetNearestPointOnAABB3D(box, center);
	if (IsPointInsideSphere3D(center, radius, closestPointOnBox))
	{
		return true;
	}
	return false;
}

bool DoesSphereZCylinderOverlap3D(Vec3 const& centerSphere, float const& radiusSphere, Vec3 centerCylinder, float radiusCylinder, FloatRange zRangeCylinder)
{
	Vec3 closestPointOnCylinder = GetNearestPointOnZCylinder3D(centerCylinder, radiusCylinder, zRangeCylinder, centerSphere);
	if (IsPointInsideSphere3D(centerSphere, radiusSphere, closestPointOnCylinder))
	{
		return true;
	}
	return false;
}

bool DoesSphereOBBOverlap3D(Vec3 const& center, float const& radius, OBB3 const& box)
{
	Vec3 closestPointOnBox = GetNearestPointOnOBB3D(box, center);
	if (IsPointInsideSphere3D(center, radius, closestPointOnBox))
	{
		return true;
	}
	return false;
}

bool DoesAABBZCylinderOverlap3D(AABB3 box, Vec3 centerCylinder, float radiusCylinder, FloatRange zRangeCylinder)
{
	Vec3 closestPointOnBox = GetNearestPointOnAABB3D(box, centerCylinder);
	if (IsPointInsideZCylinder3D(centerCylinder, radiusCylinder, zRangeCylinder, closestPointOnBox))
	{
		return true;
	}
	return false;
}

bool DoesPlaneOverlapSphere3D(Plane3 const& plane, Vec3 const& center, float radius)
{
	Vec3 nearestPoint = plane.GetNearestPoint(center);
	return IsPointInsideSphere3D(center, radius, nearestPoint);
}

bool DoesPlaneOverlapAABB3D(Plane3 const& plane, AABB3 const& box)
{
	Vec3 corner1 = box.m_mins;
	Vec3 corner2 = Vec3(box.m_mins.x, box.m_mins.y, box.m_maxs.z);
	Vec3 corner3 = Vec3(box.m_mins.x, box.m_maxs.y, box.m_maxs.z);
	Vec3 corner4 = Vec3(box.m_mins.x, box.m_maxs.y, box.m_mins.z);
	Vec3 corner5 = Vec3(box.m_maxs.x, box.m_mins.y, box.m_mins.z);
	Vec3 corner6 = Vec3(box.m_maxs.x, box.m_maxs.y, box.m_mins.z);
	Vec3 corner7 = Vec3(box.m_maxs.x, box.m_mins.y, box.m_maxs.z);
	Vec3 corner8 = box.m_maxs;
	if (plane.IsPointInFrontOfPlane(corner1) && plane.IsPointInFrontOfPlane(corner2) &&
		plane.IsPointInFrontOfPlane(corner3) && plane.IsPointInFrontOfPlane(corner4) &&
		plane.IsPointInFrontOfPlane(corner5) && plane.IsPointInFrontOfPlane(corner6) &&
		plane.IsPointInFrontOfPlane(corner7) && plane.IsPointInFrontOfPlane(corner8))
	{
		return false;
	}
	else if (!plane.IsPointInFrontOfPlane(corner1) && !plane.IsPointInFrontOfPlane(corner2) &&
		!plane.IsPointInFrontOfPlane(corner3) && !plane.IsPointInFrontOfPlane(corner4) &&
		!plane.IsPointInFrontOfPlane(corner5) && !plane.IsPointInFrontOfPlane(corner6) &&
		!plane.IsPointInFrontOfPlane(corner7) && !plane.IsPointInFrontOfPlane(corner8))
	{
		return false;
	}
	
	return true;
}

bool DoesPlaneOverlapOBB3D(Plane3 const& plane, OBB3 const& box)
{
	Vec3 halfLength = box.m_iBasis * box.m_halfDims.x;
	Vec3 halfWidth = box.m_jBasis * box.m_halfDims.y;
	Vec3 halfHeight = box.m_kBasis * box.m_halfDims.z;

	Vec3 corner1 = box.m_center - halfLength - halfWidth - halfHeight;
	Vec3 corner2 = box.m_center - halfLength + halfWidth - halfHeight;
	Vec3 corner3 = box.m_center - halfLength + halfWidth + halfHeight;
	Vec3 corner4 = box.m_center - halfLength - halfWidth + halfHeight;
	Vec3 corner5 = box.m_center + halfLength - halfWidth - halfHeight;
	Vec3 corner6 = box.m_center + halfLength + halfWidth - halfHeight;
	Vec3 corner7 = box.m_center + halfLength + halfWidth + halfHeight;
	Vec3 corner8 = box.m_center + halfLength - halfWidth + halfHeight;
	if (plane.IsPointInFrontOfPlane(corner1) && plane.IsPointInFrontOfPlane(corner2) &&
		plane.IsPointInFrontOfPlane(corner3) && plane.IsPointInFrontOfPlane(corner4) &&
		plane.IsPointInFrontOfPlane(corner5) && plane.IsPointInFrontOfPlane(corner6) &&
		plane.IsPointInFrontOfPlane(corner7) && plane.IsPointInFrontOfPlane(corner8))
	{
		return false;
	}
	else if (!plane.IsPointInFrontOfPlane(corner1) && !plane.IsPointInFrontOfPlane(corner2) &&
		!plane.IsPointInFrontOfPlane(corner3) && !plane.IsPointInFrontOfPlane(corner4) &&
		!plane.IsPointInFrontOfPlane(corner5) && !plane.IsPointInFrontOfPlane(corner6) &&
		!plane.IsPointInFrontOfPlane(corner7) && !plane.IsPointInFrontOfPlane(corner8))
	{
		return false;
	}

	return true;
}

bool IsPointInsideSphere3D(Vec3 const& center, float const& radius, Vec3 const& refPoint)
{
	Vec3 centerToRef = refPoint - center;
	return centerToRef.GetLengthSquared() < radius * radius;
}

bool IsPointInsideAABB3D(AABB3 box, Vec3 const& refPoint)
{
	Vec3 mins = box.m_mins;
	Vec3 maxs = box.m_maxs;
	if ((mins.x < refPoint.x && maxs.x > refPoint.x) &&
		(mins.y < refPoint.y && maxs.y > refPoint.y) &&
		(mins.z < refPoint.z && maxs.z > refPoint.z))
	{
		return true;
	}
	return false;
}

bool IsPointInsideOBB3D(OBB3 const& box, Vec3 const& refPoint)
{
	Vec3 iBasis = box.m_iBasis;
	Vec3 jBasis = box.m_jBasis;
	Vec3 kBasis = box.m_kBasis;

	Vec3 relativeLocalRef = refPoint - box.m_center;

	float posi = DotProduct3D(relativeLocalRef, iBasis);
	float posj = DotProduct3D(relativeLocalRef, jBasis);
	float posk = DotProduct3D(relativeLocalRef, kBasis);

	if ((-box.m_halfDims.x < posi && box.m_halfDims.x > posi) &&
		(-box.m_halfDims.y < posj && box.m_halfDims.y > posj) &&
		(-box.m_halfDims.z < posk && box.m_halfDims.z > posk))
	{
		return true;
	}
	return false;
}

bool IsPointInsideZCylinder3D(Vec3 centerCylinder, float radiusCylinder, FloatRange zRangeCylinder, Vec3 const& refPoint)
{
	if (IsPointInsideDisc2D(Vec2(refPoint.x, refPoint.y), Vec2(centerCylinder.x, centerCylinder.y), radiusCylinder) && zRangeCylinder.IsOnRange(refPoint.z))
	{
		return true;
	}
	return false;
}

Vec3 GetNearestPointOnSphere3D(Vec3 const& center, float const& radius, Vec3 const& refPoint)
{
	Vec3 centerToRef = refPoint - center;

	if (IsPointInsideSphere3D(center,radius,refPoint))
	{
		return refPoint;
	}
	centerToRef.SetLength(radius);
	return center + centerToRef;
}

Vec3 GetNearestPointOnAABB3D(AABB3 box, Vec3 const& refPoint)
{
	Vec3 temp = Vec3();
	temp.x = GetClamped(refPoint.x, box.m_mins.x, box.m_maxs.x);
	temp.y = GetClamped(refPoint.y, box.m_mins.y, box.m_maxs.y);
	temp.z = GetClamped(refPoint.z, box.m_mins.z, box.m_maxs.z);
	return temp;
}

Vec3 GetNearestPointOnZCylinder3D(Vec3 centerCylinder, float radiusCylinder, FloatRange zRangeCylinder, Vec3 const& refPoint)
{
	if (IsPointInsideZCylinder3D(centerCylinder, radiusCylinder, zRangeCylinder, refPoint))
	{
		return refPoint;
	}

	Vec2 nearestDiscPoint = GetNearestPointOnDisc2D(Vec2(refPoint.x, refPoint.y), Vec2(centerCylinder.x, centerCylinder.y), radiusCylinder);
	float nearestZ = GetClamped(refPoint.z, zRangeCylinder.m_min, zRangeCylinder.m_max);
	return Vec3(nearestDiscPoint.x, nearestDiscPoint.y, nearestZ);
}

Vec3 GetNearestPointOnOBB3D(OBB3 const& box, Vec3 const& refPoint)
{
	if (IsPointInsideOBB3D(box, refPoint))
	{
		return refPoint;
	}

	Vec3 iBasis = box.m_iBasis;
	Vec3 jBasis = box.m_jBasis;
	Vec3 kBasis = box.m_kBasis;

	Vec3 relativeLocalRef = refPoint - box.m_center;

	float posi = DotProduct3D(relativeLocalRef, iBasis);
	float posj = DotProduct3D(relativeLocalRef, jBasis);
	float posk = DotProduct3D(relativeLocalRef, kBasis);

	float nearesti = GetClamped(posi, -box.m_halfDims.x, box.m_halfDims.x);
	float nearestj = GetClamped(posj, -box.m_halfDims.y, box.m_halfDims.y);
	float nearestk = GetClamped(posk, -box.m_halfDims.z, box.m_halfDims.z);

	return box.m_center + (nearesti * box.m_iBasis) + (nearestj * box.m_jBasis) + (nearestk * box.m_kBasis);
}

bool IsPointInsideVisionCone(const Vec3& origin, const EulerAngles& forwardOrientation, float coneAngleDegrees, const Vec3& pointToTest)
{
	if (origin == pointToTest)
	{
		return true;
	}
	Vec3 forward = forwardOrientation.GetForwardNormal();
	Vec3 toPoint = (pointToTest - origin).GetNormalized();
	float dot = DotProduct3D(forward, toPoint); // Cosine of angle between
	float maxAngleCos = cosf(ConvertDegreesToRadians(coneAngleDegrees * 0.5f));

	return dot >= maxAngleCos;
}

float ComputeCubicBezier1D(float A, float B, float C, float D, float t)
{
	float p1 = Interpolate(A, B, t);
	float p2 = Interpolate(B, C, t);
	float p3 = Interpolate(C, D, t);

	float pPrime1 = Interpolate(p1, p2, t);
	float pPrime2 = Interpolate(p2, p3, t);
	return Interpolate(pPrime1, pPrime2, t);
}

float ComputeQuinticBezier1D(float A, float B, float C, float D, float E, float F, float t)
{
	float p1 = Interpolate(A, B, t);
	float p2 = Interpolate(B, C, t);
	float p3 = Interpolate(C, D, t);
	float p4 = Interpolate(D, E, t);
	float p5 = Interpolate(E, F, t);
	float pPrime1 = Interpolate(p1, p2, t);
	float pPrime2 = Interpolate(p2, p3, t);
	float pPrime3 = Interpolate(p3, p4, t);
	float pPrime4 = Interpolate(p4, p5, t);

	return ComputeCubicBezier1D(pPrime1, pPrime2, pPrime3, pPrime4, t);
}

float Linear(float t)
{
	return t;
}

float SmoothStart2(float t)
{
	return t * t;
}

float SmoothStart3(float t)
{
	return t * t * t;
}

float SmoothStart4(float t)
{
	return t * t * t * t;
}

float SmoothStart5(float t)
{
	return t * t * t * t * t;
}

float SmoothStart6(float t)
{
	return t * t * t * t * t * t;
}

float SmoothStop2(float t)
{
	float inverseT = (1.f - t);
	return 1.f - inverseT * inverseT;
}

float SmoothStop3(float t)
{
	float inverseT = (1.f - t);
	return 1.f - inverseT * inverseT * inverseT;
}

float SmoothStop4(float t)
{
	float inverseT = (1.f - t);
	return 1.f - inverseT * inverseT * inverseT * inverseT;
}

float SmoothStop5(float t)
{
	float inverseT = (1.f - t);
	return 1.f - inverseT * inverseT * inverseT * inverseT * inverseT;
}

float SmoothStop6(float t)
{
	float inverseT = (1.f - t);
	return 1.f - inverseT * inverseT * inverseT * inverseT * inverseT * inverseT;
}

float SmoothStep3(float t)
{
	return (3 * (t * t)) - (2 * (t * t * t));
}

float SmoothStep5(float t)
{
	return (6 * (t * t * t * t * t)) - (15 * (t * t * t * t)) + (10 * (t * t * t));
}

float Hesitate3(float t)
{
	return ComputeCubicBezier1D(0, 1, 0, 1, t);
}

float Hesitate5(float t)
{
	return ComputeQuinticBezier1D(0, 1, 0, 1, 0, 1, t);
}

float CustomFunky(float t)
{
	float x = RangeMap(t, 0.f, 1.f, 0.f, 90.f);
	float y = 1-abs((1-t) * CosDegrees(x * t * 5.f));
	return y;
}

Vec2 GetNearestPointOnDisc2D(Vec2 const& pointOfReference, Vec2 const& center, float radius)
{
	Vec2 centerToPoint = pointOfReference - center;
	return center + centerToPoint.GetClamped(radius);
}

Vec2 GetNearestPointOnAABB2D(Vec2 const& pointOfReference, AABB2 const& aabb)
{
	if (IsPointInsideAABB2D(pointOfReference, aabb))
	{
		return pointOfReference;
	}
	return aabb.GetNearestPoint(pointOfReference);
}

Vec2 GetNearestPointOnOBB2D(Vec2 const& pointOfReference, OBB2 const& obb)
{
	Vec2 centerToPoint = pointOfReference - obb.m_center;
	Vec2 localSpaceCenterToPoint = obb.GetLocalPosFromWorldPos(centerToPoint);
	Vec2 nearestPointIJ;
	nearestPointIJ.x = GetClamped(localSpaceCenterToPoint.x, -obb.m_halfDimensions.x, obb.m_halfDimensions.x);
	nearestPointIJ.y = GetClamped(localSpaceCenterToPoint.y, -obb.m_halfDimensions.y, obb.m_halfDimensions.y);
	return obb.GetWorldPosFromLocalPos(nearestPointIJ);
}

Vec2 GetNearestPointOnCapsule2D(Vec2 const& pointOfReference, Capsule2 const& capsule)
{
	Vec2 nearestPointOnBone = GetNearestPointOnLineSegment2D(pointOfReference, capsule.m_bone);
	Vec2 lineToPoint = pointOfReference - nearestPointOnBone;
	return nearestPointOnBone + lineToPoint.GetClamped(capsule.m_radius);
}

Vec2 GetNearestPointOnTriangle2D(Vec2 const& pointOfReference, Triangle2 triangle)
{
	if (IsPointInsideTriangle2D(pointOfReference,triangle))
	{
		return pointOfReference;
	}
	else
	{
		LineSegment2 AB = LineSegment2(triangle.m_pointsCounterClockwise[0], triangle.m_pointsCounterClockwise[1]);
		LineSegment2 BC = LineSegment2(triangle.m_pointsCounterClockwise[1], triangle.m_pointsCounterClockwise[2]);
		LineSegment2 CA = LineSegment2(triangle.m_pointsCounterClockwise[2], triangle.m_pointsCounterClockwise[0]);
		Vec2 deltaAB = pointOfReference - GetNearestPointOnLineSegment2D(pointOfReference, AB);
		Vec2 deltaBC = pointOfReference - GetNearestPointOnLineSegment2D(pointOfReference, BC);
		Vec2 deltaCA = pointOfReference - GetNearestPointOnLineSegment2D(pointOfReference, CA);

		if (deltaAB.GetLengthSquared() <= deltaBC.GetLengthSquared() && deltaAB.GetLengthSquared() <= deltaCA.GetLengthSquared())
		{
			return GetNearestPointOnLineSegment2D(pointOfReference, AB);
		}
		else if (deltaBC.GetLengthSquared() <= deltaAB.GetLengthSquared() && deltaBC.GetLengthSquared() <= deltaCA.GetLengthSquared())
		{
			return GetNearestPointOnLineSegment2D(pointOfReference, BC);
		}
		else
		{
			return GetNearestPointOnLineSegment2D(pointOfReference, CA);
		}
	}
}

bool PushDiscOutOfPoint2D(Vec2& centerDynamic, float radiusDynamic, Vec2 const& point)
{
	Vec2 centerToPoint = point - centerDynamic;
	if (radiusDynamic * radiusDynamic <= centerToPoint.GetLengthSquared())
	{
		return false;
	}
	else
	{
		float overlapLength = radiusDynamic - centerToPoint.GetLength();
		centerToPoint.SetLength(overlapLength);
		centerDynamic = centerDynamic - centerToPoint;
		return true;
	}
}

bool PushDiscOutOfDisc2D(Vec2& centerDynamic, float radiusDynamic, Vec2 const& centerStatic, float radiusStatic)
{
	Vec2 centerToStaticCenter = centerStatic - centerDynamic;
	float combinedRadius = radiusStatic + radiusDynamic;
	if (combinedRadius * combinedRadius <= centerToStaticCenter.GetLengthSquared())
	{
		return false;
	}
	else
	{
		float overlapLength = combinedRadius - centerToStaticCenter.GetLength();
		centerToStaticCenter.SetLength(overlapLength);
		centerDynamic = centerDynamic - centerToStaticCenter;
		return true;
	}
}

bool PushDiscsOutOfEachOther2D(Vec2& centerA, float radiusA, Vec2& centerB, float radiusB)
{
	Vec2 displacement = centerB - centerA;
	float distSquared = displacement.GetLengthSquared();
	float combinedRadius = radiusA + radiusB;

	if (distSquared >= combinedRadius * combinedRadius)
	{
		return false; // No overlap
	}

	if (distSquared <= 0.0000001f)
	{
		Vec2 pushDir = Vec2(1.0f, 0.0f);
		Vec2 push = pushDir * (combinedRadius * 0.5f);
		centerA -= push;
		centerB += push;
		return true;
	}

	float distance = sqrtf(distSquared);
	float overlap = combinedRadius - distance;

	Vec2 correction = displacement.GetNormalized() * (overlap * 0.5f);
	centerA -= correction;
	centerB += correction;

	return true;
}

bool PushDiscOutOfAABB2D(Vec2& centerDynamic, float radiusDynamic, AABB2 const& boxStatic)
{
	Vec2 nearestPointOnBox = boxStatic.GetNearestPoint(centerDynamic);
	Vec2 centerToNearest = nearestPointOnBox - centerDynamic;
	if (radiusDynamic * radiusDynamic <= centerToNearest.GetLengthSquared())
	{
		return false;
	}
	else
	{
		float overlapLength = radiusDynamic - centerToNearest.GetLength();
		centerToNearest.SetLength(overlapLength);
		centerDynamic = centerDynamic - centerToNearest;
		return true;
	}
}

bool IsPointInsideDisc2D(Vec2 const& point, Vec2 const& center, float radius)
{
	Vec2 centerToPoint = point - center;
	return radius * radius >= centerToPoint.GetLengthSquared();
}

bool IsPointInsideAABB2D(Vec2 const& point, AABB2 const& aabb)
{
	if (aabb.m_mins.x <= point.x && point.x <= aabb.m_maxs.x)
	{
		return aabb.m_mins.y <= point.y && point.y <= aabb.m_maxs.y;
	}
	return false;
}

bool IsPointInsideCapsule2D(Vec2 const& point, Capsule2 const& capsule)
{
	Vec2 nearestPointOnBoneToPoint = point - GetNearestPointOnLineSegment2D(point, capsule.m_bone);
	return nearestPointOnBoneToPoint.GetLengthSquared() <= capsule.m_radius * capsule.m_radius;
}

bool IsPointInsideOBB2D(Vec2 const& point, OBB2 const& obb)
{
	Vec2 centerToPoint = point - obb.m_center;
	Vec2 localSpaceCenterToPoint = obb.GetLocalPosFromWorldPos(centerToPoint);
	if (localSpaceCenterToPoint.x > obb.m_halfDimensions.x)
	{
		return false;
	}
	if (localSpaceCenterToPoint.x < -obb.m_halfDimensions.x)
	{
		return false;
	}
	if (localSpaceCenterToPoint.y > obb.m_halfDimensions.y)
	{
		return false;
	}
	if (localSpaceCenterToPoint.y < -obb.m_halfDimensions.y)
	{
		return false;
	}
	return true;
}

bool IsPointInsideOrientedSector2D(Vec2 const& point, Vec2 const& sectorTip, float sectorForwardDegrees, float sectorApertureDegrees, float sectorRadius)
{
	Vec2 centerToPoint = point - sectorTip;
	if (centerToPoint.GetLength() <= sectorRadius)
	{
		float degreesToApertureEndFromNormal = sectorApertureDegrees/2;
		float degreesToPointFromNormal = GetShortestAngularDispDegrees(sectorForwardDegrees, centerToPoint.GetOrientationDegrees());
		if (abs(degreesToPointFromNormal) <= degreesToApertureEndFromNormal)
		{
			return true;
		}
	}
	return false;
}

bool IsPointInsideDirectedSector2D(Vec2 const& point, Vec2 const& sectorTip, Vec2 const& sectorForwardNormal, float sectorApertureDegrees, float sectorRadius)
{
	Vec2 centerToPoint = point - sectorTip;
	if (centerToPoint.GetLength() <= sectorRadius)
	{
		float degreesToApertureEndFromNormal = sectorApertureDegrees / 2;
		float degreesToPointFromNormal = GetShortestAngularDispDegrees(sectorForwardNormal.GetOrientationDegrees(), centerToPoint.GetOrientationDegrees());
		if (abs(degreesToPointFromNormal) <= degreesToApertureEndFromNormal)
		{
			return true;
		}
	}
	return false;
}

bool IsPointInsideTriangle2D(Vec2 const& point, Triangle2 const& triangle)
{
	Vec2 AToB = triangle.m_pointsCounterClockwise[1] - triangle.m_pointsCounterClockwise[0];
	Vec2 BToC = triangle.m_pointsCounterClockwise[2] - triangle.m_pointsCounterClockwise[1];
	Vec2 CToA = triangle.m_pointsCounterClockwise[0] - triangle.m_pointsCounterClockwise[2];
	Vec2 AToPoint = point - triangle.m_pointsCounterClockwise[0];
	Vec2 BToPoint = point - triangle.m_pointsCounterClockwise[1];
	Vec2 CToPoint = point - triangle.m_pointsCounterClockwise[2];
	if (DotProduct2D(AToB.GetRotated90Degrees(), AToPoint) >= 0 && DotProduct2D(BToC.GetRotated90Degrees(), BToPoint) >= 0 && DotProduct2D(CToA.GetRotated90Degrees(), CToPoint) >= 0)
	{
		return true;
	}
	return false;
}

Vec2 GetNearestPointOnLineSegment2D(Vec2 const& pointOfReference, LineSegment2 const& line)
{
	Vec2 startToPoint = pointOfReference - line.m_start;
	Vec2 endToPoint = pointOfReference - line.m_end;
	Vec2 startToEnd = line.m_end - line.m_start;

	if (DotProduct2D(startToPoint, startToEnd) <= 0)
	{
		return line.m_start;
	}
	else if (DotProduct2D(endToPoint, startToEnd) >= 0)
	{
		return line.m_end;
	}
	else
	{
		float t = DotProduct2D(startToPoint, startToEnd) / startToEnd.GetLengthSquared();
		return line.m_start + t * startToEnd;
	}
}

Vec2 GetNearestPointOnInfiniteLine2D(Vec2 const& pointOfReference, LineSegment2 const& infiniteLine)
{
	Vec2 startToPoint = pointOfReference - infiniteLine.m_start;
	Vec2 startToEnd = infiniteLine.m_end - infiniteLine.m_start;
	return GetProjectedOnto2D(startToPoint, startToEnd);
}
