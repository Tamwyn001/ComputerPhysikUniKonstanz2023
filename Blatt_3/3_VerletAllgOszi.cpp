#include <iostream>
#include <vector>
#include <math.h>

double omega = 1.0; // omega = sqrt(k/m)

const double gamma = 10; // schwache schwingung: 0.1, APGF: 1.1, KF: 10

struct Data {
  double time;
  double location;
  double energie;
};

Data TempData;

double external_force(Data *Values, double *h) {
  return sin(TempData.time + (*h));
}

void verlet(Data *Values, double *h, std::vector<double>* last_xn) {
  double x_n_m1 = 0;
  auto it= last_xn->begin(); //allocator to add at the index 0
  last_xn->insert(it ,Values->location);
  if ((last_xn->size())>1)
  {
    double x_n_m1 = (*last_xn)[1];
  }  
  
  Values->location =
      1 / (2 + gamma * (*h)) *
      (2 * (*h) * (*h) * external_force(Values, h) +
       2 * Values->location * (2 - (*h) * (*h) * (omega) * (omega)
       * (x_n_m1)*(gamma*(*h)-2)));
}

int main(int argc, char *argv[]) {

  // physical systems constants
  // feder constant

  TempData.location = 0.;
  //TempData.Loc_Speed.speed = 1.;
  TempData.time = 0;
  double last_xn = 0;

  // simulation constant
  // time intervall
  const double iterations = 6000.;
  const double duration = 60.;

  double interval = duration / iterations;
  std::vector<Data> simulation_result;
  std::vector<double> x_n_temp;
  std::cout<< "Omega: "<<"\n";
  std::cin >> omega;
  // std::cout << "Time" << " | " << "|" << "Location" << " | "<<"Speed" <<"\n";
  for (int i = 0; i < iterations; i++) {

    verlet(&TempData, &interval, &x_n_temp);
    TempData.time = TempData.time + interval;

    simulation_result.push_back(TempData);
    std::cout << TempData.time << " " << TempData.location << " "
              <<  "\n";
  }

  return 0;
}