#include <iostream>
#include <vector>

const double k = 1.0;
const double m = 1.0;

struct Point {
  double location;
  double speed;
};

struct Data{
    double time;
    Point Loc_Speed;
};


// explizit verfahren
Point base_equation(Point* Value, double *h) 
{
    Point Out;
    Out.speed = Value->speed - (k/m) * (*h) * (Value->location);
    Out.location = Value->location + *h* Value->speed;
    return Out;
}
Point k_1(Point* Values, double *h)
{
    Point Out;
    Out.location = base_equation(Values, h).location * *h;
    Out.speed = base_equation(Values, h).speed * *h;
    return Out ;
}
Point k_2(Point* Values, double * h)
{
    Point Out;
    Point in_base;
    in_base = k_1(Values,h);

    in_base.speed = Values->speed + in_base.speed / 2.;
    in_base.location =Values->location +  in_base.location / 2.;

    double h_next = *h + *h/2;
    in_base = base_equation(&in_base ,& h_next);

    Out.location = *h * in_base.location; 
    Out.speed = *h * in_base.speed; 
    return Out;
}
void runge_kutta(Point * Values, double *h)
{
    Values->location = Values->location + k_2(Values, h).location;
    Values->speed = Values->speed + k_2(Values, h).speed; 
}

int main(int argc, char *argv[]) {

  // physical systems constants
  // feder constant

    double x = 0;
    double v = 0;
    Data TempData;
    Point value;
    value.location = x;
    value.speed = v;
    TempData.Loc_Speed = value;
    TempData.time = 0;    

  // simulation constant
  // time intervall

    double time = 0.;
  const double iterations = 100.;
  const double duration = 30.;

  double interval = duration / iterations;
  std::vector<Data> simulation_result;

  //std::cout << "Time" << " | " << "|" << "Location" << " | "<<"Speed" <<"\n";
  for (int i = 0; i < iterations; i++) {
    runge_kutta(&value, &interval);
    Data TempData;
    TempData.Loc_Speed = value;
    TempData.time = TempData.time + interval;
    simulation_result.push_back(TempData);
    std::cout << TempData.time << " " << TempData.Loc_Speed.location<< " " <<  TempData.Loc_Speed.speed <<"\n";
  }

  return 0;
}