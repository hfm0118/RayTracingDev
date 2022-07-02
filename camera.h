#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "ray.h"
#include "utilities.h"

class camera {
public:
     vec3 position;
     // the origin point of camera system in ray_direction
     vec3 origin; 
     // the base vector of horizonal, vertical direction of camera system in ray_direction
     vec3 base_x, base_y;
     vec3 ux, uy;
     double lens; // unused

     camera() {}
     camera(vec3 pos, vec3 lookat, vec3 up,
          double viewport_x, double viewport_y, double focus, double l) {
          position = pos;
          vec3 lookdir = normalize(position-lookat);
          ux = normalize(cross(up, lookdir));
          uy = cross(lookdir, ux);

          base_x = ux * focus * viewport_x;
          base_y = uy * focus * viewport_y;
          origin = position - lookdir*focus - base_x/2 - base_y/2;
          lens = l;
     }

     ray generate(double ax, double ay) const {
          // 0 <= ax,ay <= 1
          return ray(position, origin+base_x*ax+base_y*ay-position);
          
          // vec3 rv = random2d();
          // vec3 ru = (ux*rv.v[0] + uy*rv.v[1]) * lens; // using depth of view
          // return ray(position+ru, origin+base_x*ax+base_y*ay-(position+ru));
     }
};

#endif