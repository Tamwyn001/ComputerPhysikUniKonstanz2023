#include <iostream>
#include <vector>

const double k = 1.0;
const double m = 1.;
const double gamma = 10; //schwache schwingung: 0.1, APGF: 1.1, KF: 10
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
Point predic(Point *Values, double *h) {
  Point Out;
  Out.location = Values->location +(*h)*Values->speed;
  Out.speed = Values->speed - (*h)*((k/m)*Values->location + gamma * Values->speed);
  return Out;
}
void heun(Point *Values, double *h) {
  Point r_predic = predic(Values, h);
  Values->location = (1/2.)* (Values->location + r_predic.location + (*h)*r_predic.speed);
  Values->speed = (1/2.)*(Values->speed + r_predic.speed - (*h)*((k/m)*r_predic.location + gamma * r_predic.speed));
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

    heun(&TempData.Loc_Speed, &interval);
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