#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "typedefs.h"
//This class provides a representation of the camera configuration
//Contains position
//Rotation of camera stored as Euler angles -- not ideal


class Camera
{
public:
	Camera(int windowWidth, int windowHeight);
    ~Camera();
    //Rotation and translation aspect of camera
	vmat4 GetViewMatrix();
    //Adds perspective
	vmat4 GetProjectionMatrix();
    //Combination of GetViewMatrix() and GetProjectionMatrix()
	vmat4 GetTransformMatrix();
    //Used to calculate movement
    vvec3 GetViewDirection();
    
    inline vvec3 GetPosition();
    vvec3 position;
    //XYZ Euler angles -- Y rotation currently not included
	vfloat XRotation;
	vfloat ZRotation;
    vfloat YRotation;
    vfloat PlanetRotation;
    //FOV represents the angle the bounds of the camera's view subdtends in the scene
	vfloat FieldOfView;
    //Currently does nothing
    void ResizedWindow(int windowWidth, int windowHeight);
    //near clipping plane - all geometry CLOSER to camera than this value is invisible
    //It is possible to set this to 0 (allowing theoretically infinite zoom), but it causes problems with the depth buffer (polygons are not sorted properly on screen)
    //If I add a logarithmic depth buffer (which is ideal for large expanses where drawn geometry vary many orders of magnitude insize), the limits on this value may increase
    const vfloat NEAR_PLANE;
    //far clipping plane - all geometry FARTHER to camera from this value is invisible
	const vfloat FAR_PLANE;
private:
    //calculated from window width&height
	vfloat aspectRatio;
};

vvec3 Camera::GetPosition() {
    return vmat3(glm::rotate(vmat4(), -PlanetRotation, vvec3(0.0,0.0,1.0)))*position;
}

