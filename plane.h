#ifndef PLANEH
#define PLANEH
#include "hitable.h"
#include "vec3.h"
#include "cs225/HSLAPixel.h"
#include "aabb.h"
#include <math.h>

using namespace cs225;

class Plane : public hitable {
public:
  Plane() { }
  Plane(float a, float b, float c, float d, HSLAPixel hs) { A = a; B = b; C = c; D = d; h = hs; }
  virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
  virtual bool bounding_box(float t0, float t1, aabb& box) const;

  HSLAPixel getColor() const { return h; }

private:
  HSLAPixel h;
  float A, B, C, D;
};

bool Plane::bounding_box(float t0, float t1, aabb& box) const {
  return false;
}

bool Plane::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
  vec3 normal = vec3(A, B, C);
  vec3 point;

  if (A != 0)
    point = vec3((-D) / A, 0, 0);
  else if (B != 0)
    point = vec3(0, (-D) / B, 0);
  else if (C != 0)
    point = vec3(0, 0, (-D) / C);
  else
    return false;

  float first = dot(point - r.origin(), normal);
  float second = dot(r.direction(), normal);

  float temp = first / second;

  if (temp < t_max && temp > t_min) {
    rec.t = temp;
    rec.p = r.point_at_parameter(rec.t);
    if (dot(normal, r.direction()) > 0)
      rec.normal = normal * -1;
    else
      rec.normal = normal;
    rec.h = h;
    return true;
  }

  return false;
}

#endif
