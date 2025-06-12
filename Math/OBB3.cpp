#include "OBB3.hpp"

OBB3::OBB3()
{
	m_center = Vec3();
	m_iBasis = Vec3::WORLD_FORWARD;
	m_jBasis = Vec3::WORLD_LEFT;
	m_kBasis = Vec3::WORLD_UP;
	m_halfDims = Vec3(.5f,.5f,.5f);
}

OBB3::OBB3(Vec3 const& centerPos, Vec3 const& iBasis, Vec3 const& jBasis, Vec3 const& kBasis, Vec3 const& halfDims)
{
	m_center = centerPos;
	m_iBasis = iBasis;
	m_jBasis = jBasis;
	m_kBasis = kBasis;
	m_halfDims = halfDims;
}