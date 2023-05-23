#include <iostream>
#include <vector>

const double k = 1.0;
const double m = 1.;

struct Point {
  double location;
  double speed;
  double acceleration;
};

struct Data {
  double time;
  Point Loc_Speed;
  double energie;  
};


void base_acceleration(double* speed, double* acceleration, double *h) {
  *acceleration = (*acceleration) - (k / m) * (*speed)*(*h);
}

void leap_frog(Point *Values, double *h) {
  // update acceleration
  base_acceleration(&Values->speed, &Values->acceleration, h);

  Values->location = Values->location + Values->speed * (*h);
  Values->speed = Values->speed + (*h) * Values->acceleration;
  
}

int main(int argc, char *argv[]) {

  // physical systems constants
  // feder constant
  Data TempData;
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

    leap_frog(&TempData.Loc_Speed, &interval);

    TempData.time = TempData.time + interval;
    TempData.energie = (TempData.Loc_Speed.speed * TempData.Loc_Speed.speed +
                        (k / m) * TempData.Loc_Speed.location * TempData.Loc_Speed.location)/
                       2.0;

    simulation_result.push_back(TempData);
    std::cout << TempData.time << " " << TempData.Loc_Speed.location << " "
              << TempData.Loc_Speed.speed << " "<< TempData.energie <<"\n";
  }

  return 0;
}