#include <fstream>
#include <iostream>
#include <math.h>

#define PHY_COULOMB 8.9875517922 * pow(10., 9.) /* coulomb cosntant*/

//analytical solution to chi = r * potential
double chi_ana(double *radius) {
  return (PHY_COULOMB) *
         (1. - (1. + (*radius) / 2.) * exp(((-1.) * (*radius))));
}

//possible extention with potential (analytical solution)
double phi_ana(double *radius) {
  return (PHY_COULOMB) *
         (1 / (*radius) - (1 / (*radius) + 0.5) * exp(-(*radius)));
}

//the F function of the numerov process
double F_function_chi(double radius) {

  return exp((-1) * (radius)) * (radius) * (-1.) * PHY_COULOMB * 0.5;

}

//calculation of the next step of the function to integrate : f
void numerov_f_next(double *f_n_chi, double *f_n_minus_1_chi, double *f_n_phi,
                    double *f_n_minus_1_phi, double *h, double *radius) {

  double f_n_plus_1_chi = (*h) * (*h) / 12. *
                              ((F_function_chi((*radius) - (*h))) +
                               10. * (F_function_chi((*radius))) +
                               (F_function_chi((*radius) + (*h)))) +
                          2 * (*f_n_chi) - (*f_n_minus_1_chi);

  // add step iterration to f0
  (*f_n_minus_1_chi) = (*f_n_chi);
  (*f_n_chi) = f_n_plus_1_chi;

  //possible extention of potential but doesnt work correctly, expodes for r approx 4
  double f_n_plus_1_phi = f_n_plus_1_chi / (*radius);

  // add step iterration to f0
  (*f_n_minus_1_phi) = (*f_n_phi);
  (*f_n_phi) = f_n_plus_1_phi;

  return;
}

int main(int argc, char *argv[]) {

  //simulation initialisation
  double simulation_step = 0.0001;
  double start_r = 10.;
  double f_n_chi = chi_ana(&start_r), f_n_minus_1_chi = 0;
  double f_n_phi = phi_ana(&start_r), f_n_minus_1_phi = 0;
  double i = 0;

  //bind the output file
  std::string path =
      "C:/Users/Tamwyn/Documents/Physik/ComputerPhysikUniKonstanz2023/Blatt_5/"
      "Results/Numerov.dat";
  std::ofstream file(path, std::ios::trunc);

  //row names
  file << "Radius"
       << " "
       << "chi(r)"
       << " "
       << "analytical chi"
       << " "
       << "F"
       << " "
       << "phi"
       << " "
       << "analytical phi"
       << "\n";

  if (file.is_open()) {

    //correction of the sollution bc of the linear term, we take the grow constant to each step
    double corrector_chi =
        (8.986 * pow(10., 13.) - chi_ana(&start_r)) * 0.1 * simulation_step;
    double corrector_phi = 0;

    //outup of the inital values in file
    file << start_r << " " << f_n_chi - i * corrector_chi << " "
         << chi_ana(&start_r) << " " << F_function_chi(start_r) << " "
         << f_n_phi - i * corrector_phi << " " << phi_ana(&start_r) << "\n";
    i += 1.;


  //simulation loop
    for (double radius = start_r; radius > 0; radius -= simulation_step) {
      
      //simulation interation
      numerov_f_next(&f_n_chi, &f_n_minus_1_chi, &f_n_phi, &f_n_minus_1_phi,
                     &simulation_step, &radius);

      //output of the values
      file << radius << " " << f_n_chi - i * corrector_chi << " "
           << chi_ana(&radius) << " " << F_function_chi(radius) << " "
           << (f_n_chi - i * corrector_chi) / (radius)<< " " << phi_ana(&radius) << "\n";

      //corrector is a constant so we multiply to override the linear growth
      i += 1.;
    }
  }
}
