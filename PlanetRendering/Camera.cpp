#include "Camera.h"

Camera::Camera(int windowWidth, int windowHeight) : 
	Position(1.1f, 0.0f, 0.0f),
	aspectRatio(((float)windowWidth)/(float)windowHeight),
	FieldOfView(85.0f), XRotation(0.0f), ZRotation(0.0f), YRotation(0.0f),
	NEAR_PLANE(0.000000001), FAR_PLANE(200.0f)
{
}

Camera::~Camera() {}

vmat4 Camera::GetViewMatrix()
{
	return glm::eulerAngleXZ(XRotation, ZRotation) * glm::translate(Position) * glm::scale(vvec3(1.0, 1.0, 1.0));
}

void Camera::ResizedWindow(int windowWidth, int windowHeight)
{
	//aspectRatio = ((float)windowWidth)/((float)windowHeight);
}

vmat4 Camera::GetProjectionMatrix()
{
	return glm::perspective(
		FieldOfView,
		aspectRatio,
		NEAR_PLANE,  //near clipping plane
		FAR_PLANE //far clipping plane
	);
}
vmat4 Camera::GetTransformMatrix()
{
    return glm::rotate(GetProjectionMatrix() , 0.0, vvec3(0,0,1))* GetViewMatrix();
}

vvec3 Camera::GetViewDirection()
{
    return vvec3(vvec4(0.0, 0.0, 1.0, 1.0) * glm::eulerAngleXZ(XRotation, ZRotation));
}
