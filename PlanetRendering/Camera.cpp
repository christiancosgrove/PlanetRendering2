#include "Camera.h"

Camera::Camera(int windowWidth, int windowHeight) : 
	Position(1.1f, 0.0f, 0.0f),
	aspectRatio(((float)windowWidth)/(float)windowHeight),
	FieldOfView(85.0f), XRotation(0.0f), ZRotation(0.0f),
	NEAR_PLANE(0.1f), FAR_PLANE(200.0f)
{
    aspectRatio = ((float)windowWidth)/(float)windowHeight;
}

Camera::~Camera() {}

glm::dmat4 Camera::GetViewMatrix()
{
	return glm::eulerAngleXZ(XRotation, ZRotation) * glm::translate(Position) * glm::scale(glm::dvec3(1.0, 1.0, 1.0));
}

void Camera::ResizedWindow(int windowWidth, int windowHeight)
{
	//aspectRatio = ((float)windowWidth)/((float)windowHeight);
}

glm::dmat4 Camera::GetProjectionMatrix()
{
	return glm::perspective(
		FieldOfView,
		1.333333,
		0.00000000000001,  //near clipping plane
		1000.0 //far clipping plane
	);
}
glm::dmat4 Camera::GetTransformMatrix()
{
	return GetProjectionMatrix() * GetViewMatrix();
}

glm::dvec3 Camera::GetViewDirection()
{
	return glm::dvec3(glm::dvec4(0.0, 0.0, 1.0, 1.0) * glm::eulerAngleXZ(XRotation, ZRotation));
}
