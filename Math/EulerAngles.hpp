#pragma once

struct Vec3;
struct Mat44;

struct EulerAngles
{
public:
	EulerAngles() = default;
	EulerAngles(float yawDegrees, float pitchDegrees, float rollDegrees);
	void GetAsVectors_IFwd_JLeft_KUp(Vec3& out_fwdIBasis, Vec3& out_rightJBasis, Vec3& out_upKBasis);
	Mat44 GetAsMatrix_IFwd_JLeft_KUp() const;
	void SetFromText(const char* text);
	Vec3 GetForwardNormal() const;

	float m_yawDegrees = 0.0f;
	float m_pitchDegrees = 0.0f;
	float m_rollDegrees = 0.0f;
};