#include "geometry/vec3.h"

#include <sstream>

vec3
vec3::operator+(const vec3 &o) const
{
  return vec3{ x + o.x, y + o.y, z + o.z };
}

vec3
vec3::operator-(const vec3 &o) const
{
  return vec3{ x - o.x, y - o.y, z - o.z };
}

vec3
vec3::operator*(float s) const
{
  return vec3{ x * s, y * s, z * s };
}

vec3
vec3::operator/(float s) const
{
  return vec3{ x / s, y / s, z / s };
}

bool
vec3::operator==(const vec3 &o) const
{
  return ( x == o.x && y == o.y && z == o.z );
}

float
vec3::mag() const
{
  return std::sqrt(x*x + y*y + z*z);
}

float
vec3::dot(const vec3 &b) const
{
  const vec3 &a = *this;
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3
vec3::cross(const vec3 &b) const
{
  const vec3 &a = *this;
  return vec3{
    a.y * b.z - a.z * b.y,
    a.z * b.x - a.x * b.z,
    a.x * b.y - a.y * b.x,
  };
}

std::string
vec3::str() const
{
  std::stringstream ss;
  ss << "(" << x
     << "," << y
     << "," << z
     << ")";
  return ss.str();
}
