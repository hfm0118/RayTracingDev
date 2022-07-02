#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <cmath>

class vec3 {
public:
     double v[3];

     vec3() {
          v[0] = 0; v[1] = 0; v[2] = 0;
     }
     vec3(double x, double y, double z) {
          v[0] = x; v[1] = y; v[2] = z;
     }

     double operator[](int i) {
          return v[i];
     }

     vec3 operator-() const {
          return vec3(-v[0], -v[1], -v[2]);
     }
     double l2norm() const {
          return v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
     }
     double length() const {
          return sqrt(l2norm());
     }
};

inline std::ostream& operator<<(std::ostream &os, const vec3 &x) {
    return os << x.v[0] << ' ' << x.v[1] << ' ' << x.v[2];
}

inline vec3 operator+(const vec3 &x, const vec3 &y) {
    return vec3(x.v[0] + y.v[0], x.v[1] + y.v[1], x.v[2] + y.v[2]);
}
inline vec3 operator-(const vec3 &x, const vec3 &y) {
    return x + (-y);
}
inline vec3 operator*(const vec3 &x, double a) {
    return vec3(a*x.v[0], a*x.v[1], a*x.v[2]);
}
inline vec3 operator/(const vec3 &x, double a) {
    return x * (1/a);
}

inline vec3 operator*(const vec3 &x, const vec3 &y) {
    return vec3( x.v[0]*y.v[0], x.v[1]*y.v[1], x.v[2]*y.v[2]);
}
inline double dot(const vec3 &x, const vec3 &y) {
    return x.v[0]*y.v[0] + x.v[1]*y.v[1] + x.v[2]*y.v[2];
}
inline vec3 cross(const vec3 &x, const vec3 &y) {
    return vec3(x.v[1]*y.v[2]-x.v[2]*y.v[1], x.v[2]*y.v[0]-x.v[0]*y.v[2], x.v[0]*y.v[1]-x.v[1]*y.v[0]);
}

inline vec3 normalize(vec3 x) {
     return x / x.length();
}

inline vec3 reflect(const vec3& x, const vec3& n) {
     // n is normal
     return x - n*dot(x,n)*2;
}
inline vec3 refract(const vec3& x, const vec3& n, double ri) {
    double cosy = dot(-x, n);
    if (cosy > 1) {cosy = 1;}
    vec3 rout1 = (x+n*cosy) * ri;
    vec3 rout2 = -n * sqrt(fabs(1.0 - rout1.l2norm()));
    return rout1 + rout2;
}

#endif