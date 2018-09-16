#ifndef HITABLE_H
#define HITABLE_H

#include "vec3.h"
#include "cs225/HSLAPixel.h"
#include "ray.h"
#include "aabb.h"

//from "Ray Tracing in One Weekend" modified with HSLAPixel
struct hit_record {
  float t;
  vec3 p;
  vec3 normal;
  cs225::HSLAPixel h;
};

class hitable {
public:
  virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
  virtual bool bounding_box(float t0, float t1, aabb& box) const = 0;

};

#endif
