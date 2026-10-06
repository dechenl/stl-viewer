#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vec3.h"

struct triangle
{
  vec3 a,b,c;

  // constructor
  triangle();
  triangle(const vec3 &, const vec3 &, const vec3 &);

  // access
  const vec3& operator[](int) const;
  vec3& operator[](int);

  // prop
  vec3 normal() const;
  vec3 center() const;
  bool on_plane(const vec3 &) const;

  // iterator
  vec3 *begin();
  vec3 *end();

  const vec3 *begin() const;
  const vec3 *end() const;
};

#endif // TRIANGLE_H
