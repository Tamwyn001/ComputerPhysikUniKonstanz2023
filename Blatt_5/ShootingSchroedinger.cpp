#include <fstream>
#include <iostream>
#include <math.h>
#include <vector>
#include <string>
#include "C:\lib\C++\AlgLib\include\specialfunctions.h"

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
double analytical_psi(double position, double energie){
  double ai, aip, bi,bip;
  double x = pow(2*mass/(HBAR*HBAR), 1./3.);
  alglib::airy(x*(position - energie), ai, aip, bi, bip);
  return ai;
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

//scaling factor for the curve to norm it
double norm_factor(std::vector<Wavefunction>* Datas, int target)
{
  double summ = 0;
  for (Wavefunction data : (*Datas))
  {
    switch (target)
    {
    case 1:
    if (data.amplitude < 1000)
    {
      summ += data.amplitude;
    }
      break;
    case 2:
      summ += data.analytical;
      break;

    default:
      break;
    }
    //std::cout <<summ<<" ";
  }
  
  return abs(1. / summ);
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

double shoot_numerov(double E, bool save_values, int id) {
  
  double fn = start_pos + H, fn_min_1 = start_pos, fn_pl_1;
 
  double sign;
  if (id % 2)
  {
    sign = -1.;
  }
  else
  {
    sign = 1.;
  }
  
  for (double position = start_pos; position < xmax; position += H)
  {
    //avoid x<0, otherwise fn explodes and the porcess outputs "nan" (recursivity). we allready know psi(x)|x<0 = 0..
    if (position-H < 0.)
    {
      position = H;
    }

    if (save_values)
    {
      //we save tht datas.. the -1^i is to invert the curve of odd curves to fit to the simulation.
      // This sign change comes from the start condition but the forme of the curve is good 
      save_result(position, fn, sign*analytical_psi(position, E));
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
void output_result(std::string file_name, bool norm){

    std::string path = "C:/Users/Tamwyn/Documents/Physik/ComputerPhysikUniKonstanz2023/Blatt_5/Results/";
    std::ofstream file(path + file_name, std::ios::trunc);
    if(file.is_open())
    {
        std::cout<<" Save success\n";
        file<< "Position" << " " << "Amplitude"<<" "<<"Analytical"<<"\n" ;

        double scaling_ampl = 1.;
        double scaling_ana = 1.;
        if(norm)
        {
          scaling_ampl = norm_factor(&results, 1);
          scaling_ana = norm_factor(&results, 2);
          file << " - " << " " << scaling_ampl <<" "<< scaling_ana <<"\n" ;
        }

        for ( Wavefunction row : results)
        {
            file << row.position << " " << row.amplitude * scaling_ampl << " " << row.analytical*scaling_ana << "\n";
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

  //!we can be more accurate than the points, so we just draw a line once
  //  while (dist > 1e-6)
  //  {
    id = find_index_at_position(alpha_n);
    //std::cout<<"ID:"<<id;

  //draw a line between two points = tengent
    a = (results[id+1].amplitude - results[id].amplitude) / (H);
    b = results[id].amplitude - a*results[id].position;

    alpha_min_1 = alpha_n;
    //new x is tangent = 0
    alpha_n = -b/a;
    
    //updates distance between last and new point. If too close we are done
    //dist = abs( alpha_n - alpha_min_1 );
    //std::cout<<"dist: "<<dist<< " : "<< alpha_n <<"-" << alpha_min_1<<"\n";
  //}
  
  return alpha_n;
}


int main(int argc, char *argv[]){

    double E = 0;
    double fn;
    //generate energie curve
    for (double i = 0; i < 20; i+=0.001)
    {
      fn = shoot_numerov(E+i, false, 2);

      //energiecurve doesnt need analytical value, purely simulated
      save_result(E+i, fn, 0);
    }
    output_result("ShootingTestE.dat", false);

    //searching for the *exact* nullpoints arround those E values in the curve
    //we can find them by taking a look in the generated data from the above gen. energie curve
    //we interate in the energie list and search for sign change

    //the newton algo need to know where to iterate, like wich startvalue
    std::vector<double> newton_to_check;
    fn = 0.;
    int i = 0;
    for(Wavefunction entry : results)
    {
      
      //we reuse fn and fnm_1 to search for sign change. If we change sign: abs(a - b) > abs(a).. trust me.. ok dont trust me

      //we realy check for sign change between the value and the last value
      //trivial report
      if(entry.amplitude == 0)
      {
         if (i < 5)
        {
        newton_to_check.push_back(entry.position);
        i++;
        }
      }
      else if(((entry.amplitude > 0) && (fn < 0 ) ) || ( (entry.amplitude < 0) && (fn > 0)) )
      {
        if (i < 5)
        {
          newton_to_check.push_back(entry.position);
          i++;
        } 
      }
      //we shift the fn to the current value and do the same with next reulst.pos
      fn = entry.amplitude;

      //stop after find 5 energies
      if (i>5)
      {
        break;
      }
      
    }
    //the eigenenergies of the hamiltonian aka solutions for the wave function
    std::vector<double> eigen_energies;
    i = 0;
    for(double E_start_i : newton_to_check)
    {
      double E_i = (newton(E_start_i));
      eigen_energies.push_back(E_i);
      std::cout<<"Energie "<<i<<" is: "<<E_i <<"\n";
      i++;
    }


    //we then want to save the wavefunction so we delete the enerie save
    flush_save_buffer();


    //make some place in the terminal
    std::cout<<"\n";
    

    //draw wavefunction curve for each eigenenergie with the flag i
    i = 0;
    for (double E_i : eigen_energies)
    {
      std::cout<<">Generating Energie E_"<<i<<" .. ";

      //generate the wavefunction and save it the the buffer
      shoot_numerov(E_i, true, i);  

      //save and flush the data for the next wavefunction
      output_result("Psi__E"+ std::to_string(i)+"__.dat", false);

      flush_save_buffer();
      i++;
    }
    
    return 0;
}