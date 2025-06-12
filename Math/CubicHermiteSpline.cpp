#include "CubicHermiteSpline.hpp"

CubicHermiteSpline::CubicHermiteSpline()
{
	m_hermiteCurves;
	m_splinePoints;
}

CubicHermiteSpline::CubicHermiteSpline(std::vector<Vec2> splinePoints)
{
	m_splinePoints = splinePoints;

	std::vector<Vec2> velocities;
	velocities.push_back(Vec2(0, 0));
	for (int i = 1; i < (int)splinePoints.size() - 1; i++)
	{
		velocities.push_back((m_splinePoints[i + 1] - m_splinePoints[i - 1]) * .5f);
	}
	velocities.push_back(Vec2(0, 0));

	for (int i = 0; i < (int)splinePoints.size() - 1; i++)
	{
		m_hermiteCurves.push_back(CubicHermiteCurve2D(m_splinePoints[i], velocities[i], m_splinePoints[i + 1], velocities[i + 1]));
	}
}

float CubicHermiteSpline::GetApproximateLengthTotal(int numSubdivisions)
{
	float totalLength = 0.0f;
	for (int i = 0; i < (int)m_hermiteCurves.size(); i++)
	{
		totalLength += m_hermiteCurves[i].GetApproximateLength(numSubdivisions);
	}
	return totalLength;
}

float CubicHermiteSpline::GetApproximateLengthIndex(int index, int numSubdivisions)
{
	if (index < 0 || index > m_hermiteCurves.size() - 1)
	{
		return -1.f;
	}
	return m_hermiteCurves[index].GetApproximateLength(numSubdivisions);
}

int CubicHermiteSpline::GetNumberOfCurves()
{
	return (int)m_hermiteCurves.size();
}

Vec2 CubicHermiteSpline::EvaluateTotalPositionAtDistance(float distance, int numSubdivisions)
{
	if (distance > GetApproximateLengthTotal(numSubdivisions))
	{
		return m_hermiteCurves[0].m_start;
	}
	if (distance < 0.f)
	{
		return m_hermiteCurves[(int)m_hermiteCurves.size() - 1].m_end;
	}
	
	float currentDistance = distance;
	for (int i = 0; i < (int)m_hermiteCurves.size(); i++)
	{
		float length = m_hermiteCurves[i].GetApproximateLength();
		if (currentDistance <= length)
		{
			return m_hermiteCurves[i].EvaluateAtApproximateDistance(currentDistance);
		}
		else
		{
			currentDistance = currentDistance - length;
		}
	}
	return Vec2();
}

Vec2 CubicHermiteSpline::EvaluateParametricAtIndexCurve(float parametric, int curveIndex)
{
	if (curveIndex < 0 || curveIndex > m_hermiteCurves.size() - 1)
	{
		return Vec2();
	}
	return m_hermiteCurves[curveIndex].EvaluateAtParametric(parametric);
}
