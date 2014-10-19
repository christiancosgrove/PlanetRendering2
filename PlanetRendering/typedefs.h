#pragma once

//This file allows one to easily switch between double and single precision for vertex data
//Just include or remove #define VERTEX_DOUBLE. Note: because GPUs are heavily optimized for them, single precision floating point numbers are significantly more efficient.  Double precision numbers are used when a large amount of detail is needed.
//#define VERTEX_DOUBLE
#ifdef VERTEX_DOUBLE

typedef double vfloat;
typedef glm::dvec3 vvec3;
typedef glm::dvec4 vvec4;
typedef glm::dmat4 vmat4;
typedef glm::dmat3 vmat3;
typedef glm::dvec2 vvec2;

#else
typedef float vfloat;
typedef glm::vec3 vvec3;
typedef glm::vec4 vvec4;
typedef glm::mat4 vmat4;
typedef glm::mat3 vmat3;
typedef glm::vec2 vvec2;

#endif