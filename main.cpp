#include <iostream>
#include <math.h>
#include <vector>
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "plane.h"
#include "triangle.h"
#include "hitable.h"
#include "hitablelist.h"
#include "bvh_node.h"
#include <time.h>
#include <fstream>
#include <sstream>

#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"
#include "camera.h"

using namespace std;
using namespace cs225;

vec3 light = vec3(0, 0.3, 1);
float lightStrength = 0.5;
float init = 0.4;
float maxt = 200;
float mint = 0.0;
HSLAPixel background = HSLAPixel(0, 0, 0.1, 1);

enum projType { ortho, pers };

HSLAPixel color(const ray& r, hitable *world) {
  hit_record rec;
  HSLAPixel pix = background;

  if (world->hit(r, mint, maxt, rec)) {
    pix = rec.h;

    //directional lighting
    vec3 v = vec3(rec.normal.x(), rec.normal.y(), rec.normal.z());
    v.make_unit_vector();
    light.make_unit_vector();

    float d = dot(v, -1 * light);
    if (d > 0) {

      d = acos(d);
      d -= 3.14159 / 2;

      d *= lightStrength;
      pix.l += d;

      //cout << d << endl;


      //shadow detection
      ray ra = ray(rec.p, light);
      hit_record temp;

      if (world->hit(ra, mint + 0.01, maxt, temp)) {
        pix.l -= 0.05;
      }

      //distance fading
      //float percFade = (maxt - rec.t) / maxt;
      //pix.l *= percFade;

      if (pix.l > 1)
        pix.l = 1;
      else if (pix.l < 0)
        pix.l = 0;
    }
  }


  return pix;
}

void draw(camera cam, hitable *world, int nx, int ny, int numRays, projType p) {

    PNG img = PNG(nx, ny);

    for (int j = ny-1; j >= 0; j--) {
      for (int i = 0; i < nx; i++) {
        vec3 col(0, 0, 0);
        for (int s = 0; s < numRays; s++) {
          //drand48() and how to anti-alias came from "Ray Tracing in One Weekend"
          float u = float(i + drand48()) / float(nx);
          float v = float(j + drand48()) / float(ny);
          ray r;
          if (p == pers)
            r = cam.get_ray_pers(u, v);
          else if (p == ortho)
            r = cam.get_ray_ortho(u, v);
          HSLAPixel t = color(r, world);
          col[0] += t.h;
          col[1] += t.s;
          col[2] += t.l;
        }
        col[0] /= float(numRays);
        col[1] /= float(numRays);
        col[2] /= float(numRays);

        HSLAPixel& pix = img.getPixel(i, ny - j - 1);
        pix.h = col[0];
        pix.s = col[1];
        pix.l = col[2];
      }
    }

    if (p == pers)
      img.writeToFile("output_pers.png");
    else if (p == ortho)
      img.writeToFile("output_ortho.png");
}

//parts of main class are from "Ray Tracing in One Weekend", including general layout ideas
//and names of some variables. this is done so I would not get confused with variable names
//when reading along in the book.
int main(int argc, char *argv[]) {

  int nx = 400;
  int ny = 200;
  int numRays = 1;

  if (string(argv[argc - 1]) == "-high_res") {
    nx = 1000;
    ny = 500;
    numRays = 7;
  }

  vector<vec3> vertices;

  if (argc < 2) {
    cout << "Invalid Format" << endl;
    return 0;
  }

  ifstream file(argv[1]);
  string line;
  float x, y, z, a = 0, b = 0, c = 0, total = 0;
  string s;

  if (file.is_open()) {
    cout << "Adding vertices..." << endl;
    while (getline(file, line)) {
      istringstream input(line);
      input >> s >> x >> y >> z;
      if (s == "v") {
        a += x;
        b += y;
        c += z;
        total++;
      }
      if (s == "f")
        break;
    }
  }

  a /= total;
  b /= total;
  c /= total;

  cout << "Center: " << a << " " << b << " " << c << endl;

  int sum = 1;

  if (file.is_open()) {
    while (getline(file, line)) {
      if (line.size() > 0 && line.at(0) == 'f')
        sum++;
    }
  }

  hitable *list[sum];

  file = ifstream(argv[1]);

  int place = 0;
  if (file.is_open()) {
    cout << "Adding faces..." << endl;
    while (getline(file, line)) {
        istringstream input(line);
        input >> s >> x >> y >> z;

        if (s == "v")
          vertices.push_back(vec3(x, y, z));
        else if (s == "f") {
          if (argc >= 3 && string(argv[2]) == "-o") {
            x = place * 3 + 1;
            y = x + 1;
            z = y + 1;
          }
          list[place] = new Triangle(vertices.at(x-1), vertices.at(y-1), vertices.at(z-1),
                                    HSLAPixel(190, 1, init, 1));
          place++;
        }

        s = "";
    }
  }
  file.close();

  cout << "Setting up camera...";
  camera cam;

  if (string(argv[argc - 2]) == "-c") {
    cam.dir = cam.origin - vec3(a, b, c);
    cam.dir *= -1;
  }

  cout << "Direction: " << cam.dir << endl;

  //BVH
  hitable *world = new bvh_node(list, sum, 0, 0);

  cout << "Rendering..." << endl;
  //LINEAR
  //hitable *world = new hitable_list(list, sum);
  draw(cam, world, nx, ny, numRays, pers);

  cout << "Done." << endl;


}
