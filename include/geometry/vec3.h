#ifndef VEC3_H
#define VEC3_H

#include <string>

struct vec3
{
  float x,y,z;

  vec3 operator+(const vec3 &) const;
  vec3 operator-(const vec3 &) const;
  vec3 operator*(float) const;
  vec3 operator/(float) const;

  bool operator==(const vec3 &o) const;

  float mag() const;
  float dot(const vec3 &) const;
  vec3 cross(const vec3 &) const;

  std::string str() const;
};

#endif // VEC3_H
