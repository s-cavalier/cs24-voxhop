#include "Point.h"

Point Point::inc(const Direction &dir) const {
  switch (dir) {
    case UP:
      return {z + 1, y, x};
      break;
    case DOWN:
      return {z - 1, y, x};
      break;
    case _NORTH:
      return {z, y + 1, x};
      break;
    case _SOUTH:
      return {z, y - 1, x};
      break;
    case _WEST:
      return {z, y, x + 1};
      break;
    case _EAST:
      return {z, y, x - 1};
      break;
    default:
      return {z, y, x};
  }
}

std::istream& operator >> (std::istream& stream, Point& point) {
  return stream >> point.x >> point.y >> point.z;
}

std::ostream& operator << (std::ostream& stream, const Point& point) {
  return stream << '(' << point.x << ", " << point.y << ", " << point.z << ')';
}
