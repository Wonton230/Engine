#include "EulerAngles.hpp"
#include "MathUtils.hpp"
#include "Vec3.hpp"
#include "Mat44.hpp"

EulerAngles::EulerAngles(float yawDegrees, float pitchDegrees, float rollDegrees)
{
	m_yawDegrees = yawDegrees;
	m_pitchDegrees = pitchDegrees;
	m_rollDegrees = rollDegrees;
}

void EulerAngles::GetAsVectors_IFwd_JLeft_KUp(Vec3& out_fwdIBasis, Vec3& out_rightJBasis, Vec3& out_upKBasis)
{
	Mat44 transformedBasis = GetAsMatrix_IFwd_JLeft_KUp();
	out_fwdIBasis = transformedBasis.GetIBasis3D();
	out_rightJBasis = transformedBasis.GetJBasis3D();
	out_upKBasis = transformedBasis.GetKBasis3D();
}

Mat44 EulerAngles::GetAsMatrix_IFwd_JLeft_KUp() const
{
	Mat44 returningMatrix = Mat44();

	float* array = returningMatrix.GetAsFloatArray();
	//IBasis
	array[0] = CosDegrees(m_pitchDegrees) * CosDegrees(m_yawDegrees);
	array[1] = CosDegrees(m_pitchDegrees) * SinDegrees(m_yawDegrees);
	array[2] = -SinDegrees(m_pitchDegrees);

	//JBasis
	array[4] = (CosDegrees(m_rollDegrees) * -SinDegrees(m_yawDegrees)) + (SinDegrees(m_rollDegrees) * SinDegrees(m_pitchDegrees) * CosDegrees(m_yawDegrees));
	array[5] = (CosDegrees(m_rollDegrees) * CosDegrees(m_yawDegrees)) + (SinDegrees(m_rollDegrees) * SinDegrees(m_pitchDegrees) * SinDegrees(m_yawDegrees));
	array[6] = SinDegrees(m_rollDegrees) * CosDegrees(m_pitchDegrees);

	//KBasis
	array[8] = (-SinDegrees(m_rollDegrees) * -SinDegrees(m_yawDegrees)) + (CosDegrees(m_rollDegrees) * SinDegrees(m_pitchDegrees) * CosDegrees(m_yawDegrees));
	array[9] = (-SinDegrees(m_rollDegrees) * CosDegrees(m_yawDegrees)) + (CosDegrees(m_rollDegrees) * SinDegrees(m_pitchDegrees) * SinDegrees(m_yawDegrees));
	array[10] = CosDegrees(m_rollDegrees) * CosDegrees(m_pitchDegrees);

	return returningMatrix;
}

void EulerAngles::SetFromText(const char* text)
{
	//VALID: y, p, r
	Strings variables = SplitStringOnDelimiter(text, ',');
	if (variables.size() != 3 || variables[0].c_str() == nullptr || variables[1].c_str() == nullptr || variables[2].c_str() == nullptr)
	{
		ERROR_AND_DIE("Invalid arguments passed into EulerAngles String");
	}
	m_yawDegrees = static_cast<float>(atof(variables[0].c_str()));
	m_pitchDegrees = static_cast<float>(atof(variables[1].c_str()));
	m_rollDegrees = static_cast<float>(atof(variables[2].c_str()));
}

Vec3 EulerAngles::GetForwardNormal() const
{
	Mat44 transformedBasis = GetAsMatrix_IFwd_JLeft_KUp();
	return transformedBasis.GetIBasis3D();
}
