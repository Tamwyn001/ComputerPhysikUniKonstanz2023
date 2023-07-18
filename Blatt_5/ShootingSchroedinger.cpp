#include <fstream>
#include <iostream>
#include <math.h>
#include <vector>
#include <string>

//simulations constants
const double HBAR = 1; //1.054571817e-34;

const double xmax = 20.;
const double H = 0.001;
const double start_pos = 0.;

const double mass = 1.;

//the outup and process data structure
struct Wavefunction {
  double position;
  double amplitude;
  double analytical;
};

//the save buffer, need to be saved in a file and then flushed if we change the simulation context
std::vector<Wavefunction> results;

//for newton we need to remap a value to the closer value in the result list in order to interate on the list
bool is_in_ball(double center, double radius, double value){
  return abs(value - center ) <= radius;
}


//for newton we need to know at wich index ID(x) we are take the amplitude A(x)
int find_index_at_position(double position){
  int i = 0;
  for (Wavefunction temp_result : results){

      if (is_in_ball(temp_result.position, H/2., position)){
        break;
      }
    i++;   
  }
  return i;
}

//the analytical solution of the wave function at a given place
double analytical_psi(double position){
  double ai, aip, bi,bip;
  double x = pow(2*mass/(HBAR*HBAR), 1./3.);
  //return alglib::airy(position, ai, aip, bi, bip);
}
//the potential of the 1D Space
double potential(double x) {
  if (x < 0) {
    return INFINITY;
  }
  return x;
}

//the k^2 terme in Numerov-process
double k2(double x, double m, double E) {
  return 2 * m / HBAR * ( E - potential(x));
}

//save some data in the results list
void save_result(double position, double amplitude, double anaylical){
  Wavefunction temp_result;
  temp_result.position=position;
  temp_result.amplitude = amplitude;
  temp_result.analytical = anaylical;
  results.push_back(temp_result);
}

void flush_save_buffer()
{
  results.clear();
  return;
}

double shoot_numerov(double E, bool save_values) {
  
  double fn = start_pos + H, fn_min_1 = start_pos, fn_pl_1;

  for (double position = start_pos; position < xmax; position += H)
  {
    //avoid x<0, otherwise fn explodes and the porcess outputs "nan" (recursivity). we allready know psi(x)|x<0 = 0..
    if (position-H < 0.)
    {
      position = H;
    }

    if (save_values)
    {
      save_result(position, fn, analytical_psi(position));
    }
    
    //calculate value according to numerov process
    fn_pl_1 = (fn * (2. - (5. / 6.) * H * H * k2(position, mass, E)) - fn_min_1 * (1. + H * H / 12. * k2(position - H, mass, E))) / (1 + H*H /12. * k2(position+H, mass, E));

    //shift the values
    fn_min_1 = fn;
    fn = fn_pl_1;

  }
  return fn;
}

//draw the content of "result" in a .dat file
void output_result(std::string file_name){

    std::string path = "C:/Users/Tamwyn/Documents/Physik/ComputerPhysikUniKonstanz2023/Blatt_5/Results/";
    std::ofstream file(path + file_name, std::ios::trunc);
    if(file.is_open())
    {
        std::cout<<" Output File found \n";
        file<< "Position" << " " << "Amplitude"<<" "<<"Analytical"<<"\n" ;
        for ( Wavefunction row : results)
        {
            file<< row.position << " "<< row.amplitude << "\n";
        }
        return;
    }
    std::cout<<" Cant find or open file \n";
    return;
    
}

//newton process to determine the 0 positions in the energie graph
double newton(double start_x){
  double alpha_n = start_x;
  double dist = 100000.;
  double a, b, alpha_min_1, id;

   while (dist > 1e-10)
   {
    id = find_index_at_position(alpha_n);
    //std::cout<<"ID:"<<id;

  //draw a line between two points = tengent
    a = (results[id+1].amplitude - results[id].amplitude) / (H);
    b = results[id].amplitude - a*results[id].position;

    alpha_min_1 = alpha_n;
    //new x is tangent = 0
    alpha_n = -b/a;
    
    //updates distance between last and new point. If too close we are done
    dist = abs( alpha_n - alpha_min_1 );
  
  }
  
  return alpha_n;
   
  
}

int main(int argc, char *argv[]){

    double E = 0;
    double fn;
    //generate energie curve
    for (double i = 0; i < 20; i+=0.001)
    {
      fn = shoot_numerov(E+i, false);

      //energiecurve doesnt need analytical value, purely simulated
      save_result(E+i, fn, 0);
    }

    //searching for the *exact* nullpoints arround those E values in the curve
    //we can find them by taking a look in the generated data from the above gen. energie curve
    double E_0 = newton(6.31);
    double E_1 = newton(7.15);
    double E_2 = newton(7.96);
    double E_3 = newton(8.74);
    double E_4 = newton(9.51);

    flush_save_buffer();


    //make some place in the terminal
    std::cout<<"\n";
    
    //set of the eigenenergies
    std::vector<double> energies = {E_0, E_1, E_2, E_3, E_4};
    double i = 0;

    //draw wavefunction curve for each eigenenergie
    for (double E_i : energies)
    {
      std::cout<<"Generating Energie E_"<<i<<":"<<E_i<<"\n";

      shoot_numerov(E_i, true);

      output_result("Psi__E"+ std::to_string(i)+"__.dat");

      flush_save_buffer();
      i++;
    }
    
    return 0;
}