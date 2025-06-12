#include "RaycastUtils.hpp"
#include "MathUtils.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/LineSegment2.hpp"
#include "Engine/Math/Plane3.hpp"
#include "Engine/Math/OBB3.hpp"

RaycastResult2D RaycastVsDisc2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, Vec2 discCenter, float discRadius)
{
	RaycastResult2D result = RaycastResult2D();
	result.m_rayFwdNormal = fwdNormal;
	result.m_rayMaxLength = maxDist;
	result.m_rayStartPos = startPos;

	float startToCenterj = DotProduct2D((discCenter - startPos), fwdNormal.GetRotated90Degrees());
	if (startToCenterj >= discRadius || startToCenterj <= -discRadius)
	{
		result.m_didImpact = false;
		result.m_impactDist = maxDist;
		result.m_impactPos = startPos + fwdNormal * maxDist;
		result.m_impactNormal = fwdNormal;
		return result;
	}

	float startToCenteri = DotProduct2D((discCenter - startPos), fwdNormal);
	if (startToCenteri >= maxDist + discRadius || startToCenteri <= -discRadius)
	{
		result.m_didImpact = false;
		result.m_impactDist = maxDist;
		result.m_impactPos = startPos + fwdNormal * maxDist;
		result.m_impactNormal = fwdNormal;
		return result;
	}

	if (IsPointInsideDisc2D(startPos, discCenter, discRadius))
	{
		result.m_didImpact = true;
		result.m_impactDist = 0.f;
		result.m_impactPos = startPos;
		result.m_impactNormal = -fwdNormal;
		return result;
	}

	float centerToImpactAdjustment = sqrtf(discRadius * discRadius - startToCenterj * startToCenterj);
	if (startToCenteri - centerToImpactAdjustment > maxDist)
	{
		result.m_didImpact = false;
		result.m_impactDist = maxDist;
		result.m_impactPos = startPos + fwdNormal * maxDist;
		result.m_impactNormal = fwdNormal;
		return result;
	}
	
	result.m_didImpact = true;
	result.m_impactDist = startToCenteri - centerToImpactAdjustment;
	result.m_impactPos = startPos + fwdNormal * result.m_impactDist;
	result.m_impactNormal = (result.m_impactPos - discCenter).GetNormalized();

	return result;
}

RaycastResult2D RaycastVsAABB2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, AABB2 boundingBox)
{
	RaycastResult2D result;
	result.m_rayFwdNormal = fwdNormal;
	result.m_rayMaxLength = maxDist;
	result.m_rayStartPos = startPos;
	if (IsPointInsideAABB2D(startPos, boundingBox))
	{
		result.m_didImpact = true;
		result.m_impactDist = 0.f;
		result.m_impactPos = startPos;
		result.m_impactNormal = -fwdNormal;
		return result;
	}

	Vec2 rayMaxEnd = startPos + (fwdNormal * maxDist);

	float tXMin = (boundingBox.m_mins.x - startPos.x) / fwdNormal.x;
	float tXMax = (boundingBox.m_maxs.x - startPos.x) / fwdNormal.x;
	float tYMin = (boundingBox.m_mins.y - startPos.y) / fwdNormal.y;
	float tYMax = (boundingBox.m_maxs.y - startPos.y) / fwdNormal.y;

	if (tXMin > tXMax) std::swap(tXMin, tXMax);
	if (tYMin > tYMax) std::swap(tYMin, tYMax);

	float tEnter = std::max(tXMin, tYMin);
	float tExit = std::min(tXMax, tYMax);

	if (tEnter > tExit || tExit < 0.f || tEnter > maxDist)
	{
		result.m_didImpact = false;
		result.m_impactDist = maxDist;
		result.m_impactPos = rayMaxEnd;
		result.m_impactNormal = fwdNormal;
		return result;
	}

	result.m_didImpact = true;
	result.m_impactDist = tEnter;
	result.m_impactPos = startPos + fwdNormal * result.m_impactDist;

	if (tEnter == tXMin)
		result.m_impactNormal = Vec2(-1.f * (fwdNormal.x > 0 ? 1 : -1), 0.f);
	else if (tEnter == tXMax)
		result.m_impactNormal = Vec2(1.f * (fwdNormal.x > 0 ? 1 : -1), 0.f);
	else if (tEnter == tYMin)
		result.m_impactNormal = Vec2(0.f, -1.f * (fwdNormal.y > 0 ? 1 : -1));
	else if (tEnter == tYMax)
		result.m_impactNormal = Vec2(0.f, 1.f * (fwdNormal.y > 0 ? 1 : -1));

	return result;
}

RaycastResult2D RaycastVsLineSegement2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, LineSegment2 line)
{
	RaycastResult2D result = RaycastResult2D();
	result.m_rayFwdNormal = fwdNormal;
	result.m_rayMaxLength = maxDist;
	result.m_rayStartPos = startPos;

	Vec2 jBasis = fwdNormal.GetRotated90Degrees();
	Vec2 RayToStart = line.m_start - startPos;
	Vec2 RayToEnd = line.m_end - startPos;

	float RayToStartj = DotProduct2D(jBasis, RayToStart);
	float RayToEndj = DotProduct2D(jBasis, RayToEnd);
	if (RayToEndj * RayToStartj >= 0)
	{
		result.m_didImpact = false;
		result.m_impactDist = maxDist;
		result.m_impactPos = startPos + fwdNormal * maxDist;
		result.m_impactNormal = fwdNormal;
		return result;
	}

	float RayToStarti = DotProduct2D(RayToStart,fwdNormal);
	float RayToEndi = DotProduct2D(RayToEnd, fwdNormal);
	if (RayToStarti >= maxDist && RayToEndi >= maxDist)
	{
		result.m_didImpact = false;
		result.m_impactDist = maxDist;
		result.m_impactPos = startPos + fwdNormal * maxDist;
		result.m_impactNormal = fwdNormal;
		return result;
	}
	if (RayToStarti <= 0 && RayToEndi <= 0)
	{
		result.m_didImpact = false;
		result.m_impactDist = maxDist;
		result.m_impactPos = startPos + fwdNormal * maxDist;
		result.m_impactNormal = fwdNormal;
		return result;
	}

	float tFraction;
	float denominator = abs(RayToStartj) + abs(RayToEndj);
	if (denominator > 0)
	{
		tFraction = abs(RayToStartj) / denominator;
	}
	else
	{
		result.m_didImpact = false;
		result.m_impactDist = maxDist;
		result.m_impactPos = startPos + fwdNormal * maxDist;
		result.m_impactNormal = fwdNormal;
		return result;
	}

	result.m_impactDist = RayToStarti + (tFraction * (RayToEndi - RayToStarti));

	if (result.m_impactDist > maxDist || result.m_impactDist < 0)
	{
		result.m_didImpact = false;
		result.m_impactDist = maxDist;
		result.m_impactPos = startPos + fwdNormal * maxDist;
		result.m_impactNormal = fwdNormal;
		return result;
	}

	result.m_impactPos = startPos + (fwdNormal * result.m_impactDist);
	result.m_didImpact = true;
	result.m_impactNormal = ((line.m_end - line.m_start).GetNormalized()).GetRotated90Degrees();
	if (DotProduct2D(fwdNormal, result.m_impactNormal) > 0)
	{
		result.m_impactNormal = -result.m_impactNormal;
	}
	return result;
}

RaycastResult3D RaycastVsSphere3D(Vec3 startPos, Vec3 fwdNormal, float maxDist, Vec3 sphereCenter, float sphereRadius)
{
	RaycastResult3D result;
	result.m_rayFwdNormal = fwdNormal;
	result.m_rayStartPos = startPos;
	result.m_rayMaxLength = maxDist;

	if (IsPointInsideSphere3D(sphereCenter, sphereRadius, startPos))
	{
		result.m_impactDist = 0.f;
		result.m_impactPos = startPos;
		result.m_didImpact = true;
		result.m_impactNormal = -fwdNormal;
		return result;
	}

	Vec3 rayStartToCenter = sphereCenter - startPos;
	float startToCenterI = DotProduct3D(rayStartToCenter, fwdNormal);
	if (startToCenterI >= sphereRadius + maxDist || startToCenterI < -sphereRadius)
	{
		result.m_didImpact = false;
		result.m_impactDist = maxDist;
		result.m_impactPos = startPos + fwdNormal * maxDist;
		result.m_impactNormal = fwdNormal;
		return result;
	}

	Vec3 rayEndToCenter = sphereCenter - (startPos + (fwdNormal * startToCenterI));
	float startToCenterJ = rayEndToCenter.GetLength();
	if (startToCenterJ >= sphereRadius)
	{
		result.m_didImpact = false;
		result.m_impactDist = maxDist;
		result.m_impactPos = startPos + fwdNormal * maxDist;
		result.m_impactNormal = fwdNormal;
		return result;
	}

	float centerToImpactAdjustment = sqrtf(sphereRadius * sphereRadius - startToCenterJ * startToCenterJ);
	if (maxDist < startToCenterI - centerToImpactAdjustment)
	{
		result.m_didImpact = false;
		result.m_impactDist = maxDist;
		result.m_impactPos = startPos + fwdNormal * maxDist;
		result.m_impactNormal = fwdNormal;
		return result;
	}

	result.m_impactDist = startToCenterI - centerToImpactAdjustment;
	result.m_impactPos = startPos + (fwdNormal * result.m_impactDist);
	result.m_didImpact = true;
	result.m_impactNormal = (result.m_impactPos - sphereCenter).GetNormalized();
	return result;
}

RaycastResult3D RaycastVsAABB3D(Vec3 startPos, Vec3 fwdNormal, float maxDist, AABB3 box)
{
	RaycastResult3D result;
	result.m_rayFwdNormal = fwdNormal;
	result.m_rayStartPos = startPos;
	result.m_rayMaxLength = maxDist;

	if (IsPointInsideAABB3D(box, startPos))
	{
		result.m_didImpact = true;
		result.m_impactDist = 0.f;
		result.m_impactPos = startPos;
		result.m_impactNormal = -fwdNormal;
		return result;
	}

	Vec3 rayMaxEnd = startPos + (fwdNormal * maxDist);

	float tXMin = (box.m_mins.x - startPos.x) / fwdNormal.x;
	float tXMax = (box.m_maxs.x - startPos.x) / fwdNormal.x;
	float tYMin = (box.m_mins.y - startPos.y) / fwdNormal.y;
	float tYMax = (box.m_maxs.y - startPos.y) / fwdNormal.y;
	float tZMin = (box.m_mins.z - startPos.z) / fwdNormal.z;
	float tZMax = (box.m_maxs.z - startPos.z) / fwdNormal.z;

	if (tXMin > tXMax) std::swap(tXMin, tXMax);
	if (tYMin > tYMax) std::swap(tYMin, tYMax);
	if (tZMin > tZMax) std::swap(tZMin, tZMax);

	float tEnter = std::max(tXMin, tYMin);
	tEnter = std::max(tEnter, tZMin);
	float tExit = std::min(tXMax, tYMax);
	tExit = std::min(tExit, tZMax);

	if (tEnter > tExit || tExit < 0.f || tEnter > maxDist)
	{
		result.m_didImpact = false;
		result.m_impactDist = maxDist;
		result.m_impactPos = rayMaxEnd;
		result.m_impactNormal = fwdNormal;
		return result;
	}

	result.m_didImpact = true;
	result.m_impactDist = tEnter;
	result.m_impactPos = startPos + fwdNormal * result.m_impactDist;

	if (tEnter == tXMin)
		result.m_impactNormal = Vec3(-1.f * (fwdNormal.x > 0 ? 1 : -1), 0.f, 0.f);
	else if (tEnter == tXMax)
		result.m_impactNormal = Vec3(1.f * (fwdNormal.x > 0 ? 1 : -1), 0.f, 0.f);
	else if (tEnter == tYMin)
		result.m_impactNormal = Vec3(0.f, -1.f * (fwdNormal.y > 0 ? 1 : -1), 0.f);
	else if (tEnter == tYMax)
		result.m_impactNormal = Vec3(0.f, 1.f * (fwdNormal.y > 0 ? 1 : -1), 0.f);
	else if (tEnter == tZMin)
		result.m_impactNormal = Vec3(0.f, 0.f, -1.f * (fwdNormal.z > 0 ? 1 : -1));
	else if (tEnter == tZMax)
		result.m_impactNormal = Vec3(0.f, 0.f, 1.f * (fwdNormal.z > 0 ? 1 : -1));

	return result;
}

RaycastResult3D RaycastVsOBB3D(Vec3 startPos, Vec3 fwdNormal, float maxDist, OBB3 box)
{
	Vec3 endPos = fwdNormal * maxDist;
	Vec3 iBasis = box.m_iBasis;
	Vec3 jBasis = box.m_jBasis;
	Vec3 kBasis = box.m_kBasis;
	Vec3 rayMaxEnd = startPos + fwdNormal * maxDist;

	Vec3 startRel = startPos - box.m_center;
	Vec3 endRel = rayMaxEnd - box.m_center;

	float startPosi = DotProduct3D(startRel, iBasis);
	float startPosj = DotProduct3D(startRel, jBasis);
	float startPosk = DotProduct3D(startRel, kBasis);

	float endPosi = DotProduct3D(endRel, iBasis);
	float endPosj = DotProduct3D(endRel, jBasis);
	float endPosk = DotProduct3D(endRel, kBasis);

	Vec3 localSpaceStart = Vec3(startPosi, startPosj, startPosk);
	Vec3 localSpaceEnd = Vec3(endPosi, endPosj, endPosk);
	Vec3 localFwdNormal = (localSpaceEnd - localSpaceStart).GetNormalized();

	RaycastResult3D result;
	result.m_rayFwdNormal = fwdNormal;
	result.m_rayStartPos = startPos;
	result.m_rayMaxLength = maxDist;

	if (IsPointInsideOBB3D(box, startPos))
	{
		result.m_didImpact = true;
		result.m_impactDist = 0.f;
		result.m_impactPos = startPos;
		result.m_impactNormal = -fwdNormal;
		return result;
	}

	float tIMin = (-box.m_halfDims.x - localSpaceStart.x) / localFwdNormal.x;
	float tIMax = ( box.m_halfDims.x - localSpaceStart.x) / localFwdNormal.x;
	float tJMin = (-box.m_halfDims.y - localSpaceStart.y) / localFwdNormal.y;
	float tJMax = ( box.m_halfDims.y - localSpaceStart.y) / localFwdNormal.y;
	float tKMin = (-box.m_halfDims.z - localSpaceStart.z) / localFwdNormal.z;
	float tKMax = ( box.m_halfDims.z - localSpaceStart.z) / localFwdNormal.z;

	if (tIMin > tIMax) std::swap(tIMin, tIMax);
	if (tJMin > tJMax) std::swap(tJMin, tJMax);
	if (tKMin > tKMax) std::swap(tKMin, tKMax);

	float tEnter = std::max(tIMin, tJMin);
	tEnter = std::max(tEnter, tKMin);
	float tExit = std::min(tIMax, tJMax);
	tExit = std::min(tExit, tKMax);

	if (tEnter > tExit || tExit < 0.f || tEnter > maxDist)
	{
		result.m_didImpact = false;
		result.m_impactDist = maxDist;
		result.m_impactPos = rayMaxEnd;
		result.m_impactNormal = fwdNormal;
		return result;
	}

	result.m_didImpact = true;
	Vec3 localImpactPos = localSpaceStart + localFwdNormal * tEnter;
	Vec3 worldSpaceImpact = box.m_center + (box.m_iBasis * localImpactPos.x) + (box.m_jBasis * localImpactPos.y) + (box.m_kBasis * localImpactPos.z);
	result.m_impactPos = worldSpaceImpact;
	result.m_impactDist = tEnter;

	if (tEnter == tIMin)
	{
		if (DotProduct3D(box.m_iBasis, fwdNormal) > 0.f)
		{
			result.m_impactNormal = -box.m_iBasis;
		}
		else
		{
			result.m_impactNormal = box.m_iBasis;
		}
	}
	else if (tEnter == tIMax)
	{
		if (DotProduct3D(box.m_iBasis, fwdNormal) > 0.f)
		{
			result.m_impactNormal = -box.m_iBasis;
		}
		else
		{
			result.m_impactNormal = box.m_iBasis;
		}
	}
	else if (tEnter == tJMin)
	{
		if (DotProduct3D(box.m_jBasis, fwdNormal) > 0.f)
		{
			result.m_impactNormal = -box.m_jBasis;
		}
		else
		{
			result.m_impactNormal = box.m_jBasis;
		}
	}
	else if (tEnter == tJMax)
	{
		if (DotProduct3D(box.m_jBasis, fwdNormal) > 0.f)
		{
			result.m_impactNormal = -box.m_jBasis;
		}
		else
		{
			result.m_impactNormal = box.m_jBasis;
		}
	}
	else if (tEnter == tKMin)
	{
		if (DotProduct3D(box.m_kBasis, fwdNormal) > 0.f)
		{
			result.m_impactNormal = -box.m_kBasis;
		}
		else
		{
			result.m_impactNormal = box.m_kBasis;
		}
	}
	else if (tEnter == tKMax)
	{
		if (DotProduct3D(box.m_kBasis, fwdNormal) > 0.f)
		{
			result.m_impactNormal = -box.m_kBasis;
		}
		else
		{
			result.m_impactNormal = box.m_kBasis;
		}
	}

	return result;
}

RaycastResult3D RaycastVsCylinderZ3D(Vec3 startPos, Vec3 fwdNormal, float maxDist, Vec3 const& center, FloatRange const& minMaxZ, float radiusXY)
{
	RaycastResult3D result;
	result.m_rayFwdNormal = fwdNormal;
	result.m_rayStartPos = startPos;
	result.m_rayMaxLength = maxDist;
	if (IsPointInsideZCylinder3D(center, radiusXY, minMaxZ, startPos))
	{
		result.m_didImpact = true;
		result.m_impactDist = 0.f;
		result.m_impactPos = startPos;
		result.m_impactNormal = -fwdNormal;
		return result;
	}

	Vec3 ray = (fwdNormal * maxDist);
	Vec3 endPos = startPos + ray;
	float tEntryZ = RangeMap(minMaxZ.m_min, startPos.z, endPos.z, 0.f, 1.f);
	float tExitZ = RangeMap(minMaxZ.m_max, startPos.z, endPos.z, 0.f, 1.f);
	if (tEntryZ > tExitZ)
	{
		float temp = tExitZ;
		tExitZ = tEntryZ;
		tEntryZ = temp;
	}
	FloatRange rayRange = FloatRange(startPos.z, endPos.z);
	if (startPos.z >= endPos.z)
	{
		rayRange = FloatRange(endPos.z, startPos.z);
	}

	if (!minMaxZ.IsOverlappingWith(rayRange) ||
		tExitZ < 0.f || tEntryZ > 1.f)
	{
		result.m_didImpact = false;
		result.m_impactDist = maxDist;
		result.m_impactPos = endPos;
		result.m_impactNormal = fwdNormal;
		return result;
	}

	Vec2 flattenedRay = Vec2(ray.x, ray.y);
	Vec2 flatRayNormalized = flattenedRay.GetNormalized();
	float rayToCenterXYj = DotProduct2D(flatRayNormalized.GetRotated90Degrees(), (Vec2(center.x, center.y) - Vec2(startPos.x, startPos.y)));
	if (abs(rayToCenterXYj) >= radiusXY)
	{
		result.m_didImpact = false;
		result.m_impactDist = maxDist;
		result.m_impactPos = endPos;
		result.m_impactNormal = fwdNormal;
		return result;
	}
	float rayToCenterXYi = DotProduct2D(flatRayNormalized, (Vec2(center.x, center.y) - Vec2(startPos.x, startPos.y)));
	float flatMaxRayLength = flattenedRay.GetLength();
	if (rayToCenterXYi <= -radiusXY || rayToCenterXYi >= radiusXY + flatMaxRayLength)
	{
		result.m_didImpact = false;
		result.m_impactDist = maxDist;
		result.m_impactPos = endPos;
		result.m_impactNormal = fwdNormal;
		return result;
	}
	float radiusAdjustment = std::sqrt(radiusXY * radiusXY - rayToCenterXYj * rayToCenterXYj);
	if (rayToCenterXYi - radiusAdjustment >= flatMaxRayLength)
	{
		result.m_didImpact = false;
		result.m_impactDist = maxDist;
		result.m_impactPos = endPos;
		result.m_impactNormal = fwdNormal;
		return result;
	}

	float tEntryXY = RangeMap(rayToCenterXYi - radiusAdjustment, 0.f, flatMaxRayLength, 0.f, 1.f);
	float tExitXY = RangeMap(rayToCenterXYi + radiusAdjustment, 0.f, flatMaxRayLength, 0.f, 1.f);
	FloatRange ztRange = FloatRange(tEntryZ, tExitZ);
	FloatRange xytRange = FloatRange(tEntryXY, tExitXY);
	if (!ztRange.IsOverlappingWith(xytRange) && !(tEntryZ == 0.f && tExitZ == 0.f))
	{
		result.m_didImpact = false;
		result.m_impactDist = maxDist;
		result.m_impactPos = endPos;
		result.m_impactNormal = fwdNormal;
		return result;
	}

	if (minMaxZ.IsOnRange(startPos.z))
	{
		result.m_impactDist = RangeMap(tEntryXY, 0.f, 1.f, 0.f, maxDist);
		result.m_impactPos = startPos + fwdNormal * result.m_impactDist;
		result.m_impactNormal = (result.m_impactPos - Vec3(center.x, center.y, result.m_impactPos.z)).GetNormalized();
	}
	else if (IsPointInsideDisc2D(Vec2(startPos.x, startPos.y), Vec2(center.x, center.y), radiusXY))
	{
		result.m_impactDist = RangeMap(tEntryZ, 0.f, 1.f, 0.f, maxDist);
		result.m_impactPos = startPos + fwdNormal * result.m_impactDist;
		if (startPos.z < minMaxZ.m_min)
		{
			result.m_impactNormal = Vec3(0.f, 0.f, -1.f);
		}
		else
		{
			result.m_impactNormal = Vec3(0.f, 0.f, 1.f);
		}
	}
	else if (tEntryZ > tEntryXY)
	{
		result.m_impactDist = RangeMap(tEntryZ, 0.f, 1.f, 0.f, maxDist);
		result.m_impactPos = startPos + fwdNormal * result.m_impactDist;
		if (startPos.z < minMaxZ.m_min)
		{
			result.m_impactNormal = Vec3(0.f, 0.f, -1.f);
		}
		else
		{
			result.m_impactNormal = Vec3(0.f, 0.f, 1.f);
		}
	}
	else
	{
		result.m_impactDist = RangeMap(tEntryXY, 0.f, 1.f, 0.f, maxDist);
		result.m_impactPos = startPos + fwdNormal * result.m_impactDist;
		result.m_impactNormal = (result.m_impactPos - Vec3(center.x, center.y, result.m_impactPos.z)).GetNormalized();
	}

	result.m_didImpact = true;

	return result;
}

RaycastResult3D RaycastVsPlane3D(Vec3 startPos, Vec3 fwdNormal, float maxDist, Plane3 plane)
{
	RaycastResult3D result;
	result.m_rayFwdNormal = fwdNormal;
	result.m_rayStartPos = startPos;
	result.m_rayMaxLength = maxDist;

	Vec3 endPos = startPos + (fwdNormal * maxDist);
	if (plane.IsPointInFrontOfPlane(startPos) && plane.IsPointInFrontOfPlane(endPos) ||
		!plane.IsPointInFrontOfPlane(startPos) && !plane.IsPointInFrontOfPlane(endPos))
	{
		result.m_didImpact = false;
		result.m_impactDist = maxDist;
		result.m_impactPos = endPos;
		result.m_impactNormal = fwdNormal;
		return result;
	}

	float startPointAltitude = DotProduct3D(startPos, plane.m_normal) - plane.m_distanceFromOrigin;
	float shortHeight = DotProduct3D(fwdNormal, plane.m_normal);
	result.m_impactDist = startPointAltitude / -shortHeight;
	result.m_didImpact = true;
	result.m_impactPos = startPos + (fwdNormal * result.m_impactDist);
	
	if (plane.IsPointInFrontOfPlane(startPos))
	{
		result.m_impactNormal = plane.m_normal;
	}
	else
	{
		result.m_impactNormal = -plane.m_normal;
	}

	return result;
}
