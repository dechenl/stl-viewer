#ifndef LINE2D_H
#define LINE2D_H

#include "vec2.h"

class line2d
{
private:
  float m_, b_;
public:
  line2d(const vec2 &p1, const vec2 &p2);
  float m() const;
  float atX(float x) const;
  float atY(float y) const;
  vec2 intersect(const line2d &l2) const;
};

#endif // LINE2D_H
