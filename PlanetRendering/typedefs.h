#pragma once

#define VERTEX_DOUBLE
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