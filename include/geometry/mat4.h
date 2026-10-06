#ifndef MAT4_H
#define MAT4_H

#include "vec4.h"

struct mat4
{
  float x0,x1,x2,x3;
  float y0,y1,y2,y3;
  float z0,z1,z2,z3;
  float w0,w1,w2,w3;

  vec4 operator*(const vec4 &v) const;
  mat4 operator*(const mat4 &o) const;
};

#endif // MAT4_H
