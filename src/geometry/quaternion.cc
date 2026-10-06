#include "geometry/quaternion.h"

#include <cmath>
#include <sstream>

quaternion::quaternion(float w_, float x_, float y_, float z_) :
  w(w_), x(x_), y(y_), z(z_)
{
}

quaternion::quaternion(float rx, float ry, float rz) :
  w(std::cos(rx/2)*std::cos(ry/2)*std::cos(rz/2) + std::sin(rx/2)*std::sin(ry/2)*std::sin(rz/2)),
  x(std::sin(rx/2)*std::cos(ry/2)*std::cos(rz/2) - std::cos(rx/2)*std::sin(ry/2)*std::sin(rz/2)),
  y(std::cos(rx/2)*std::sin(ry/2)*std::cos(rz/2) + std::sin(rx/2)*std::cos(ry/2)*std::sin(rz/2)),
  z(std::cos(rx/2)*std::cos(ry/2)*std::sin(rz/2) - std::sin(rx/2)*std::sin(ry/2)*std::cos(rz/2))
{
}

quaternion
quaternion::operator*(const quaternion &o) const
{
  const quaternion &s = *this;
  return {
    s.w*o.w - s.x*o.x - s.y*o.y - s.z*o.z,
    s.w*o.x + s.x*o.w + s.y*o.z - s.z*o.y,
    s.w*o.y - s.x*o.z + s.y*o.w + s.z*o.x,
    s.w*o.z + s.x*o.y - s.y*o.x + s.z*o.w,
  };
}

quaternion
quaternion::operator/(float s) const
{
  return { w / s, x / s, y / s, z / s, };
}

mat4
quaternion::mat4() const
{
  return {
    1 - 2*(y*y + z*z), 2*(x*y - w*z),     2*(x*z + w*y),     0,
    2*(x*y + w*z),     1 - 2*(x*x + z*z), 2*(y*z - w*x),     0,
    2*(x*z - w*y),     2*(y*z + w*x),     1 - 2*(x*x + y*y), 0,
    0,                 0,                 0,                 1
  };
}

quaternion
quaternion::inverse() const
{
  return quaternion(w, -x, -y, -z) / (w*w + x*x + y*y + z*z);
}

float
quaternion::mag() const
{
  return std::sqrt(w*w + x*x + y*y + z*z);
}

std::string
quaternion::str() const
{
  std::stringstream ss;
  ss << "(" << w
     << "," << x
     << "," << y
     << "," << z
     << ")";
  return ss.str();
}
