#include "geometry/line3d.h"

line3d::line3d(const vec3 &p1, const vec3 &p2) :
  p_(p1), d_(p2-p1)
{
}

vec3
line3d::intersect(const line3d &other) const
{
  float t,s;
  if (d_.x * other.d_.y != d_.y * other.d_.x) {
    t = ( other.d_.x * (other.p_.y - p_.y) - other.d_.y * (other.p_.x - p_.x) ) /
        (d_.y * other.d_.x - d_.x * other.d_.y);
    s = ( d_.x * (p_.y - other.p_.y) - d_.y * (p_.x - other.p_.x) ) /
        (d_.x * other.d_.y - d_.y * other.d_.x);
  } else if (d_.y * other.d_.z != d_.z * other.d_.z) {
    t = ( other.d_.z * (other.p_.y - p_.y) - other.d_.y * (other.p_.z - p_.z) ) /
        (d_.y * other.d_.z - d_.z * other.d_.y);
    s = ( d_.z * (p_.y - other.p_.y) - d_.y * (p_.z - other.p_.z) ) /
        (d_.z * other.d_.y - d_.y * other.d_.z);
  } else {
    t = ( other.d_.z * (other.p_.x - p_.x) - other.d_.x * (other.p_.z - p_.z) ) /
        (d_.x * other.d_.z - d_.z * other.d_.x);
    s = ( d_.z * (p_.x - other.p_.x) - d_.x * (p_.z - other.p_.z) ) /
        (d_.z * other.d_.x - d_.x * other.d_.z);
  }

  return {
    p_.x + d_.x * t,
    p_.y + d_.y * t,
    p_.z + d_.z * t,
  };
}
