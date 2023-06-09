#include <iostream>
#include <vector>

const double k = 1.0;
const double m = 1.;

struct Point {
  double location;
  double speed;
};

struct Data {
  double time;
  Point Loc_Speed;
  double energie;
};

Data TempData;

Point k_1(Point *Values, double *h) {
  Point F ;
  F.location = (Values->speed) * (*h);
  F.speed = -(Values->location)*(k/m)* (*h);
  return F;
}
Point k_2(Point *Values, double *h) {
  Point f_n;
  f_n = k_1(Values, h);
  f_n.location = (*h)*(Values->speed + f_n.speed/2.);
  f_n.speed = -(*h)*(k/m)*(Values->location + f_n.location /2.);
  return f_n;
}
void runge_kutta(Point *Values, double *h) {
  Point f_n = k_2(Values, h);
  Values->location = Values->location + f_n.location;
  Values->speed = Values->speed + f_n.speed;
}

int main(int argc, char *argv[]) {

  // physical systems constants
  // feder constant

  TempData.Loc_Speed.location = 0.;
  TempData.Loc_Speed.speed = 1.;
  TempData.time = 0;

  // simulation constant
  // time intervall
  const double iterations = 6000.;
  const double duration = 60.;

  double interval = duration / iterations;
  std::vector<Data> simulation_result;

  // std::cout << "Time" << " | " << "|" << "Location" << " | "<<"Speed" <<"\n";
  for (int i = 0; i < iterations; i++) {

    runge_kutta(&TempData.Loc_Speed, &interval);

    TempData.energie =
        (TempData.Loc_Speed.speed * TempData.Loc_Speed.speed +
         (k / m) * TempData.Loc_Speed.location * TempData.Loc_Speed.location) /
        2.0;
    TempData.time = TempData.time + interval;

    simulation_result.push_back(TempData);
    std::cout << TempData.time << " " << TempData.Loc_Speed.location << " "
              << TempData.Loc_Speed.speed << " " << TempData.energie << "\n";
  }

  return 0;
}