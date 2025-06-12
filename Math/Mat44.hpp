#pragma once

struct Vec4;
struct Vec3;
struct Vec2;

struct Mat44
{
	enum { Ix,Iy,Iz,Iw,  Jx,Jy,Jz,Jw,  Kx,Ky,Kz,Kw,  Tx,Ty,Tz,Tw};
	float m_values[16];

	Mat44();
	explicit Mat44(Vec2 const& ibasis2D, Vec2 const& jbasis2D, Vec2 const& translation2D);
	explicit Mat44(Vec3 const& ibasis3D, Vec3 const& jbasis3D, Vec3 const& kbasis3D, Vec3 const& translation3D);
	explicit Mat44(Vec4 const& ibasis4D, Vec4 const& jbasis4D, Vec4 const& kbasis4D, Vec4 const& translation4D);
	explicit Mat44(const float* const sixteenValueBasisMajor);

	static Mat44 const MakeTranslation2D(Vec2 const& translationXY);
	static Mat44 const MakeTranslation3D(Vec3 const& translationXYZ);
	static Mat44 const MakeUniformScale2D(float uniformScaleXY);
	static Mat44 const MakeUniformScale3D(float uniformScaleXYZ);
	static Mat44 const MakeNonUniformScale2D(Vec2 const& nonUniformScaleXY);
	static Mat44 const MakeNonUniformScale3D(Vec3 const& nonUniformScaleXYZ);
	static Mat44 const MakeZRotationDegrees(float rotationDegreesAboutZ);
	static Mat44 const MakeYRotationDegrees(float rotationDegreesAboutY);
	static Mat44 const MakeXRotationDegrees(float rotationDegreesAboutX);
	static Mat44 const MakeOrthoProjection(float left, float right, float bottom, float top, float zNear, float zFar);
	static Mat44 const MakePerspectiveProjection(float fovYDegrees, float aspect, float zNear, float zFar);

	Vec2 const		TransformVectorQuantity2D(Vec2 const& vectorQuantityXY) const;
	Vec3 const		TransformVectorQuantity3D(Vec3 const& vectorQuantityXYZ) const;
	Vec2 const		TransformPosition2D(Vec2 const& positionXY) const;
	Vec3 const		TransformPosition3D(Vec3 const& positionXYZ) const;
	Vec4 const		TransformHomogeneous3D(Vec4 const& homogeneousPoint3D) const;

	float*			GetAsFloatArray();
	float const*	GetAsFloatArray() const;
	Vec2 const		GetIBasis2D() const;
	Vec2 const		GetJBasis2D() const;
	Vec2 const		GetTranslation2D() const;
	Vec3 const		GetIBasis3D() const;
	Vec3 const		GetJBasis3D() const;
	Vec3 const		GetKBasis3D() const;
	Vec3 const		GetTranslation3D() const;
	Vec4 const		GetIBasis4D() const;
	Vec4 const		GetJBasis4D() const;
	Vec4 const		GetKBasis4D() const;
	Vec4 const		GetTranslation4D() const;
	Mat44 const		GetOrthonormalInverse() const;

	void SetTranslation2D(Vec2 const& translationXY);
	void SetTranslation3D(Vec3 const& translationXYZ);
	void SetIJ2D(Vec2 const& iBasis2D, Vec2 const& jBasis2D);
	void SetIJT2D(Vec2 const& iBasis2D, Vec2 const& jBasis2D, Vec2 translationXY);
	void SetIJK3D(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D);
	void SetIJKT3D(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D, Vec3 const& translationXYZ);
	void SetIJKT4D(Vec4 const& iBasis4D, Vec4 const& jBasis4D, Vec4 const& kBasis4D, Vec4 const& translationXYZW);
	void Transpose();
	void Orthonormalize_IFwd_JLeft_KUp();

	void Append(Mat44 const& appendThis);
	void AppendZRotation(float degreesRotationAboutZ);
	void AppendYRotation(float degreesRotationAboutY);
	void AppendXRotation(float degreesRotationAboutX);
	void AppendTranslation2D(Vec2 const& translationXY);
	void AppendTranslation3D(Vec3 const& translationXYZ);
	void AppendScaleUniform2D(float uniformScaleXY);
	void AppendScaleUniform3D(float uniformScaleXYZ);
	void AppendScaleNonUniform2D(Vec2 const& nonUniformScaleXY);
	void AppendScaleNonUniform3D(Vec3 const& nonUniformScaleXYZ);
};