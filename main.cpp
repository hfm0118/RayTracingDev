#include <iostream>
#include <fstream>
#include <cmath>
#include "vec3.h"
#include "ray.h"
#include "camera.h"
#include "object.h"
#include "utilities.h"


int WIDTH = 800;
int HEIGHT = 600;
const int SAMPLE_DENSITY = 30;
const int DEPTH = 10;
const double PI = acos(-1.0); // 3.14159265...


std::string initiate(world& w, camera& cam, std::string type, int theta=0) {
     // initialize the world objects and camera
     vec3 pos(10,1,10), lookat(0,0,0), up(0,1,0);
     std::string output_path = "output.ppm";
     double cam_radius, cam_height;

     if (type == "simple") {
          w.sky_color = vec3(0.8, 0.8, 0.9);
          optics ground_material(vec3(0.5, 0.5, 0.5));
          ball ground(vec3(0,-1000,0), 1000, ground_material);
          w.objects.push_back(ground);
          optics mirror_material(vec3(0.8, 0.8, 0.8), 0, true);
          ball mirror(vec3(0,1,0), 1, mirror_material);
          w.objects.push_back(mirror);
          optics ball_material(vec3(0.6, 0.7, 0.8));
          ball reference(vec3(2,0.3,-1), 0.3, ball_material);
          w.objects.push_back(reference);
          cam_radius = 12; cam_height = 2;
          lookat = vec3(0,1,0);
          up = vec3(0,1,0);
          output_path = "output_simple.ppm";
     }
     else if (type == "glass") {
          w.sky_color = vec3(0.8, 0.8, 0.9);
          optics ground_material(vec3(0.5, 0.5, 0.5));
          ball ground(vec3(0,-1500,0), 1500, ground_material);
          w.objects.push_back(ground);
          optics glass_material(vec3(1.0, 1.0, 1.0), 0, true, true, 1.05);
          ball lens(vec3(0,1,0), 1, glass_material);
          w.objects.push_back(lens);
          optics ball_material(vec3(0.6, 0.7, 0.8));
          ball reference(vec3(-2,0.5,-1), 0.5, ball_material);
          w.objects.push_back(reference);
          cam_radius = 15; cam_height = 3;
          lookat = vec3(0,1,0);
          up = vec3(0,1,0);
          output_path = "output_glass.ppm";
     }
     else if (type == "light") {
          w.sky_color = vec3(0.3, 0.3, 0.3);
          optics ground_material(vec3(0.5, 0.5, 0.5));
          ball ground(vec3(0,-1000,0), 1000, ground_material);
          w.objects.push_back(ground);
          optics light_material(vec3(0.9, 0.9, 0.9), 0, false, false, 0, true);
          ball light(vec3(-1,0.8,0), 0.8, light_material);
          w.objects.push_back(light);
          optics ball_material(vec3(0.6, 0.7, 0.8));
          ball reference(vec3(1,0.4,-1), 0.4, ball_material);
          w.objects.push_back(reference);
          cam_radius = 12; cam_height = 2;
          lookat = vec3(0,0,0);
          up = vec3(0,1,0);
          output_path = "output_light.ppm";
     }

     double costh = cos(PI*theta/180), sinth = sin(PI*theta/180);
     pos = vec3(cam_radius*costh, cam_height, cam_radius*sinth);
     cam = camera(pos, lookat, up, 0.3*WIDTH/HEIGHT, 0.3, 10, 0.05);
     return output_path;
}


int main() {

     std::string world_type = "simple";
     int theta = 0;
     std::ifstream fin;
     fin.open("config.ini", std::ios::in);
     char buf[64];
     if (fin.is_open()) {
          fin >> WIDTH >> HEIGHT >> buf >> theta;
          world_type = std::string(buf);
     }
     fin.close();
     std::cout << "The world setting is " << buf << ".\n";

     world w; 
     camera cam;
     std::string output_path = initiate(w, cam, world_type, theta);
     std::ofstream fout(output_path);
     fout << "P3\n" << WIDTH << ' ' << HEIGHT << "\n255\n";

     // rendering every pixel
     for (int y = HEIGHT-1; y>-1; --y) {
          std::cout << "\rRendering " << HEIGHT-y << "/" << HEIGHT << " Lines..." << std::flush;
          for (int x = 0; x<WIDTH; ++x) {
               vec3 color;
               for (int i=0; i<SAMPLE_DENSITY; ++i) {
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
