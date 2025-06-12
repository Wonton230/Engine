#include <cmath>
#include <vector>
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/AABB3.hpp"
#include "Engine/Math/FloatRange.hpp"
#pragma once

struct Vec2;
struct IntVec2;
struct Vec3;
struct Vec4;
struct Vertex_PCU;
struct AABB2;
struct Capsule2;
struct OBB2;
struct LineSegment2;
struct Triangle2;
struct Mat44;
struct EulerAngles;
struct OBB3;
struct Plane3;

enum class BillBoardType
{
	NONE = -1,
	WORLD_UP_FACING,
	WORLD_UP_OPPOSING,
	FULL_FACING,
	FULL_OPPOSING,
	COUNT
};

Mat44 GetBillboardTransform(
	BillBoardType billboardType,
	Mat44 const& targetTransform,
	const Vec3& billboardPosition,
	const Vec2& billboardSize = Vec2(1.0f, 1.0f));

//Angle Functions
float			ConvertDegreesToRadians(float degrees);
float			ConvertRadiansToDegrees(float radians);
float			CosDegrees(float degrees);
float			SinDegrees(float degrees);
float			Atan2Degrees(float y, float x);
float			GetShortestAngularDispDegrees(float startDeg, float endDeg);
float			GetTurnedTowardDegrees(float currentDeg, float targetDeg, float maxDeltaDeg);
float			GetAngleDegreesBetweenVectors2D(Vec2 const& vectorA, Vec2 const& vectorB);

//Distance Functions
float			GetDistance2D(Vec2 const& posA, Vec2 const& posB);
float			GetDistanceSquared2D(Vec2 const& posA, Vec2 const& posB);
float			GetDistance3D(Vec3 const& posA, Vec3 const& posB);
float			GetDistanceXY3D(Vec3 const& posA, Vec3 const& posB);
float			GetDistanceSquared3D(Vec3 const& posA, Vec3 const& posB);
float			GetDistanceXYSquared3D(Vec3 const& posA, Vec3 const& posB);
int				GetTaxicabDistance2D(IntVec2 const& pointA, IntVec2 const& pointB);
float			GetProjectedLength2D(Vec2 const& vectorProjecting, Vec2 const& vectorProjectedOn);
Vec2			GetProjectedOnto2D(Vec2 const& vectorProjecting, Vec2 const& vectorProjectedOn);

//Transform Functions
void			TransformPosition2D(Vec2& pos, float scale, float rotationDegrees, Vec2 const& translation);
void			TransformPosition2D(Vec2& pos, Vec2 const& iBasis, Vec2 const& jBasis, Vec2 const& translation);
void			TransformPositionXY3D(Vec3& pos, float scaleXY, float zRotationDegrees, Vec2 const& translationXY);
void			TransformPositionXY3D(Vec3& pos, Vec2 const& iBasis, Vec2 const& jBasis, Vec2 const& translationXY);
void			TransformVertexArrayXY3D(int numVerts, Vertex_PCU* pos, float scaleXY, float zRotationDegrees, Vec2 const& translationXY);

//Lerps and Ranges
Vec2			Lerp(Vec2 start, Vec2 end, float fraction);
float			Interpolate(float start, float end, float fraction);
float			GetFractionWithinRange(float value, float start, float end);
float			RangeMap(float value, float inStart, float inEnd, float outStart, float outEnd);
float			RangeMapClamped(float value, float inStart, float inEnd, float outStart, float outEnd);
float			GetClamped(float value, float min, float max);
float			GetClampedZeroToOne(float value);
int				RoundDownToInt(float value);
float			DotProduct2D(Vec2 const& vecA, Vec2 const& vecB);
float			DotProduct3D(Vec3 const& vecA, Vec3 const& vecB);
float			DotProduct4D(Vec4 const& vecA, Vec4 const& vecB);
float			CrossProduct2D(Vec2 const& firstVec, Vec2 const& secondVec);
Vec3			CrossProduct3D(Vec3 const& firstVec, Vec3 const& secondVec);
float			NormalizeByte(unsigned char byteValue);
unsigned char	DenormalizeByte(float zeroToOne);

//Geometric Queries 2D
bool			DoDiscsOverlap2D(Vec2 const& center1, float const& rad1, Vec2 const& center2, float const& rad2);
bool			PushDiscOutOfPoint2D(Vec2& centerDynamic, float radiusDynamic, Vec2 const& point);
bool			PushDiscOutOfDisc2D(Vec2& centerDynamic, float radiusDynamic, Vec2 const& centerStatic, float radiusStatic);
bool			PushDiscsOutOfEachOther2D(Vec2& centerDynamicA, float radiusDynamicA, Vec2& centerDynamicB, float radiusDynamicB);
bool			PushDiscOutOfAABB2D(Vec2& centerDynamic, float radiusDynamic, AABB2 const& boxStatic);
bool			IsPointInsideDisc2D(Vec2 const& point, Vec2 const& center, float radius);
bool			IsPointInsideAABB2D(Vec2 const& point, AABB2 const& aabb);
bool			IsPointInsideCapsule2D(Vec2 const& point, Capsule2 const& capsule);
bool			IsPointInsideOBB2D(Vec2 const& point, OBB2 const& obb);
bool			IsPointInsideOrientedSector2D(Vec2 const& point, Vec2 const& sectorTip, float sectorForwardDegrees, float sectorApertureDegrees, float sectorRadius);
bool			IsPointInsideDirectedSector2D(Vec2 const& point, Vec2 const& sectorTip, Vec2 const& sectorForwardNormal, float sectorApertureDegrees, float sectorRadius);
bool			IsPointInsideTriangle2D(Vec2 const& point, Triangle2 const& triangle);
Vec2			GetNearestPointOnLineSegment2D(Vec2 const& pointOfReference, LineSegment2 const& line);
Vec2			GetNearestPointOnInfiniteLine2D(Vec2 const& pointOfReference, LineSegment2 const& infiniteLine);
Vec2			GetNearestPointOnDisc2D(Vec2 const& pointOfReference, Vec2 const& center, float radius);
Vec2			GetNearestPointOnAABB2D(Vec2 const& pointOfReference, AABB2 const& aabb);
Vec2			GetNearestPointOnOBB2D(Vec2 const& pointOfReference, OBB2 const& obb);
Vec2			GetNearestPointOnCapsule2D(Vec2 const& pointOfReference, Capsule2 const& capsule);
Vec2			GetNearestPointOnTriangle2D(Vec2 const& pointOfReference, Triangle2 triangle);

//Geometric Queries 3D
bool			DoSpheresOverlap3D(Vec3 const& center1, float const& rad1, Vec3 const& center2, float const& rad2);
bool			DoAABBsOverlap3D(AABB3 box1, AABB3 box2);
bool			DoZCylindersOverlap3D(Vec3 center1, float radius1, FloatRange zRange1, Vec3 center2, float radius2, FloatRange zRange2);
bool			DoesSphereAABBOverlap3D(Vec3 const& center, float const& radius, AABB3 box);
bool			DoesSphereZCylinderOverlap3D(Vec3 const& centerSphere, float const& radiusSphere, Vec3 centerCylinder, float radiusCylinder, FloatRange zRangeCylinder);
bool			DoesSphereOBBOverlap3D(Vec3 const& center, float const& radius, OBB3 const& box);
bool			DoesAABBZCylinderOverlap3D(AABB3 box, Vec3 centerCylinder, float radiusCylinder, FloatRange zRangeCylinder);
bool			DoesPlaneOverlapSphere3D(Plane3 const& plane, Vec3 const& center, float radius);
bool			DoesPlaneOverlapAABB3D(Plane3 const& plane, AABB3 const& box);
bool			DoesPlaneOverlapOBB3D(Plane3 const& plane, OBB3 const& box);
bool			IsPointInsideSphere3D(Vec3 const& center, float const& radius, Vec3 const& refPoint);
bool			IsPointInsideAABB3D(AABB3 box, Vec3 const& refPoint);
bool			IsPointInsideOBB3D(OBB3 const& box, Vec3 const& refPoint);
bool			IsPointInsideZCylinder3D(Vec3 centerCylinder, float radiusCylinder, FloatRange zRangeCylinder, Vec3 const& refPoint);
Vec3			GetNearestPointOnSphere3D(Vec3 const& center, float const& radius, Vec3 const& refPoint);
Vec3			GetNearestPointOnAABB3D(AABB3 box, Vec3 const& refPoint);
Vec3			GetNearestPointOnZCylinder3D(Vec3 centerCylinder, float radiusCylinder, FloatRange zRangeCylinder, Vec3 const& refPoint);
Vec3			GetNearestPointOnOBB3D(OBB3 const& box, Vec3 const& refPoint);
bool			IsPointInsideVisionCone(const Vec3& origin, const EulerAngles& forwardOrientation, float coneAngleDegrees, const Vec3& pointToTest);

//Curves
float ComputeCubicBezier1D(float A, float B, float C, float D, float t);
float ComputeQuinticBezier1D(float A, float B, float C, float D, float E, float F, float t);

//Easing Functions
float Linear(float t);
float SmoothStart2(float t);
float SmoothStart3(float t);
float SmoothStart4(float t);
float SmoothStart5(float t);
float SmoothStart6(float t);

float SmoothStop2(float t);
float SmoothStop3(float t);
float SmoothStop4(float t);
float SmoothStop5(float t);
float SmoothStop6(float t);

float SmoothStep3(float t);
float SmoothStep5(float t);

float Hesitate3(float t);
float Hesitate5(float t);
float CustomFunky(float t);