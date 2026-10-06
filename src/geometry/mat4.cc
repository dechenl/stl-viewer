#include "geometry/mat4.h"

vec4
mat4::operator*(const vec4 &v) const
{
  return {
    x0 * v.x + x1 * v.y + x2 * v.z + x3 * v.w,
    y0 * v.x + y1 * v.y + y2 * v.z + y3 * v.w,
    z0 * v.x + z1 * v.y + z2 * v.z + z3 * v.w,
    w0 * v.x + w1 * v.y + w2 * v.z + w3 * v.w,
  };
}

mat4
mat4::operator*(const mat4 &o) const
{
  const mat4 &s = *this;
  return {
    s.x0 * o.x0 + s.x1 * o.y0 + s.x2 * o.z0 + s.x3 * o.w0,
    s.x0 * o.x1 + s.x1 * o.y1 + s.x2 * o.z1 + s.x3 * o.w1,
    s.x0 * o.x2 + s.x1 * o.y2 + s.x2 * o.z2 + s.x3 * o.w2,
    s.x0 * o.x3 + s.x1 * o.y3 + s.x2 * o.z3 + s.x3 * o.w3,

    s.y0 * o.x0 + s.y1 * o.y0 + s.y2 * o.z0 + s.y3 * o.w0,
    s.y0 * o.x1 + s.y1 * o.y1 + s.y2 * o.z1 + s.y3 * o.w1,
    s.y0 * o.x2 + s.y1 * o.y2 + s.y2 * o.z2 + s.y3 * o.w2,
    s.y0 * o.x3 + s.y1 * o.y3 + s.y2 * o.z3 + s.y3 * o.w3,

    s.z0 * o.x0 + s.z1 * o.y0 + s.z2 * o.z0 + s.z3 * o.w0,
    s.z0 * o.x1 + s.z1 * o.y1 + s.z2 * o.z1 + s.z3 * o.w1,
    s.z0 * o.x2 + s.z1 * o.y2 + s.z2 * o.z2 + s.z3 * o.w2,
    s.z0 * o.x3 + s.z1 * o.y3 + s.z2 * o.z3 + s.z3 * o.w3,

    s.w0 * o.x0 + s.w1 * o.y0 + s.w2 * o.z0 + s.w3 * o.w0,
    s.w0 * o.x1 + s.w1 * o.y1 + s.w2 * o.z1 + s.w3 * o.w1,
    s.w0 * o.x2 + s.w1 * o.y2 + s.w2 * o.z2 + s.w3 * o.w2,
    s.w0 * o.x3 + s.w1 * o.y3 + s.w2 * o.z3 + s.w3 * o.w3,
  };
}
