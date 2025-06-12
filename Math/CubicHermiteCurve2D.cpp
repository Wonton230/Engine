#pragma once
#include "CubicHermiteCurve2D.hpp"
#include "Engine/Math/CubicBezierCurve2D.hpp"
#include "Engine/Math/MathUtils.hpp"

CubicHermiteCurve2D::CubicHermiteCurve2D(Vec2 start, Vec2 velStart, Vec2 end, Vec2 velEnd)
{
	m_start = start;
	m_startVel = velStart;
	m_end = end;
	m_endVel = velEnd;
}

CubicHermiteCurve2D::CubicHermiteCurve2D(CubicBezierCurve2D const& curve)
{
	m_start = curve.m_start;
	m_end = curve.m_end;
	m_startVel = 3 * (curve.m_guideStart - m_start);
	m_endVel = 3 * (m_end - curve.m_guideEnd);
}

Vec2 CubicHermiteCurve2D::EvaluateAtParametric(float parametricZeroToOne) const
{
	float t2 = parametricZeroToOne * parametricZeroToOne;
	float t3 = t2 * parametricZeroToOne;

	float h00 = 2.f * t3 - 3.f * t2 + 1.f;
	float h10 = t3 - 2.f * t2 + parametricZeroToOne;
	float h01 = -2.f * t3 + 3.f * t2;
	float h11 = t3 - t2;

	return h00 * m_start + h10 * m_startVel + h01 * m_end + h11 * m_endVel;
}

float CubicHermiteCurve2D::GetApproximateLength(int numSubDivisions) const
{
	float fractionStep = 1.f / numSubDivisions;
	float distanceSum = 0.f;
	for (int i = 0; i <= numSubDivisions; i++)
	{
		Vec2 step = EvaluateAtParametric(fractionStep * (i + 1)) - EvaluateAtParametric(fractionStep * i);
		distanceSum = distanceSum + step.GetLength();
	}
	return (distanceSum);
}

Vec2 CubicHermiteCurve2D::EvaluateAtApproximateDistance(float length, int numSubDivisions) const
{
	std::vector<float> cumulativeLengths;
	std::vector<float> tValues;
	cumulativeLengths.push_back(0.f);
	tValues.push_back(0.f);

	Vec2 prev = EvaluateAtParametric(0.f);
	float totalLength = 0.f;
	float t = (float)1.f / (float)numSubDivisions;

	for (int i = 0; i <= numSubDivisions; ++i)
	{
		Vec2 current = EvaluateAtParametric(t * i);
		totalLength += (current - prev).GetLength();
		cumulativeLengths.push_back(totalLength);
		tValues.push_back(t * i);
		prev = current;
	}

	length = GetClamped(length, 0.f, totalLength);

	for (int i = 0; i <= numSubDivisions; ++i)
	{
		if (length <= cumulativeLengths[i + 1])
		{
			float segmentLength = cumulativeLengths[i + 1] - cumulativeLengths[i];
			float segmentFraction = (length - cumulativeLengths[i]) / segmentLength;
			t = Interpolate(tValues[i], tValues[i + 1], segmentFraction);
			return EvaluateAtParametric(t);
		}
	}

	return EvaluateAtParametric(1.f);
}
