#include "Camera.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Window/Window.hpp"

extern Window* g_theWindow;

Camera::Camera()
{
	m_position = Vec3(0.0f, 0.0f, 0.0f);
	m_orientation = EulerAngles(0.0f, 0.0f, 0.0f);
	m_viewPort = AABB2(0.f, 0.f, G_SCREEN_SIZE_X, G_SCREEN_SIZE_Y);

	m_orthographicBottomLeft = Vec2(0.0f, 0.0f);
	m_orthographicTopRight = Vec2(0.0f, 0.0f);
	m_orthographicNear = 0.0f;
	m_orthographicFar = 1.0f;

	m_perspectiveAspect = 0.0f;
	m_perspectiveFOV = 0.0f;
	m_perspectiveNear = 0.0f;
	m_perspectiveFar = 0.0f;

	m_cameraToRenderTransform = Mat44();
}

void Camera::SetOrthographicView(Vec2 const& bottomLeft, Vec2 const& topRight, float near, float far)
{
	m_orthographicBottomLeft = bottomLeft;
	m_orthographicTopRight = topRight;
	m_orthographicNear = near;
	m_orthographicFar = far;

	m_mode = Mode::eMode_Orthographic;
}

void Camera::SetPerspectiveView(float aspect, float fov, float near, float far)
{
	m_perspectiveAspect = aspect;
	m_perspectiveFOV = fov;
	m_perspectiveNear = near;
	m_perspectiveFar = far;

	m_mode = Mode::eMode_Perspective;
}

void Camera::SetViewPort(AABB2 const& screenViewZone)
{
	m_viewPort = screenViewZone;
}

bool Camera::IsOrthoMode() const
{
	if (m_mode == eMode_Orthographic)
		return true;
	else
		return false;
}

void Camera::SetPositionAndOrientation(const Vec3& position, const EulerAngles& orientation)
{
	m_position = position;
	m_orientation = orientation;
}

void Camera::SetPosition(const Vec3& position)
{
	m_position = position;
}

Vec3 Camera::GetPosition() const
{
	return m_position;
}

void Camera::SetOrientation(const EulerAngles& orientation)
{
	//Todo: Investigate this behavior. Inverse Pitching
	m_orientation = orientation;
}

EulerAngles Camera::GetOrientation() const
{
	return m_orientation;
}

Mat44 Camera::GetCameraToWorldTransform() const
{
	Mat44 rotationMatrix = m_orientation.GetAsMatrix_IFwd_JLeft_KUp();
	Mat44 translationMatrix = Mat44::MakeTranslation3D(m_position);
	translationMatrix.Append(rotationMatrix); //make [T][R] = [TR]

	return translationMatrix;
}

Mat44 Camera::GetWorldToCameraTransform() const
{
	Mat44 rotationMatrix = m_orientation.GetAsMatrix_IFwd_JLeft_KUp();
 	Mat44 translationMatrix = Mat44::MakeTranslation3D(m_position);
	translationMatrix.Append(rotationMatrix);
	Mat44 inverseCameraTransforms = translationMatrix.GetOrthonormalInverse(); //make [TR]**-1

	return inverseCameraTransforms;
}

void Camera::SetCameraToRenderTransform(const Mat44& m)
{
	m_cameraToRenderTransform = m;
}

Mat44 Camera::GetCameraToRenderTransform() const
{
	return m_cameraToRenderTransform;
}

void Camera::SetModelToWorldTransform(const Mat44& m)
{
	m_position = m.GetTranslation3D();
}

Mat44 Camera::GetModelToWorldTransform() const
{
	Mat44 modelToWorld = Mat44::MakeTranslation3D(m_position);
	modelToWorld.Append(m_orientation.GetAsMatrix_IFwd_JLeft_KUp());
	return modelToWorld;
}

Vec2 Camera::GetOrthographicBottomLeft() const
{
	return m_orthographicBottomLeft;
}

Vec2 Camera::GetOrthographicTopRight() const
{
	return m_orthographicTopRight;
}

void Camera::Translate2D(const Vec2& translation2D)
{
	m_orthographicBottomLeft += translation2D;
	m_orthographicTopRight += translation2D;
}

AABB2 Camera::GetViewPort() const
{
	return m_viewPort;
}

Mat44 Camera::GetRenderToClipTransform() const
{
	return GetProjectionMatrix();
}

Mat44 Camera::GetOrthographicMatrix() const
{
	return Mat44::MakeOrthoProjection(m_orthographicBottomLeft.x, m_orthographicTopRight.x, 
									  m_orthographicBottomLeft.y, m_orthographicTopRight.y,
									  m_orthographicNear, m_orthographicFar);
}

Mat44 Camera::GetPerspectiveMatrix() const
{
	return Mat44::MakePerspectiveProjection(m_perspectiveFOV, m_perspectiveAspect, m_perspectiveNear, m_perspectiveFar);
}

Mat44 Camera::GetProjectionMatrix() const
{
	switch (m_mode)
	{
	case eMode_Orthographic:
		return GetOrthographicMatrix();
		break;
	case eMode_Perspective:
		return GetPerspectiveMatrix();
		break;
	}
	ERROR_AND_DIE("Invalid Projection Mode");
}