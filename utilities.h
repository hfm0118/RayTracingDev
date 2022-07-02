#ifndef UTILITIES_H
#define UTILITIES_H

#include <cmath>
#include <cstdlib>

inline double restrict(double x, double min, double max) {
    // restrict x in range [min, max]
    if (x > max) {return max;}
    if (x < min) {return min;}
    return x;
}

inline double random_in_one() {
    // sample a random double from [0,1)
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // sample a random double from [min, max)
    return min + (max-min)*random_in_one();
}

inline vec3 random2d() {
    // sample vector in a unit circle
    vec3 rv;
    while (1) {
        rv = vec3(random_double(-1,1), random_double(-1,1), 0);
        if (rv.l2norm() < 1) {
            break;
        }
    }
    return rv;
}

inline vec3 random3d() {
    // sample vector in a unit sphere
    vec3 rv;
    while (1) {
        rv = vec3(random_double(-1,1), random_double(-1,1), random_double(-1,1));
        if (rv.l2norm() < 1) {
            break;
        }
    }
    return rv;
}

#endif