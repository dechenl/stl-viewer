#include "geometry/triangle.h"

triangle::triangle() :
  a({0}), b({0}), c({0})
{
}

triangle::triangle(const vec3 &v1, const vec3 &v2, const vec3 &v3) :
  a(v1), b(v2), c(v3)
{
}

const vec3 &
triangle::operator[](int i) const
{
  i = i % 3;
  if (i == 1) return b;
  else if (i == 2) return c;
  return a;
}

vec3 &
triangle::operator[](int i)
{
  return const_cast<vec3&>(static_cast<const triangle&>(*this)[i]);
}

vec3
triangle::normal() const
{
  vec3 v1 = b - a;
  vec3 v2 = c - a;
  return v1.cross(v2);
}

vec3
triangle::center() const
{
  return (a + b + c) / 3.0f;
}

bool
triangle::on_plane(const vec3 &p) const
{
  auto norm = normal();
  return norm.dot(p) == 0;
}

vec3 *
triangle::begin()
{
  return &a;
}

vec3 *
triangle::end()
{
  return &c +1;
}

const vec3 *
triangle::begin() const
{
  return &a;
}

const vec3 *
triangle::end() const
{
  return &c +1;
}
