#ifndef TRIH
#define TRIH
#include "hitable.h"
#include "vec3.h"
#include "cs225/HSLAPixel.h"
#include "aabb.h"
#include <math.h>

using namespace cs225;

class Triangle : public hitable {
public:
  Triangle() { A = vec3(0, 0, 0); B = vec3(0, 0, 0); C = vec3(0, 0, 0); h = HSLAPixel(0,0,0,0); }
  Triangle(vec3 a, vec3 b, vec3 c, HSLAPixel hs) { A = a; B = b; C = c; h = hs; }
  virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
  virtual bool bounding_box(float t0, float t1, aabb& box) const;

  HSLAPixel getColor() const { return h; }

private:
  HSLAPixel h;
  vec3 A;
  vec3 B;
  vec3 C;
};


float retMin(float a, float b, float c) {
  float ret = a;
  if (b < ret)
    ret = b;
  if (c < ret)
    ret = c;
  return ret;
}

float retMax(float a, float b, float c) {
  float ret = a;
  if (b > ret)
    ret = b;
  if (c > ret)
    ret = c;
  return ret;
}

bool Triangle::bounding_box(float t0, float t1, aabb& box) const {
  float minx, miny, minz, maxx, maxy, maxz;
  minx = retMin(A.x(), B.x(), C.x());
  miny = retMin(A.y(), B.y(), C.y());
  minz = retMin(A.z(), B.z(), C.z());

  maxz = retMax(A.z(), B.z(), C.z());
  maxy = retMax(A.y(), B.y(), C.y());
  maxx = retMax(A.x(), B.x(), C.x());

  box._min = vec3(minx, miny, minz);
  box._max = vec3(maxx, maxy, maxz);

  if (box._max.x() - box._min.x() <= 0.01) {
    box._max[0] += 0.005;
    box._min[0] -= 0.005;
  }
  if (box._max.y() - box._min.y() <= 0.01) {
    box._max[1] += 0.005;
    box._min[1] -= 0.005;
  }
  if (box._max.z() - box._min.z() <= 0.01) {
    box._max[2] += 0.005;
    box._min[2] -= 0.005;
  }

  return true;
}


bool Triangle::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
  float epsilon = 0.001;

  float ABCArea = cross((B-A), (C-A)).length() / 2.0;

  vec3 normal = cross(A - B, C - B);

  float first = dot(A - r.origin(), normal);
  float second = dot(r.direction(), normal);

  float temp = first / second;

  if (temp <= t_min || temp > t_max)
    return false;

  vec3 P = r.point_at_parameter(temp);

  float ABP = cross((B-P), (A-P)).length() / 2.0;
  float BCP = cross((B-P), (C-P)).length() / 2.0;
  float ACP = cross((C-P), (A-P)).length() / 2.0;

  float comp = ABP + BCP + ACP - ABCArea;

  if (comp <= epsilon && comp >= -1 * epsilon) {
      rec.t = temp;
      rec.p = P;
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
