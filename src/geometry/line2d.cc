#include "geometry/line2d.h"

line2d::line2d(const vec2 &p1, const vec2 &p2) :
  m_( (p1.y - p2.y) / (p1.x - p2.x) ),
  b_( p1.y - (p1.y - p2.y) / (p1.x - p2.x) * p1.x )
{
}

float
line2d::m() const
{
  return m_;
}

float
line2d::atX(float x) const
{
  return m_ * x + b_;
}

float
line2d::atY(float y) const
{
  return (y - b_) / m_;
}

vec2
line2d::intersect(const line2d &l2) const
{
  const line2d &l1 = *this;
  return {
    (l1.b_ - l2.b_) / (l2.m_ - l1.m_),
    (l2.m_*l1.b_ - l1.m_*l2.b_) / (l2.m_ - l1.m_),
  };
}
