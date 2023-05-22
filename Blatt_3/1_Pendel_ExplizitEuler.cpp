#include <iostream>
#include <vector>

struct Point {
  double time;
  double value;
  double energie;
};

// explizit verfahren
void euler(double *x, double *v, double interval, double k, double m) {
  *v = *v - interval * (k / m) * (*x);
  *x = *x + interval * *v;
}

int main(int argc, char *argv[]) {

  // physical systems constants
  // feder constant
  const double k = 1.0;

  // mass
  const double m = 3.0;

  double x = 0.;
  double v = 2.0;

  // simulation constant
  // time intervall

  const double iterations = 100.;
  const double duration = 30.;

  double interval = duration / iterations;
  std::vector<Point> simulation_result;

  for (int i = 0; i < iterations; i++) {
    euler(&x, &v, interval, k, m);
    Point value;
    value.time = x;
    value.value = v;
    value.energie = ((v) * (v) + (k / m) * (x) * (x)) / 2.0;
    simulation_result.push_back(value);
    std::cout << i * interval << " " << x <<" "<< v << " " << value.energie << "\n";
  }

  return 0;
}