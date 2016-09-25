#include "../src/tree.h"
#include <iostream>

struct Point
{
  float data[3];

  Point() {}
  Point(float x, float y, float z) {
    data[0] = x;
    data[1] = y;
    data[2] = z;
  }

  float operator [] (int i) const {
    return data[i];
  }

  bool operator == (const Point& p) const {
    return data[0] == p[0] && data[1] == p[1] && data[2] == p[2];
  }

  friend std::ostream& operator << (std::ostream& s, const Point& p) {
    return s << '(' << p[0] << ", " << p[1] << ", " << p[2] << ')';
  }
};

int main() {
  typedef KD::Core<3, Point> CORE;

  Point min(0, 0, 0);
  Point max(9, 9, 9);
  KD::Tree<CORE> kdtree(min, max);

  kdtree.insert(Point(1, 2, 3));
  kdtree.insert(Point(8, 2, 5));
  kdtree.insert(Point(1, 0, 8));
  kdtree.insert(Point(3, 1, 4));

  std::cout << kdtree.nearest(Point(5, 5, 5)) << std::endl;
  return 0;
}
