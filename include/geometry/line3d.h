#ifndef LINE3D_H
#define LINE3D_H

#include "vec3.h"

class line3d
{
public:
  line3d(const vec3 &, const vec3 &);

  vec3 intersect(const line3d &) const;

private:
  vec3 p_, d_;
};

#endif // LINE3D_H
