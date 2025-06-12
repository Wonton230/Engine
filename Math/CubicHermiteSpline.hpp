#pragma once
#include <vector>
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/CubicBezierCurve2D.hpp"
#include "Engine/Math/CubicHermiteCurve2D.hpp"

class CubicHermiteSpline
{
public:
	CubicHermiteSpline();
	CubicHermiteSpline(std::vector<Vec2> splinePoints);

	float GetApproximateLengthTotal(int numSubdivisions = 64);
	float GetApproximateLengthIndex(int index, int numSubdivisions = 64);
	int GetNumberOfCurves();
	Vec2 EvaluateTotalPositionAtDistance(float distance, int numSubdivisions);
	Vec2 EvaluateParametricAtIndexCurve(float parametric, int curveIndex);

	std::vector<Vec2> m_splinePoints;
	std::vector<CubicHermiteCurve2D> m_hermiteCurves;
};