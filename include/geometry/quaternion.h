#ifndef QUATERNION_H
#define QUATERNION_H

#include "mat4.h"

#include <string>

struct quaternion
{
  float w,x,y,z;

  // constructor
  quaternion(float, float, float, float); // from w,x,y,z
  quaternion(float, float, float);        // from angle

  quaternion operator*(const quaternion &) const;
  quaternion operator/(float) const;
  mat4 mat4() const;
  quaternion inverse() const;
  float mag() const;

  std::string str() const;
};

#endif // QUATERNION_H
