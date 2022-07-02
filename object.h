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
     bool rin_front;
     optics* material;
};


class optics {
public:
     // describes how light will be changed when hit an object
     vec3 color;
     bool do_reflect;
     bool do_refract;
     bool is_light;
     double scatter_rate;
     double refractive_index;

     optics() {}
     optics(vec3 c, double sr=1, bool rf=false, bool rfr=false, double ri=0, bool il=false) {
          color = c; scatter_rate = sr; 
          do_reflect = rf; do_refract = rfr; refractive_index = ri;
          is_light = il;
     }

     int reflect_ray(const ray& rin, ray& rout, const record& rec, vec3& rcolor) const {
          vec3 scatter_dir;
          if (is_light) {
               rcolor = color;
               return 2;
          }
          if (do_refract) {
               double ri = refractive_index;
               if (rec.rin_front) {
                    ri = 1.0/ri;
               }
               double cosx = dot(-rin.dir, rec.normal);
               if (cosx > 1) {
                    cosx = 1;
               }
               double sinx = sqrt(1 - cosx*cosx);
               double reflect_ratio;
               if (ri*sinx > 1) {
                    reflect_ratio = 1.001;
               }
               else {
                    // reflect_ratio = 0.1;
                    double a = (1-ri)*(1-ri)*(1+ri)*(1+ri);
                    reflect_ratio = a + (1-a)*pow(1-cosx, 5);
               }
               vec3 rout_dir;
               if (random_in_one() < reflect_ratio) {
                    rout_dir = reflect(rin.dir, rec.normal);
               }
               else {
                    rout_dir = refract(rin.dir, rec.normal, ri);                 
               }
               rout = ray(rec.hit_point, rout_dir);
               rcolor = color;
               return 1;
          }
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
          if (rt) {return 1;}
          return 0;          
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
          // use the nearest, legal root
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
               rec.rin_front = true;
               rec.normal = normal;
          }
          else {
               rec.rin_front = false;
               rec.normal = -normal;
          }
          rec.material = (optics *)&material;
          return true;
     }
};


class world {
public:
     std::vector<ball> objects;
     vec3 sky_color;

     world() {}

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
          int rflag = rec.material->reflect_ray(r, rout, rec, rcolor);
          if (rflag == 1) {
               return rcolor * calculate_color(rout, w, depth-1);
          }
          if (rflag == 2) {
               return rcolor;
          }
          return vec3(0, 0, 0);
     }

     // does not hit anything
     return w.sky_color;
}


#endif