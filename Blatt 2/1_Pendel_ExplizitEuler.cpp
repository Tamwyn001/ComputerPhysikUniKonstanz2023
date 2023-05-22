#include <iostream>

struct Point {
  double time;
  double value;
};

// explizit verfahren
void euler(double *x, double *v, double interval, double k, double m) {
  *v = *v - interval * (k / m) * *x;
  *x = *x + *v;
}

int main(int argc, char *argv[]) {

  // physical systems constants
  // feder constant
  double k = 1.0;

  // mass
  double m = 3.0;

  double x = 0;
  double v = 0;

  // simulation constant
  // time intervall

  double iterations = 100.;
  double duration = 30.;

  double interval = duration / iterations;

  Point simulatiomn_result[int(iterations)];

  for (int i = 0; i < iterations; i++) {
    euler(&x, &v, interval, k, m);
    Point value;
    value.time = x;
    value.value = v;
    simulatiomn_result[i] = value;
    std::cout << i*interval << " " << x <<"\n";
  }

  return 0;
}