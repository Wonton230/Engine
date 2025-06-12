#pragma once
#include "Engine/Math/Vec3.hpp"

struct OBB3
{
public:
	OBB3();
	OBB3(Vec3 const& centerPos, Vec3 const& iBasis, Vec3 const& jBasis, Vec3 const& kBasis, Vec3 const& halfDims);

	Vec3 m_center;
	Vec3 m_iBasis;
	Vec3 m_jBasis;
	Vec3 m_kBasis;
	Vec3 m_halfDims;
};