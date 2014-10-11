#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/euler_angles.hpp"

class Camera
{
public:
	Camera(int windowWidth, int windowHeight);
    ~Camera();
	glm::dmat4 GetViewMatrix();
	glm::dmat4 GetProjectionMatrix();
	glm::dmat4 GetTransformMatrix();
	glm::dvec3 GetViewDirection();
	glm::dvec3 Position;
	double XRotation;
	double ZRotation;
	double FieldOfView;
	void ResizedWindow(int windowWidth, int windowHeight);
	const double NEAR_PLANE;
	const double FAR_PLANE;
private:
	float aspectRatio;
};

