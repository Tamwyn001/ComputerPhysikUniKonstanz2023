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

Point base_equation(Point *Value, double *h) {
  Point Out;

  Out.location = Value->location + (*h) * Value->speed;
  Out.speed = Value->speed - (k / m) * (*h) * (Value->location);

  return Out;
}
Point k_1(Point *Values, double *h) {
  Point F = base_equation(Values, &TempData.time);
  F.location = F.location * (*h);
  F.speed = F.speed * (*h);
  return F;
}
Point k_2(Point *Values, double *h) {
  Point Out;
  Point f_n;
  f_n = k_1(Values, h);

  f_n.location = Values->location + (f_n.location * (*h)/ 2.);
  f_n.speed = Values->speed + (f_n.speed * (*h)/ 2.);


  // double x_new = (*h) + (*h) / 2;
  double x_new = TempData.time + ((*h) / 2);

  f_n = base_equation(&f_n, &x_new);

  Out.location = (*h) * f_n.location;
  Out.speed = (*h) * f_n.speed;
  return Out;
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
  const double iterations = 1000.;
  const double duration = 10.;

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