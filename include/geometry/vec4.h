#ifndef VEC4_H
#define VEC4_H

#include <string>

#include "vec3.h"

struct vec4
{
  float x,y,z,w;

  // constructor
  vec4(float, float, float, float);
  vec4(const vec3 &, float);

  std::string str() const;
};

#endif // VEC4_H
