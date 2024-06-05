#include "VoxMap.h"
#include "Errors.h"
#include <iostream>
#include <queue>
using namespace std;

bool VoxMap::isValid(Point p) const {
  return (p.x < bounds.x && p.y < bounds.y && p.z < bounds.z) && (p.x >= 0 && p.y >= 0 && p.z >= 0);
}

bool VoxMap::validSource(Voxel p) const{
  if(p.fall < 0){
    return false;
  }
  if(p.fall > 0){
    return false;
  }
  return true;
}


VoxMap::Voxel& VoxMap::operator[] (Point p) {
  if (!isValid(p)) throw InvalidPoint(p);
  return map[p.z][p.y][p.x];
}

VoxMap::Voxel& VoxMap::at (int z, int y, int x) {
  Point use(z, y, x);
  if (!isValid(use)) throw InvalidPoint(use);
  return map[z][y][x];
}

void VoxMap::set_air(int z, int y, int x) {
  if (z - 1 <= -1) {
    map[z][y][x].fall = 50000;
    map[z][y][x].self.x = x;
    map[z][y][x].self.y = y;
    map[z][y][x].self.z = z;
    return;
  }
  map[z][y][x].fall = map[z-1][y][x].fall + 1;
  map[z][y][x].self.x = x;
  map[z][y][x].self.y = y;
  map[z][y][x].self.z = z;
}

VoxMap::VoxMap(std::istream& stream) {
// (x, y, z)
  std::string line;
  getline(stream, line, ' ');
  bounds.x = stoi(line);
  getline(stream, line, ' ');
  bounds.y = stoi(line);
  getline(stream, line);
  bounds.z = stoi(line);

  map = new Voxel**[bounds.z];
  for (int i = 0; i < bounds.z; i++) {
    map[i] = new Voxel*[bounds.y];
    for (int j = 0; j < bounds.y; j++)
        map[i][j] = new Voxel[bounds.x];
  }

  int height = -1;
  int ns = -1; // tells what line is being read (North-South coordinate)
  while (std::getline(stream, line)) {
    if (line.size() == 0) {
      height++;
      ns = -1;
      continue;
    }

    ns++;
    int end = line.size();
    for (int i = 0; i < end; i++) {
      
      // giant switch case to save on runtime
      switch (line[i]) {
        case 'f': // 1111
          map[height][ns][4 * i] = Voxel(-1,height,ns,4*i);
          map[height][ns][4 * i + 1] = Voxel(-1,height,ns,4*i+1);
          map[height][ns][4 * i + 2] = Voxel(-1,height,ns,4*i+2);
          map[height][ns][4 * i + 3] = Voxel(-1,height,ns,4*i+3);
          break;
        case 'e': // 1110
          map[height][ns][4 * i] = Voxel(-1,height,ns,4*i);
          map[height][ns][4 * i + 1] = Voxel(-1,height,ns,4*i+1);
          map[height][ns][4 * i + 2] = Voxel(-1,height,ns,4*i+2);
          set_air(height, ns, 4 * i + 3);
        break;
        case 'd': // 1101
          map[height][ns][4 * i] = Voxel(-1,height,ns,4*i);
          map[height][ns][4 * i + 1] = Voxel(-1,height,ns,4*i+1);
          set_air(height, ns, 4 * i + 2);
          map[height][ns][4 * i + 3] = Voxel(-1,height,ns,4*i+3);
        break;
        case 'c': // 1100
          map[height][ns][4 * i] = Voxel(-1,height,ns,4*i);
          map[height][ns][4 * i + 1] = Voxel(-1,height,ns,4*i+1);
          set_air(height, ns, 4 * i + 2);
          set_air(height, ns, 4 * i + 3);
        break;
        case 'b': // 1011
          map[height][ns][4 * i] = Voxel(-1,height,ns,4*i);
          set_air(height, ns, 4 * i + 1);
          map[height][ns][4 * i + 2] = Voxel(-1,height,ns,4*i+2);
          map[height][ns][4 * i + 3] = Voxel(-1,height,ns,4*i+3);
        break;
        case 'a': // 1010
          map[height][ns][4 * i] = Voxel(-1,height,ns,4*i);
          set_air(height, ns, 4 * i + 1);
          map[height][ns][4 * i + 2] = Voxel(-1,height,ns,4*i+2);
          set_air(height, ns, 4 * i + 3);
        break;
        case '9': // 1001
          map[height][ns][4 * i] = Voxel(-1,height,ns,4*i);
          set_air(height, ns, 4 * i + 1);
          set_air(height, ns, 4 * i + 2);
          map[height][ns][4 * i + 3] = Voxel(-1,height,ns,4*i+3);
        break;
        case '8': // 1000
          map[height][ns][4 * i] = Voxel(-1,height,ns,4*i);
          set_air(height, ns, 4 * i + 1);
          set_air(height, ns, 4 * i + 2);
          set_air(height, ns, 4 * i + 3);
        break;
        case '7': // 0111
          set_air(height, ns, 4 * i);
          map[height][ns][4 * i + 1] = Voxel(-1,height,ns,4*i+1);
          map[height][ns][4 * i + 2] = Voxel(-1,height,ns,4*i+2);
          map[height][ns][4 * i + 3] = Voxel(-1,height,ns,4*i+3);
        break;
        case '6': // 0110
          set_air(height, ns, 4 * i);
          map[height][ns][4 * i + 1] = Voxel(-1,height,ns,4*i+1);
          map[height][ns][4 * i + 2] = Voxel(-1,height,ns,4*i+2);
          set_air(height, ns, 4 * i + 3);
        break;
        case '5': // 0101
          set_air(height, ns, 4 * i);
          map[height][ns][4 * i + 1] = Voxel(-1,height,ns,4*i+1);
          set_air(height, ns, 4 * i + 2);
          map[height][ns][4 * i + 3] = Voxel(-1,height,ns,4*i+3);
        break;
        case '4': // 0100
          set_air(height, ns, 4 * i);
          map[height][ns][4 * i + 1] = Voxel(-1,height,ns,4*i+1);
          set_air(height, ns, 4 * i + 2);
          set_air(height, ns, 4 * i + 3);
        break;
        case '3': // 0011
          set_air(height, ns, 4 * i);
          set_air(height, ns, 4 * i + 1);
          map[height][ns][4 * i + 2] = Voxel(-1,height,ns,4*i+2);
          map[height][ns][4 * i + 3] = Voxel(-1,height,ns,4*i+3);
        break;
        case '2': // 0010
          set_air(height, ns, 4 * i);
          set_air(height, ns, 4 * i + 1);
          map[height][ns][4 * i + 2] = Voxel(-1,height,ns,4*i+2);
          set_air(height, ns, 4 * i + 3);
        break;
        case '1': // 0001
          set_air(height, ns, 4 * i);
          set_air(height, ns, 4 * i + 1);
          set_air(height, ns, 4 * i + 2);
          map[height][ns][4 * i + 3] = Voxel(-1,height,ns,4*i+3);
        break;
        case '0': // 0000
          set_air(height, ns, 4 * i);
          set_air(height, ns, 4 * i + 1);
          set_air(height, ns, 4 * i + 2);
          set_air(height, ns, 4 * i + 3);
        break;
        default:
          throw std::runtime_error("Non standard Voxel");
        break;
      }
    }
  }
  // for(int i = 0; i < bounds.z; i++){
  //   for(int j = 0; j < bounds.y; j++){
  //     for(int k = 0; k < bounds.x; k++){
  //       cout << map[i][j][k].fall << " ";
  //     }
  //     cout << endl;
  //   }
  //   cout << endl;
  // }
}

VoxMap::~VoxMap() {
  // FIXME
  for (int i = 0; i < bounds.z; i++) {
    for (int j = 0; j < bounds.y; j++){
      delete[] map[i][j];
    }
    delete[] map[i];
  }
  delete[] map; 
}

void VoxMap::mark(Voxel& at, const Direction &from, const char &sign, queue<Voxel> &q) {
  at.path_sign = sign;
  at.dir = from;
  if (at.fall == 0) q.push(at);
}

Route VoxMap::route(Point src, Point dst) {
  if(!isValid(src)){
    throw InvalidPoint(src);
  }
  if(!validSource(map[src.z][src.y][src.x])){
    //cout << "Z: " << src.z << " Y: " << src.y << " X: " << src.x << endl;
    throw InvalidPoint(src);
  }
  if(!isValid(dst)){
    throw InvalidPoint(dst);
  }
  if(!validSource(map[dst.z][dst.y][dst.x])){
    throw InvalidPoint(dst);
  }

  queue<Voxel> source;
  queue<Voxel> target;
  bool found = false;
  mark((*this)[src], START, '$', source);
  mark((*this)[dst], END, '#', target);

  while (!found) {
    if (source.empty()) throw NoRoute(src, dst);
    Voxel parent = source.front();
    source.pop();

    for (Direction d = Direction::_NORTH; d < 4; d = Direction(d + 1)) {
    Voxel curr = (*this)[parent.self.inc(d)];
      try {
        if (curr.fall == 0) {
          if (curr.dir == DEFAULT) {
            mark(curr, d, '$', source);
          }
          else if (curr.path_sign != '$') {
            found = true;
            source.push(curr);
            break;
          }
        }
        else if (curr.fall > 0 && curr.fall < 50000) {
          mark(curr, d, '$', source);
          curr = (*this)[{curr.self.z - curr.fall, curr.self.y, curr.self.x}];
          mark(curr, Direction::DOWN, '$', source);
        }
        else if (curr.fall == -1 && (*this)[parent.self.inc(UP)].fall >= 0 && (*this)[curr.self.inc(UP)].fall == 0) {
          curr = (*this)[curr.self.inc(UP)];
          mark(curr, d, '$', source);
        }
      } catch (const InvalidPoint &e) {
        continue;
      }
    }

  }

  throw NoRoute(src, dst);
}