#ifndef CAMERAH
#define CAMERAH

#include "ray.h"
#include "vec3.h"
using namespace std;

//taken from "Ray Tracing in One Weekend" but edited for orthographic projections
class camera {
public:
  camera() {
    lower_left_corner = vec3(-4, -2, -1);
    horizontal = vec3(4, 0, 0);
    vertical = vec3(0, -0.1, 0);

    //origin = vec3(2, 4, 2); //teapot
    //origin = vec3(92.85, 43, 47); //pattern_egg
    origin = vec3(20, -33, 57);


    //dir = -1 * origin;
    dir = vec3(0, 1, -0.13); //scream
  }

  ray get_ray_pers(float u, float v) {
    vec3 horiz;

    if (dir.x() == 0)
      horiz = vec3(-1, 0, 0);
    else
      horiz = vec3(-dir.z() / dir.x(), 0, 1);

    horiz.make_unit_vector();
    horiz *= 2;

    if ((horiz.x() > 0 && dir.z() < 0) || (horiz.x() < 0 && dir.z() > 0))
      horiz *= -1;

    vec3 vert(cross(horiz, dir));
    vert.make_unit_vector();

    if (vert.y() > 0)
      vert *= -1;

    dir.make_unit_vector();

    vec3 llc(horiz + vert + dir);

    vec3 fin(llc - u*horiz*2 - v*vert*2);
    fin *= vec3(1, 1, 1);
    return ray(origin, fin);
  }
  ray get_ray_ortho(float u, float v) {
    return ray((lower_left_corner + u*horizontal + v*vertical - origin), dir);
  }
  vec3 origin;
  vec3 lower_left_corner;
  vec3 horizontal;
  vec3 vertical;
  vec3 dir;
};

#endif
