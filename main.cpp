#include <iostream>
#include <fstream>
#include "vec3.h"
#include "ray.h"
#include "camera.h"
#include "object.h"
#include "utilities.h"

const int WIDTH = 800;
const int HEIGHT = 600;
const int SAMPLE_DENSITY = 30;
const int DEPTH = 10;

int main() {

     world w("simple"); 

     vec3 pos(12,2,3);
     vec3 lookat(0,1,0);
     vec3 up(0,1,0);
     camera cam(pos, lookat, up, 0.4, 0.3, 10, 0.05);

     std::ofstream fout("output.ppm");
     fout << "P3\n" << WIDTH << ' ' << HEIGHT << "\n255\n";

     // rendering every pixel
     for (int y = HEIGHT-1; y>-1; --y) {
          std::cout << "\rRendering " << HEIGHT-y << "/" << HEIGHT << " Lines..." << std::flush;
          for (int x = 0; x<WIDTH; ++x) {
               vec3 color;
               for (int i = 0; i<SAMPLE_DENSITY; ++i) {
                    // calculating rays...
                    double _x = (x+random_in_one())/(WIDTH-1);
                    double _y = (y+random_in_one())/(HEIGHT-1);
                    ray r = cam.generate(_x, _y);
                    color = color + calculate_color(r, w, DEPTH) / SAMPLE_DENSITY;
               }
               fout << int(256*restrict(color.v[0],0.0,0.99999)) << ' '
                    << int(256*restrict(color.v[1],0.0,0.99999)) << ' '
                    << int(256*restrict(color.v[2],0.0,0.99999)) << '\n';
          }
     }

     std::cout << "\nFinished.";

}
