#ifndef SPHEREH
#define SPHEREH
#include "hitable.h"
#include "vec3.h"
#include "cs225/HSLAPixel.h"
#include "aabb.h"
#include <math.h>

using namespace cs225;

//taken from "Ray Tracing in One Weekend" with a few modifications to work with HSLAPixel
//and a few other shading modifications
class Sphere : public hitable {
public:
  Sphere() { center = vec3(0.0, 0.0, 0.0); radius = 0.5; h = HSLAPixel(0,0,0,0); }
  Sphere(vec3 ce, float rad, HSLAPixel hs) { center = ce; radius = rad; h = hs; }
  virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
  virtual bool bounding_box(float t0, float t1, aabb& box) const;

  vec3 getCenter() const { return center; }
  float getRadius() const { return radius; }
  HSLAPixel getColor() const { return h; }

private:
  HSLAPixel h;
  vec3 center;
  float radius;
};

bool Sphere::bounding_box(float t0, float t1, aabb& box) const {
  box = aabb(center - vec3(radius, radius, radius),
             center + vec3(radius, radius, radius));
  return true;
}

bool Sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
  vec3 oc = r.origin() - center;
  float a = dot(r.direction(), r.direction());
  float b = 2.0 * dot(oc, r.direction());
  float c = dot(oc, oc) - radius * radius;
  float dis = b * b - 4 * a * c;

  if (dis > 0) {
    float temp = (-b - sqrt(b*b-4*a*c))/(2*a);
    if (temp < t_max && temp > t_min) {
      //std::cout << center << std::endl;
      rec.t = temp;
      rec.p = r.point_at_parameter(rec.t);
      rec.normal = (rec.p - center) / radius;
      rec.h = h;
      return true;
    }
    temp = (-b + sqrt(b*b-4*a*c))/(2*a);
    if (temp < t_max && temp > t_min) {
      rec.t = temp;
      rec.p = r.point_at_parameter(rec.t);
      rec.normal = (rec.p - center) / radius;
      rec.h = h;
      return true;
    }
  }
  return false;
}
#endif
