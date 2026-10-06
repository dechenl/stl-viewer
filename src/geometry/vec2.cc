#include "geometry/vec2.h"

#include <sstream>

vec2
vec2::operator-(const vec2 &o) const
{
  return vec2{ x - o.x, y - o.y };
}

bool
vec2::operator==(const vec2 &o) const
{
  return ( x == o.x && y == o.y );
}

std::string
vec2::str() const
{
  std::stringstream ss;
  ss << "(" << x
     << "," << y
     << ")";
  return ss.str();
}
