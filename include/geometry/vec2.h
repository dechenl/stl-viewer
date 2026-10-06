#ifndef VEC2_H
#define VEC2_H

#include <string>

struct vec2
{
  float x,y;
  float mag() const;
  vec2 operator-(const vec2 &o) const;
  bool operator==(const vec2 &o) const;
  std::string str() const;
};

#endif // VEC2_H
