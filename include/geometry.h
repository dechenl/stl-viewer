#ifndef GEOMETRY_H
#define GEOMETRY_H

#define PI_F 3.14159f

#include "geometry/vec2.h"
#include "geometry/vec3.h"
#include "geometry/vec4.h"
#include "geometry/line2d.h"
#include "geometry/line3d.h"
#include "geometry/triangle.h"
#include "geometry/mat4.h"
#include "geometry/quaternion.h"

// angle conversion
float rad(float deg);
float deg(float rad);

// vec3 rotation
vec2 rotate(const vec2 &, float);
void rotatexz(vec3 &, float);
void rotateyz(vec3 &, float);

// barycentric
float area(const vec2 &, const vec2 &, const vec2 &);
vec3 bary(const vec2 &, const vec2 &, const vec2 &, const vec2 &);
vec2 unbary(const vec2 &, const vec2 &, const vec2 &, const vec3 &);
vec3 unbary(const vec3 &, const vec3 &, const vec3 &, const vec3 &);

#endif // GEOMETRY_H
