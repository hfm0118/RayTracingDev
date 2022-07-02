#ifndef OBJECT_H
#define OBJECT_H

#include <cmath>
#include <iostream>
#include <vector>
#include "vec3.h"
#include "ray.h"
#include "utilities.h"


class record;
class optics;


class record {
public:
     vec3 hit_point;
     vec3 normal;
     double t;
     optics* material;
};


class optics {
public:
     // describes how light will be changed when hit an object
     vec3 color;
     bool do_reflect;
     double scatter_rate;

     optics() {}
     optics(vec3 c, double sr=1, bool rf=false) {
          color = c; do_reflect = rf; scatter_rate = sr;
     }

     bool reflect_ray(const ray& rin, ray& rout, const record& rec, vec3& rcolor) const {
          vec3 scatter_dir;
          if (do_reflect) {
               vec3 reflect_dir = reflect(rin.dir, rec.normal);
               scatter_dir = normalize(reflect_dir + random3d()*scatter_rate);               
          }
          else {
               scatter_dir = normalize(rec.normal + normalize(random3d()*scatter_rate));          
          }
          rout = ray(rec.hit_point, scatter_dir);
          rcolor = color;
          bool rt = (dot(scatter_dir, rec.normal) > 0);
          return rt;          
     };
};


class object {
public:
     vec3 center;
     double radius;
     optics material;
     virtual bool hit(const ray& r, record& rec, double tmax = 1e100) const {};
};


class ball: public object {
public:
     vec3 center;
     double radius;
     optics material;

     ball(vec3 c, double r, optics m) {
          object();
          center = c; radius = r; material = m;
     }

     virtual bool hit(const ray& r, record& rec, double tmax = 1e100) const {
          // root = (-b+-sqrt(b^2-4ac))/(2a)
          vec3 dist = r.origin - center;
          double a = r.dir.l2norm();
          double b = 2*dot(dist, r.dir);
          double c = dist.l2norm() - radius*radius;
          double d = b*b - 4*a*c;
          if (d < 0) {
               return false;
          }
          double root = (-b-sqrt(d)) / (2*a);
          if (root<0.0001 || root>tmax) {
               root = (-b+sqrt(d)) / (2*a);
               if (root<0.0001 || root>tmax) {
                    return false;
               }
          }

          rec.t = root;
          rec.hit_point = r.value(root);
          vec3 normal = (rec.hit_point-center)/radius;
          if (dot(r.dir, normal) < 0) {
               rec.normal = normal;
          }
          else {
               rec.normal = -normal;
          }
          rec.material = (optics *)&material;
          return true;
     }
};


class world {
public:
     std::vector<ball> objects;

     world() {}

     world(std::string type) {
          if (type == "simple") {
               optics ground_material(vec3(0.5, 0.5, 0.5));
               ball ground(vec3(0, -1000, 0), 1000, ground_material);
               objects.push_back(ground);
               optics mirror_material(vec3(0.8, 0.8, 0.8), 0, true);
               ball mirror(vec3(0,1,0), 1, mirror_material);
               objects.push_back(mirror);
               optics ball_material(vec3(0.5, 0.6, 0.7));
               ball reference(vec3(3, 0.3, 0), 0.3, ball_material);
               objects.push_back(reference); 
          }
     }

     bool hit_something(const ray& r, record& rec, double tmax = 1e100) const {
          bool hit_flag = false;
          double _tmax = tmax;
          record _rec;

          for (int i=0; i<objects.size(); ++i) {
               bool flag = objects[i].hit(r, _rec, _tmax);
               if (flag) {
                    hit_flag = true;
                    rec = _rec;
                    _tmax = _rec.t;
               }
          }

          return hit_flag;
     }
};


vec3 calculate_color(const ray& r, const world& w, int depth) {
     if (depth < 1) {
          // end of recursion
          return vec3(0, 0, 0);
     }

     record rec;
     if (w.hit_something(r, rec)) {
          ray rout;
          vec3 rcolor;
          bool rflag = rec.material->reflect_ray(r, rout, rec, rcolor);
          if (rflag) {
               return rcolor * calculate_color(rout, w, depth-1);
          }
          return vec3(0, 0, 0);
     }

     vec3 sky_color(0.8, 0.9, 1.0);
     return sky_color;
}


#endif