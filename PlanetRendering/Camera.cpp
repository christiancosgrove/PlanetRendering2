#include "Camera.h"

Camera::Camera(int windowWidth, int windowHeight) : 
	position(10.1f, 10.0f, 0.0f),
	aspectRatio(((float)windowWidth)/(float)windowHeight),
	FieldOfView(60), XRotation(0.0f), ZRotation(90.0f), YRotation(0.0f),PlanetRotation(0.0),
NEAR_PLANE(0),//std::numeric_limits<float>::epsilon()),
FAR_PLANE(2000.0f)
{
}

Camera::~Camera() {}

vmat4 Camera::GetViewMatrix()
{
    return glm::rotate(vmat4(), glm::degrees(XRotation), vvec3(1,0,0)) * glm::rotate(vmat4(), glm::degrees(ZRotation), vvec3(0,0,1)) * glm::translate(-position);
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
//    return GetViewMatrix();//
    return GetProjectionMatrix() * GetViewMatrix();
}

vvec3 Camera::GetViewDirection()
{
    return vvec3(vvec4(0.0, 0.0, 1.0, 1.0) * glm::eulerAngleXZ(XRotation, ZRotation));
}
