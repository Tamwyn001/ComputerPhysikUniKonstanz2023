#include <fstream>
#include <iostream>
#include <math.h>

#define PHY_COULOMB 8.9875517922 * pow(10., 9.) /* coulomb cosntant*/

double chi_ana(double *radius) {
  return (PHY_COULOMB) *
         (1. - (1. + (*radius) / 2.) * exp(((-1.) * (*radius))));
}

double F_function(double radius) {
  // std::cout<<"In:"<< radius<<": "<<exp((-1) * (radius))* (radius)<<"\n";
  return exp((-1) * (radius)) * (radius) * (-1.) * PHY_COULOMB * 0.5;
  // return 0.;
}

void numerov_f_next(double *f_n, double *f_n_minus_1, double *h, double *radius,
                    double *corrector) {

  double f_n_plus_1 =
      (*h) * (*h) / 12. *
          ((F_function((*radius) - (*h))) + 10. * (F_function((*radius))) +
           (F_function((*radius) + (*h)))) +
      2 * (*f_n) - (*f_n_minus_1) - (*corrector);

  // add step iterration to f0
  (*f_n_minus_1) = (*f_n);
  (*f_n) = f_n_plus_1;

  return;
}

int main(int argc, char *argv[]) {

  double simulation_step = 0.001;
  double start_r = 10.;
  double f_n = chi_ana(&start_r), f_n_minus_1 = 0;
  double i = 0;
  std::string path =
      "C:/Users/Tamwyn/Documents/Physik/ComputerPhysikUniKonstanz2023/Blatt_5/"
      "Results/Numerov.dat";
  std::ofstream file(path, std::ios::trunc);

  file << "Radius"
       << " "
       << "chi(r)"
       << " "
       << "analytical"
       << "\n";
  if (file.is_open()) {

    double corrector = 0; //(8.986 * pow( 10., 13.) - chi_ana(&start_r)) * 0.1 *
                          //simulation_step;
    double corrector_2 =
        (8.986 * pow(10., 13.) - chi_ana(&start_r)) * 0.1 * simulation_step;

    file << start_r << " " << f_n - i * corrector_2 << " " << chi_ana(&start_r)
         << " " << F_function(start_r) << "\n";
    i += 1.;

    for (double radius = start_r; radius > 0; radius -= simulation_step) {

      numerov_f_next(&f_n, &f_n_minus_1, &simulation_step, &radius, &corrector);

      file << radius << " " << f_n - i * corrector_2 << " " << chi_ana(&radius)
           << " " << F_function(radius) << "\n";

      i += 1.;
    }
  }
}
