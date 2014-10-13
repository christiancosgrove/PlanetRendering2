#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "typedefs.h"
class Camera
{
public:
	Camera(int windowWidth, int windowHeight);
    ~Camera();
	vmat4 GetViewMatrix();
	vmat4 GetProjectionMatrix();
	vmat4 GetTransformMatrix();
	vvec3 GetViewDirection();
	vvec3 Position;
	vfloat XRotation;
	vfloat ZRotation;
    vfloat YRotation;
	vfloat FieldOfView;
	void ResizedWindow(int windowWidth, int windowHeight);
	const vfloat NEAR_PLANE;
	const vfloat FAR_PLANE;
private:
	vfloat aspectRatio;
};

