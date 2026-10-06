#include "geometry/vec4.h"

#include <sstream>

vec4::vec4(float x_, float y_, float z_, float w_) :
  x(x_), y(y_), z(z_), w(w_)
{
}

vec4::vec4(const vec3 &v, float w_) :
  x(v.x), y(v.y), z(v.z), w(w_)
{
}

std::string
vec4::str() const
{
  std::stringstream ss;
  ss << "(" << x
     << "," << y
     << "," << z
     << "," << w
     << ")";
  return ss.str();
}
