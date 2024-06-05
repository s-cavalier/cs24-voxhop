#ifndef VOXMAP_H
#define VOXMAP_H

#include <istream>
#include <vector>
#include <queue>

#include "Point.h"
#include "Route.h"

class VoxMap {
  // Member Variables
  struct Voxel {
    char path_sign;
    Direction dir;
    Point self;
    int fall;

    public:
      Voxel(int f){
        fall = f;
        dir = DEFAULT;
      }
      Voxel(int f, int z, int y, int x){
        fall = f;
        self.x = x;
        self.y = y;
        self.z = z;
        dir = DEFAULT;
      }
      Voxel(){
        fall = -1;
        dir = DEFAULT;
      }
  };

  Point bounds;
  Voxel*** map;

  // Helper Functions
  bool isValid(Point p) const;
  bool validSource(Voxel p) const;
  void set_air(int x, int y, int z);
  Voxel& operator[] (Point p);
  Voxel& at(int x, int y, int z);
  void mark(Voxel& at, const Direction &from, const char &sign, std::queue<Voxel> &q);


public:
  VoxMap(std::istream& stream);
  ~VoxMap();

  Route route(Point src, Point dst);
};

#endif