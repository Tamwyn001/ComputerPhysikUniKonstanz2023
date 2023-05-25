#include <iostream>
#include <math.h>
#include <vector>

double omega_pendel = 1; // omega = sqrt(k/m)
double gamma;
std::vector<double> gammas = {100.};
double amplitude;
double omega_trieb;

struct Data {
  double time;
  double location;
  double energie;
};

Data TempData;

void update_max_amplitude(double* current_x, double *x_min, double* x_max){
  if ((*current_x)>(*x_max))
  {
    (*x_max) = (*current_x);
  }
  else if ((*current_x)>(*x_min))
  {
    (*x_min) = (*current_x);
  }
  return;
  
}

double external_force(Data *Values, double *h) {
  return sin(omega_trieb*(TempData.time + (*h))); //omega_t =1
}

void verlet(Data *Values, double *h, std::vector<double> *last_xn) {
  double x_n_m1 = 0;
  auto it = last_xn->begin(); // allocator to add at the index 0
  last_xn->insert(it, Values->location);
  if ((last_xn->size()) > 1) {
    x_n_m1 = (*last_xn)[1];
  }

  Values->location = 1 / (2 + gamma * (*h)) *
                     (2 * (*h) * (*h) * external_force(Values, h) +
                      2 * Values->location *
                          (2 - (*h) * (*h) * (omega_pendel) * (omega_pendel) +
                           (x_n_m1) * (gamma * (*h) - 2)));
}

int main(int argc, char *argv[]) {

  // physical systems constants
  // feder constant

  TempData.location = 0.;
  // TempData.Loc_Speed.speed = 1.;
  TempData.time = 0;
  double last_xn = 0;

  // simulation constant
  // time intervall
  const double iterations = 1000.;
  const double duration = 10.;

  double interval = duration / iterations;
  std::vector<double> values_at_time;
  std::vector<double> x_n_temp;

  double x_min, x_max = TempData.location;
  

  for (double iter_omega=0; iter_omega < 10.; iter_omega = iter_omega + 0.1) {
    omega_trieb = 2;
    std::cout<< omega_trieb<<" ";
    
    for (double iter_gamma : gammas) {
      gamma = iter_gamma;
      for (double i=0 ; i< iterations; i+=1){

        verlet(&TempData, &interval, &x_n_temp);

        TempData.time = TempData.time + interval;
        //simulation_result.push_back(TempData);
        //std::cout<<TempData.location<<" ";
        std::cout<<amplitude<<" ";
        update_max_amplitude(&TempData.location, &x_min, &x_max);
        }

      amplitude = x_max - x_min;
      std::cout<<amplitude;
    }
    std::cout <<"\n";
  }
  return 0;
}