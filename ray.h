#ifndef RAY_H
#define RAY_H

# include "vec3.h"

class ray {
public:
     // ray: origin + t*direction
     vec3 origin;
     vec3 dir;
     double t;

     ray() {
          origin = vec3(); dir = vec3(); t = 0;
     }
     ray(vec3 o, vec3 d, double _t=0) {
          origin = o; dir = normalize(d); t = _t;
     }

     vec3 value(double _t) const {
          return origin + dir*_t;
     }

};


#endif