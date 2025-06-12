#include "AABB3.hpp"

AABB3::AABB3(AABB3 const& copyFrom)
{
	m_mins = copyFrom.m_mins;
	m_maxs = copyFrom.m_maxs;
}

AABB3::AABB3(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
{
	m_mins = Vec3(minX, minY, minZ);
	m_maxs = Vec3(maxX, maxY, maxZ);
}

AABB3::AABB3(Vec3 const& mins, Vec3 const& maxs)
{
	m_mins = mins;
	m_maxs = maxs;
}

bool AABB3::IsPointInside(Vec3 const& point) const
{
	if (m_mins.x <= point.x && point.x <= m_maxs.x)
	{
		if (m_mins.y <= point.y && point.y <= m_maxs.y) 
		{
			if (m_mins.z <= point.z && point.z <= m_maxs.z)
			{
				return true;
			}
		}
	}
	return false;
}
