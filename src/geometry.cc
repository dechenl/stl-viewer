#include "geometry.h"

#include <cmath>

// angle conversion
float rad(float deg)
{
  return deg * PI_F / 180.0f;
}

float deg(float rad)
{
  return rad / PI_F * 180.0f;
}

vec2 rotate(const vec2 &in, float deg)
{
    float r = rad(deg);
    return {
        in.x * std::cos(r) - in.y * std::sin(r),
        in.x * std::sin(r) + in.y * std::cos(r),
    };
}

void rotatexz(vec3 &vec, float deg)
{
    auto rotated_vec = rotate({vec.x, vec.z}, deg);
    vec.x = rotated_vec.x;
    vec.z = rotated_vec.y;
}

void rotateyz(vec3 &vec, float deg)
{
    auto rotated_vec = rotate({vec.y, vec.z}, deg);
    vec.y = rotated_vec.x;
    vec.z = rotated_vec.y;
}

// barycentric
float area(const vec2 &a, const vec2 &b, const vec2 &c)
{
  return 0.5 * std::abs(
           a.x * (b.y - c.y) + 
           b.x * (c.y - a.y) + 
           c.x * (a.y - b.y)
         );
}
vec3 bary(const vec2 &a, const vec2 &b, const vec2 &c, const vec2 &p)
{
  auto a1 = area(b,c,p),
       a2 = area(a,c,p),
       a3 = area(a,b,p),
       aa = area(a,b,c);
  return {a1/aa, a2/aa, a3/aa};
}
vec2 unbary(const vec2 &a, const vec2 &b, const vec2 &c, const vec3 &p)
{
  return {
    p.x * a.x + p.y * b.x + p.z * c.x,
    p.x * a.y + p.y * b.y + p.z * c.y,
  };
}
vec3 unbary(const vec3 &a, const vec3 &b, const vec3 &c, const vec3 &p)
{
  return {
    p.x * a.x + p.y * b.x + p.z * c.x,
    p.x * a.y + p.y * b.y + p.z * c.y,
    p.x * a.z + p.y * b.z + p.z * c.z,
  };
}

