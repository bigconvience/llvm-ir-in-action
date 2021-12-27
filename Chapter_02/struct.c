typedef struct Point {
    double x;
    double y;
    double z;
} Point;

Point add_points(Point a, Point b) {
  Point p;
  p.x = a.x + b.x;
  p.y = a.y + b.y;
  p.z = a.z + b.z;
  return p;
}

int main() {
  Point a = {1.0, 3.0, 4.0};
  Point b = {2.0, 8.0, 5.0};
  Point c = add_points(a, b);
  return 0;
}