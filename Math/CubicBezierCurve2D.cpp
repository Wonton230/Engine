#include "CubicBezierCurve2D.hpp"
#include "Engine/Math/CubicHermiteCurve2D.hpp"
#include "Engine/Math/MathUtils.hpp"

CubicBezierCurve2D::CubicBezierCurve2D(Vec2 start, Vec2 guide1, Vec2 guide2, Vec2 end)
{
	m_start = start;
	m_guideStart = guide1;
	m_guideEnd = guide2;
	m_end = end;
}

CubicBezierCurve2D::CubicBezierCurve2D(CubicHermiteCurve2D const& curve)
{
	m_start = curve.m_start;
	m_end = curve.m_end;
	m_guideStart = (curve.m_startVel / 3.f) + m_start;
	m_guideEnd = m_end - (curve.m_endVel / 3.f);
}

Vec2 CubicBezierCurve2D::EvaluateAtParametric(float parametricZeroToOne) const
{
	Vec2 guideLine1 = Lerp(m_start, m_guideStart, parametricZeroToOne);
	Vec2 guideLine2 = Lerp(m_guideStart, m_guideEnd, parametricZeroToOne);
	Vec2 guideLine3 = Lerp(m_guideEnd, m_end, parametricZeroToOne);

	Vec2 guideLinePrime1 = Lerp(guideLine1, guideLine2, parametricZeroToOne);
	Vec2 guideLinePrime2 = Lerp(guideLine2, guideLine3, parametricZeroToOne);
	return Lerp(guideLinePrime1, guideLinePrime2, parametricZeroToOne);
}

float CubicBezierCurve2D::GetApproximateLength(int numSubDivisions) const
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

Vec2 CubicBezierCurve2D::EvaluateAtApproximateDistance(float length, int numSubDivisions) const
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