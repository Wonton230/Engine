#include "Mat44.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Vec4.hpp"
#include "Engine/Math/MathUtils.hpp"

Mat44::Mat44()
{
	m_values[Ix] = 1.f; m_values[Jx] = 0.f; m_values[Kx] = 0.f; m_values[Tx] = 0.f;
	m_values[Iy] = 0.f; m_values[Jy] = 1.f; m_values[Ky] = 0.f; m_values[Ty] = 0.f;
	m_values[Iz] = 0.f; m_values[Jz] = 0.f; m_values[Kz] = 1.f; m_values[Tz] = 0.f;
	m_values[Iw] = 0.f; m_values[Jw] = 0.f; m_values[Kw] = 0.f; m_values[Tw] = 1.f;
}

Mat44::Mat44(Vec2 const& ibasis2D, Vec2 const& jbasis2D, Vec2 const& translation2D)
{
	m_values[Ix] = ibasis2D.x;	m_values[Jx] = jbasis2D.x;	m_values[Kx] = 0.f;			m_values[Tx] = translation2D.x;
	m_values[Iy] = ibasis2D.y;	m_values[Jy] = jbasis2D.y;	m_values[Ky] = 0.f;			m_values[Ty] = translation2D.y;
	m_values[Iz] = 0.f;			m_values[Jz] = 0.f;			m_values[Kz] = 1.f;			m_values[Tz] = 0.f;
	m_values[Iw] = 0.f;			m_values[Jw] = 0.f;			m_values[Kw] = 0.f;			m_values[Tw] = 1.f;
}

Mat44::Mat44(Vec3 const& ibasis3D, Vec3 const& jbasis3D, Vec3 const& kbasis3D, Vec3 const& translation3D)
{
	m_values[Ix] = ibasis3D.x;	m_values[Jx] = jbasis3D.x;	m_values[Kx] = kbasis3D.x;	m_values[Tx] = translation3D.x;
	m_values[Iy] = ibasis3D.y;	m_values[Jy] = jbasis3D.y;	m_values[Ky] = kbasis3D.y;	m_values[Ty] = translation3D.y;
	m_values[Iz] = ibasis3D.z;	m_values[Jz] = jbasis3D.z;	m_values[Kz] = kbasis3D.z;	m_values[Tz] = translation3D.z;
	m_values[Iw] = 0.f;			m_values[Jw] = 0.f;			m_values[Kw] = 0.f;			m_values[Tw] = 1.f;
}

Mat44::Mat44(Vec4 const& ibasis4D, Vec4 const& jbasis4D, Vec4 const& kbasis4D, Vec4 const& translation4D)
{
	m_values[Ix] = ibasis4D.x;	m_values[Jx] = jbasis4D.x;	m_values[Kx] = kbasis4D.x;	m_values[Tx] = translation4D.x;
	m_values[Iy] = ibasis4D.y;	m_values[Jy] = jbasis4D.y;	m_values[Ky] = kbasis4D.y;	m_values[Ty] = translation4D.y;
	m_values[Iz] = ibasis4D.z;	m_values[Jz] = jbasis4D.z;	m_values[Kz] = kbasis4D.z;	m_values[Tz] = translation4D.z;
	m_values[Iw] = ibasis4D.w;	m_values[Jw] = jbasis4D.w;	m_values[Kw] = kbasis4D.w;	m_values[Tw] = translation4D.w;
}

Mat44::Mat44(const float* const sixteenValueBasisMajor)
{
	m_values[Ix] = sixteenValueBasisMajor[0];	m_values[Jx] = sixteenValueBasisMajor[4];	m_values[Kx] = sixteenValueBasisMajor[8];	m_values[Tx] = sixteenValueBasisMajor[12];
	m_values[Iy] = sixteenValueBasisMajor[1];	m_values[Jy] = sixteenValueBasisMajor[5];	m_values[Ky] = sixteenValueBasisMajor[9];	m_values[Ty] = sixteenValueBasisMajor[13];
	m_values[Iz] = sixteenValueBasisMajor[2];	m_values[Jz] = sixteenValueBasisMajor[6];	m_values[Kz] = sixteenValueBasisMajor[10];	m_values[Tz] = sixteenValueBasisMajor[14];
	m_values[Iw] = sixteenValueBasisMajor[3];	m_values[Jw] = sixteenValueBasisMajor[7];	m_values[Kw] = sixteenValueBasisMajor[11];	m_values[Tw] = sixteenValueBasisMajor[15];
}

Mat44 const Mat44::MakeTranslation2D(Vec2 const& translationXY)
{
	Mat44 translation = Mat44(Vec2(1.f,0.f), Vec2(0.f,1.f), translationXY);
	return translation;
}

Mat44 const Mat44::MakeTranslation3D(Vec3 const& translationXYZ)
{
	Mat44 translation = Mat44(Vec3(1.f, 0.f, 0.f), Vec3(0.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f), translationXYZ);
	return translation;
}

Mat44 const Mat44::MakeUniformScale2D(float uniformScaleXY)
{
	Mat44 uScale = Mat44(Vec2(uniformScaleXY, 0.f), Vec2(0.f, uniformScaleXY), Vec2(0.f,0.f));
	return uScale;
}

Mat44 const Mat44::MakeUniformScale3D(float uniformScaleXYZ)
{
	Mat44 uScale = Mat44(Vec3(uniformScaleXYZ, 0.f, 0.f), Vec3(0.f, uniformScaleXYZ, 0.f), Vec3(0.f, 0.f, uniformScaleXYZ), Vec3(0.f, 0.f, 0.f));
	return uScale;
}

Mat44 const Mat44::MakeNonUniformScale2D(Vec2 const& nonUniformScaleXY)
{
	Mat44 nUScale = Mat44(Vec2(nonUniformScaleXY.x, 0.f), Vec2(0.f, nonUniformScaleXY.y), Vec2(0.f, 0.f));
	return nUScale;
}

Mat44 const Mat44::MakeNonUniformScale3D(Vec3 const& nonUniformScaleXYZ)
{
	Mat44 nUScale = Mat44(Vec3(nonUniformScaleXYZ.x, 0.f, 0.f), Vec3(0.f, nonUniformScaleXYZ.y, 0.f), Vec3(0.f, 0.f, nonUniformScaleXYZ.z), Vec3(0.f, 0.f, 0.f));
	return nUScale;
}

Mat44 const Mat44::MakeZRotationDegrees(float rotationDegreesAboutZ)
{
	Mat44 rotation = Mat44(Vec4(CosDegrees(rotationDegreesAboutZ),SinDegrees(rotationDegreesAboutZ), 0.f, 0.f), Vec4(-SinDegrees(rotationDegreesAboutZ), CosDegrees(rotationDegreesAboutZ),0.f,0.f), Vec4(0.f,0.f,1.f,0.f), Vec4(0.f,0.f,0.f,1.f));
	return rotation;
}

Mat44 const Mat44::MakeYRotationDegrees(float rotationDegreesAboutY)
{
	Mat44 rotation = Mat44(Vec4(CosDegrees(rotationDegreesAboutY), 0.f, -SinDegrees(rotationDegreesAboutY), 0.f), Vec4(0.f, 1.f, 0.f, 0.f), Vec4(SinDegrees(rotationDegreesAboutY), 0.f, CosDegrees(rotationDegreesAboutY), 0.f), Vec4(0.f, 0.f, 0.f, 1.f));
	return rotation;
}

Mat44 const Mat44::MakeXRotationDegrees(float rotationDegreesAboutX)
{
	Mat44 rotation = Mat44(Vec4(1.f, 0.f, 0.f, 0.f), Vec4(0.f, CosDegrees(rotationDegreesAboutX), SinDegrees(rotationDegreesAboutX), 0.f), Vec4(0.f, -SinDegrees(rotationDegreesAboutX), CosDegrees(rotationDegreesAboutX), 0.f), Vec4(0.f, 0.f, 0.f, 1.f));
	return rotation;
}

Mat44 const Mat44::MakeOrthoProjection(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Mat44 temp = Mat44();
	temp.m_values[Ix] = 2.0f / (right - left);
	temp.m_values[Tx] = (left + right) / (left - right);

	temp.m_values[Jy] = 2.0f / (top - bottom);
	temp.m_values[Ty] = (top + bottom) / (bottom - top);

	temp.m_values[Kz] = 1.0f / (zFar - zNear);
	temp.m_values[Tz] = -(zNear / (zFar - zNear));

	return temp;
}

Mat44 const Mat44::MakePerspectiveProjection(float fovYDegrees, float aspect, float zNear, float zFar)
{
	Mat44 perspective = Mat44();
	float cosFOV = CosDegrees(fovYDegrees * .5f);
	float sinFOV = SinDegrees(fovYDegrees * .5f);
	float yScale = cosFOV / sinFOV;
	float xScale = yScale / aspect;
	float zScale = zFar / (zFar - zNear);
	float zTranslation = (zNear * zFar) / (zNear - zFar);

	perspective.m_values[Ix] = xScale;
	perspective.m_values[Jy] = yScale;
	perspective.m_values[Kz] = zScale;
	perspective.m_values[Tz] = zTranslation;
	perspective.m_values[Tw] = 0.f;
	perspective.m_values[Kw] = 1.0f;
	return perspective;
}

Vec2 const Mat44::TransformVectorQuantity2D(Vec2 const& vectorQuantityXY) const
{
	float x = m_values[Ix] * vectorQuantityXY.x + m_values[Jx] * vectorQuantityXY.y;
	float y = m_values[Iy] * vectorQuantityXY.x + m_values[Jy] * vectorQuantityXY.y;
	return Vec2(x,y);
}

Vec3 const Mat44::TransformVectorQuantity3D(Vec3 const& vectorQuantityXYZ) const
{
	float x = m_values[Ix] * vectorQuantityXYZ.x + m_values[Jx] * vectorQuantityXYZ.y + m_values[Kx] * vectorQuantityXYZ.z;
	float y = m_values[Iy] * vectorQuantityXYZ.x + m_values[Jy] * vectorQuantityXYZ.y + m_values[Ky] * vectorQuantityXYZ.z;
	float z = m_values[Iz] * vectorQuantityXYZ.x + m_values[Jz] * vectorQuantityXYZ.y + m_values[Kz] * vectorQuantityXYZ.z;
	return Vec3(x, y, z);
}

Vec2 const Mat44::TransformPosition2D(Vec2 const& positionXY) const
{
	float x = m_values[Ix] * positionXY.x + m_values[Jx] * positionXY.y + m_values[Tx];
	float y = m_values[Iy] * positionXY.x + m_values[Jy] * positionXY.y + m_values[Ty];
	return Vec2(x, y);
}

Vec3 const Mat44::TransformPosition3D(Vec3 const& positionXYZ) const
{
	float x = m_values[Ix] * positionXYZ.x + m_values[Jx] * positionXYZ.y + m_values[Kx] * positionXYZ.z + m_values[Tx];
	float y = m_values[Iy] * positionXYZ.x + m_values[Jy] * positionXYZ.y + m_values[Ky] * positionXYZ.z + m_values[Ty];
	float z = m_values[Iz] * positionXYZ.x + m_values[Jz] * positionXYZ.y + m_values[Kz] * positionXYZ.z + m_values[Tz];
	return Vec3(x, y, z);
}

Vec4 const Mat44::TransformHomogeneous3D(Vec4 const& homogeneousPoint3D) const
{
	float x = m_values[Ix] * homogeneousPoint3D.x + m_values[Jx] * homogeneousPoint3D.y + m_values[Kx] * homogeneousPoint3D.z + m_values[Tx] * homogeneousPoint3D.w;
	float y = m_values[Iy] * homogeneousPoint3D.x + m_values[Jy] * homogeneousPoint3D.y + m_values[Ky] * homogeneousPoint3D.z + m_values[Ty] * homogeneousPoint3D.w;
	float z = m_values[Iz] * homogeneousPoint3D.x + m_values[Jz] * homogeneousPoint3D.y + m_values[Kz] * homogeneousPoint3D.z + m_values[Tz] * homogeneousPoint3D.w;
	float w = m_values[Iw] * homogeneousPoint3D.x + m_values[Jw] * homogeneousPoint3D.y + m_values[Kw] * homogeneousPoint3D.z + m_values[Tw] * homogeneousPoint3D.w;
	return Vec4(x, y, z, w);
}

float* Mat44::GetAsFloatArray()
{
	return &m_values[0]; // or simply return m_values;
}

float const* Mat44::GetAsFloatArray() const
{
	float* returnArray = new float[16]{
		m_values[Ix], m_values[Iy], m_values[Iz], m_values[Iw],
		m_values[Jx], m_values[Jy], m_values[Jz], m_values[Jw],
		m_values[Kx], m_values[Ky], m_values[Kz], m_values[Kw],
		m_values[Tx], m_values[Ty], m_values[Tz], m_values[Tw]
	};
	return returnArray;
}

Vec2 const Mat44::GetIBasis2D() const
{
	return Vec2(m_values[Ix], m_values[Iy]);
}

Vec2 const Mat44::GetJBasis2D() const
{
	return Vec2(m_values[Jx], m_values[Jy]);
}

Vec2 const Mat44::GetTranslation2D() const
{
	return Vec2(m_values[Tx], m_values[Ty]);
}

Vec3 const Mat44::GetIBasis3D() const
{
	return Vec3(m_values[Ix], m_values[Iy], m_values[Iz]);
}

Vec3 const Mat44::GetJBasis3D() const
{
	return Vec3(m_values[Jx], m_values[Jy], m_values[Jz]);
}

Vec3 const Mat44::GetKBasis3D() const
{
	return Vec3(m_values[Kx], m_values[Ky], m_values[Kz]);
}

Vec3 const Mat44::GetTranslation3D() const
{
	return Vec3(m_values[Tx], m_values[Ty], m_values[Tz]);
}

Vec4 const Mat44::GetIBasis4D() const
{
	return Vec4(m_values[Ix], m_values[Iy], m_values[Iz], m_values[Iw]);
}

Vec4 const Mat44::GetJBasis4D() const
{
	return Vec4(m_values[Jx], m_values[Jy], m_values[Jz], m_values[Jw]);
}

Vec4 const Mat44::GetKBasis4D() const
{
	return Vec4(m_values[Kx], m_values[Ky], m_values[Kz], m_values[Kw]);
}

Vec4 const Mat44::GetTranslation4D() const
{
	return Vec4(m_values[Tx], m_values[Ty], m_values[Tz], m_values[Tw]);
}

Mat44 const Mat44::GetOrthonormalInverse() const
{
	Mat44 RInverse = Mat44(m_values);
	Mat44 TInverse = Mat44();
	RInverse.SetTranslation3D(Vec3(0.0f, 0.0f, 0.0f));
	RInverse.Transpose();
	TInverse.SetTranslation3D(Vec3(-m_values[Tx], -m_values[Ty], -m_values[Tz]));

	RInverse.Append(TInverse);
	return RInverse;
}

void Mat44::SetTranslation2D(Vec2 const& translationXY)
{
	m_values[Tx] = translationXY.x;
	m_values[Ty] = translationXY.y;
	m_values[Tz] = 0.f;
	m_values[Tw] = 1.f;
}

void Mat44::SetTranslation3D(Vec3 const& translationXYZ)
{
	m_values[Tx] = translationXYZ.x;
	m_values[Ty] = translationXYZ.y;
	m_values[Tz] = translationXYZ.z;
	m_values[Tw] = 1.f;
}

void Mat44::SetIJ2D(Vec2 const& iBasis2D, Vec2 const& jBasis2D)
{
	m_values[Ix] = iBasis2D.x;
	m_values[Iy] = iBasis2D.y;
	m_values[Iz] = 0.f;
	m_values[Iw] = 0.f;

	m_values[Jx] = jBasis2D.x;
	m_values[Jy] = jBasis2D.y;
	m_values[Jz] = 0.f;
	m_values[Jw] = 0.f;
}

void Mat44::SetIJT2D(Vec2 const& iBasis2D, Vec2 const& jBasis2D, Vec2 translationXY)
{
	m_values[Ix] = iBasis2D.x;
	m_values[Iy] = iBasis2D.y;
	m_values[Iz] = 0.f;
	m_values[Iw] = 0.f;

	m_values[Jx] = jBasis2D.x;
	m_values[Jy] = jBasis2D.y;
	m_values[Jz] = 0.f;
	m_values[Jw] = 0.f;

	m_values[Tx] = translationXY.x;
	m_values[Ty] = translationXY.y;
	m_values[Tz] = 0.f;
	m_values[Tw] = 1.f;
}

void Mat44::SetIJK3D(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D)
{
	m_values[Ix] = iBasis3D.x;
	m_values[Iy] = iBasis3D.y;
	m_values[Iz] = iBasis3D.z;
	m_values[Iw] = 0.f;

	m_values[Jx] = jBasis3D.x;
	m_values[Jy] = jBasis3D.y;
	m_values[Jz] = jBasis3D.z;
	m_values[Jw] = 0.f;

	m_values[Kx] = kBasis3D.x;
	m_values[Ky] = kBasis3D.y;
	m_values[Kz] = kBasis3D.z;
	m_values[Kw] = 0.f;
}

void Mat44::SetIJKT3D(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D, Vec3 const& translationXYZ)
{
	m_values[Ix] = iBasis3D.x;
	m_values[Iy] = iBasis3D.y;
	m_values[Iz] = iBasis3D.z;
	m_values[Iw] = 0.f;

	m_values[Jx] = jBasis3D.x;
	m_values[Jy] = jBasis3D.y;
	m_values[Jz] = jBasis3D.z;
	m_values[Jw] = 0.f;

	m_values[Kx] = kBasis3D.x;
	m_values[Ky] = kBasis3D.y;
	m_values[Kz] = kBasis3D.z;
	m_values[Kw] = 0.f;

	m_values[Tx] = translationXYZ.x;
	m_values[Ty] = translationXYZ.y;
	m_values[Tz] = translationXYZ.z;
	m_values[Tw] = 1.f;
}

void Mat44::SetIJKT4D(Vec4 const& iBasis4D, Vec4 const& jBasis4D, Vec4 const& kBasis4D, Vec4 const& translationXYZW)
{
	m_values[Ix] = iBasis4D.x;
	m_values[Iy] = iBasis4D.y;
	m_values[Iz] = iBasis4D.z;
	m_values[Iw] = iBasis4D.w;

	m_values[Jx] = jBasis4D.x;
	m_values[Jy] = jBasis4D.y;
	m_values[Jz] = jBasis4D.z;
	m_values[Jw] = jBasis4D.w;

	m_values[Kx] = kBasis4D.x;
	m_values[Ky] = kBasis4D.y;
	m_values[Kz] = kBasis4D.z;
	m_values[Kw] = kBasis4D.w;

	m_values[Tx] = translationXYZW.x;
	m_values[Ty] = translationXYZW.y;
	m_values[Tz] = translationXYZW.z;
	m_values[Tw] = translationXYZW.w;
}

void Mat44::Transpose()
{
	Mat44 storage = Mat44(m_values);
	m_values[Iy] = storage.m_values[Jx];
	m_values[Iz] = storage.m_values[Kx];
	m_values[Iw] = storage.m_values[Tx];

	m_values[Jx] = storage.m_values[Iy];
	m_values[Jz] = storage.m_values[Ky];
	m_values[Jw] = storage.m_values[Ty];

	m_values[Kx] = storage.m_values[Iz];
	m_values[Ky] = storage.m_values[Jz];
	m_values[Kw] = storage.m_values[Tz];

	m_values[Tx] = storage.m_values[Iw];
	m_values[Ty] = storage.m_values[Jw];
	m_values[Tz] = storage.m_values[Kw];
}

void Mat44::Orthonormalize_IFwd_JLeft_KUp()
{
	Vec3 i = Vec3(GetIBasis3D());
	i /= i.GetLength();

	Vec3 Ki = i * DotProduct3D(GetKBasis3D(), i);
	Vec3 k = Vec3(GetKBasis3D() - Ki);
	k /= k.GetLength();

	Vec3 Ji = i * DotProduct3D(GetJBasis3D(), i);
	Vec3 Jk = k * DotProduct3D(GetJBasis3D(), k);
	Vec3 j = Vec3(GetJBasis3D() - Ji - Jk);
	j /= j.GetLength();

	SetIJK3D(i, j, k);
}

void Mat44::Append(Mat44 const& appendThis)
{
	Mat44 copyThis = Mat44(m_values);
	m_values[Ix] = (copyThis.m_values[Ix] * appendThis.m_values[Ix]) + (copyThis.m_values[Jx] * appendThis.m_values[Iy]) + (copyThis.m_values[Kx] * appendThis.m_values[Iz]) + (copyThis.m_values[Tx] * appendThis.m_values[Iw]);  //myIx
	m_values[Iy] = (copyThis.m_values[Iy] * appendThis.m_values[Ix]) + (copyThis.m_values[Jy] * appendThis.m_values[Iy]) + (copyThis.m_values[Ky] * appendThis.m_values[Iz]) + (copyThis.m_values[Ty] * appendThis.m_values[Iw]);  //myIy
	m_values[Iz] = (copyThis.m_values[Iz] * appendThis.m_values[Ix]) + (copyThis.m_values[Jz] * appendThis.m_values[Iy]) + (copyThis.m_values[Kz] * appendThis.m_values[Iz]) + (copyThis.m_values[Tz] * appendThis.m_values[Iw]);  //myIz
	m_values[Iw] = (copyThis.m_values[Iw] * appendThis.m_values[Ix]) + (copyThis.m_values[Jw] * appendThis.m_values[Iy]) + (copyThis.m_values[Kw] * appendThis.m_values[Iz]) + (copyThis.m_values[Tw] * appendThis.m_values[Iw]);  //myIw
																																												
	m_values[Jx] = (copyThis.m_values[Ix] * appendThis.m_values[Jx]) + (copyThis.m_values[Jx] * appendThis.m_values[Jy]) + (copyThis.m_values[Kx] * appendThis.m_values[Jz]) + (copyThis.m_values[Tx] * appendThis.m_values[Jw]);  //myJx
	m_values[Jy] = (copyThis.m_values[Iy] * appendThis.m_values[Jx]) + (copyThis.m_values[Jy] * appendThis.m_values[Jy]) + (copyThis.m_values[Ky] * appendThis.m_values[Jz]) + (copyThis.m_values[Ty] * appendThis.m_values[Jw]);  //myJy
	m_values[Jz] = (copyThis.m_values[Iz] * appendThis.m_values[Jx]) + (copyThis.m_values[Jz] * appendThis.m_values[Jy]) + (copyThis.m_values[Kz] * appendThis.m_values[Jz]) + (copyThis.m_values[Tz] * appendThis.m_values[Jw]);  //myJz
	m_values[Jw] = (copyThis.m_values[Iw] * appendThis.m_values[Jx]) + (copyThis.m_values[Jw] * appendThis.m_values[Jy]) + (copyThis.m_values[Kw] * appendThis.m_values[Jz]) + (copyThis.m_values[Tw] * appendThis.m_values[Jw]);  //myJw
																																												
	m_values[Kx] = (copyThis.m_values[Ix] * appendThis.m_values[Kx]) + (copyThis.m_values[Jx] * appendThis.m_values[Ky]) + (copyThis.m_values[Kx] * appendThis.m_values[Kz]) + (copyThis.m_values[Tx] * appendThis.m_values[Kw]);  //myKx
	m_values[Ky] = (copyThis.m_values[Iy] * appendThis.m_values[Kx]) + (copyThis.m_values[Jy] * appendThis.m_values[Ky]) + (copyThis.m_values[Ky] * appendThis.m_values[Kz]) + (copyThis.m_values[Ty] * appendThis.m_values[Kw]);  //myKy
	m_values[Kz] = (copyThis.m_values[Iz] * appendThis.m_values[Kx]) + (copyThis.m_values[Jz] * appendThis.m_values[Ky]) + (copyThis.m_values[Kz] * appendThis.m_values[Kz]) + (copyThis.m_values[Tz] * appendThis.m_values[Kw]);  //myKz
	m_values[Kw] = (copyThis.m_values[Iw] * appendThis.m_values[Kx]) + (copyThis.m_values[Jw] * appendThis.m_values[Ky]) + (copyThis.m_values[Kw] * appendThis.m_values[Kz]) + (copyThis.m_values[Tw] * appendThis.m_values[Kw]);  //myKw
																																												
	m_values[Tx] = (copyThis.m_values[Ix] * appendThis.m_values[Tx]) + (copyThis.m_values[Jx] * appendThis.m_values[Ty]) + (copyThis.m_values[Kx] * appendThis.m_values[Tz]) + (copyThis.m_values[Tx] * appendThis.m_values[Tw]);  //myTx
	m_values[Ty] = (copyThis.m_values[Iy] * appendThis.m_values[Tx]) + (copyThis.m_values[Jy] * appendThis.m_values[Ty]) + (copyThis.m_values[Ky] * appendThis.m_values[Tz]) + (copyThis.m_values[Ty] * appendThis.m_values[Tw]);  //myTy
	m_values[Tz] = (copyThis.m_values[Iz] * appendThis.m_values[Tx]) + (copyThis.m_values[Jz] * appendThis.m_values[Ty]) + (copyThis.m_values[Kz] * appendThis.m_values[Tz]) + (copyThis.m_values[Tz] * appendThis.m_values[Tw]);  //myTz
	m_values[Tw] = (copyThis.m_values[Iw] * appendThis.m_values[Tx]) + (copyThis.m_values[Jw] * appendThis.m_values[Ty]) + (copyThis.m_values[Kw] * appendThis.m_values[Tz]) + (copyThis.m_values[Tw] * appendThis.m_values[Tw]);  //myTw
}

void Mat44::AppendZRotation(float degreesRotationAboutZ)
{
	Mat44 appendRotation = MakeZRotationDegrees(degreesRotationAboutZ);
	Append(appendRotation);
}

void Mat44::AppendYRotation(float degreesRotationAboutY)
{
	Mat44 appendRotation = MakeYRotationDegrees(degreesRotationAboutY);
	Append(appendRotation);
}

void Mat44::AppendXRotation(float degreesRotationAboutX)
{
	Mat44 appendRotation = MakeXRotationDegrees(degreesRotationAboutX);
	Append(appendRotation);
}

void Mat44::AppendTranslation2D(Vec2 const& translationXY)
{
	Mat44 appendTranslation = MakeTranslation2D(translationXY);
	Append(appendTranslation);
}

void Mat44::AppendTranslation3D(Vec3 const& translationXYZ)
{
	Mat44 appendTranslation = MakeTranslation3D(translationXYZ);
	Append(appendTranslation);
}

void Mat44::AppendScaleUniform2D(float uniformScaleXY)
{
	Mat44 appendScale = MakeUniformScale2D(uniformScaleXY);
	Append(appendScale);
}

void Mat44::AppendScaleUniform3D(float uniformScaleXYZ)
{
	Mat44 appendScale = MakeUniformScale3D(uniformScaleXYZ);
	Append(appendScale);
}

void Mat44::AppendScaleNonUniform2D(Vec2 const& nonUniformScaleXY)
{
	Mat44 appendScale = MakeNonUniformScale2D(nonUniformScaleXY);
	Append(appendScale);
}

void Mat44::AppendScaleNonUniform3D(Vec3 const& nonUniformScaleXYZ)
{
	Mat44 appendScale = MakeNonUniformScale3D(nonUniformScaleXYZ);
	Append(appendScale);
}
