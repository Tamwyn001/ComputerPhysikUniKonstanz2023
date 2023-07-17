#include <fstream>
#include <iostream>
#include <math.h>
#include <vector>
#include <string>

const double HBAR = 1; //1.054571817e-34;

const double xmax = 20.;
const double H = 0.001;
const double start_pos = 0.;

const double mass = 1.;

struct Wavefunction {
  double position;
  double amplitude;
};

std::vector<Wavefunction> results;

bool is_in_ball(double center, double radius, double value){
  //std::cout<<value - center <<"\n";
  return abs(value - center ) <= radius;
}

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

double potential(double x) {
  if (x < 0) {
    return INFINITY;
  }
  return x;
}

double k2(double x, double m, double E) {
  return 2 * m / HBAR * ( E - potential(x));
}

void save_result(double position, double amplitude){
  Wavefunction temp_result;
  temp_result.position=position;
  temp_result.amplitude = amplitude;
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
    //avoid x<0, otherwise fn explodes and the porcess is not defined anymore. we allready know psi(x)|x<0 = 0..
    if (position-H < 0.)
    {
      position = H;
    }

    if (save_values)
    {
      save_result(position, fn);
    }
    
    fn_pl_1 = (fn * (2. - (5. / 6.) * H * H * k2(position, mass, E)) - fn_min_1 * (1. + H * H / 12. * k2(position - H, mass, E))) / (1 + H*H /12. * k2(position+H, mass, E));

    fn_min_1 = fn;
    fn = fn_pl_1;

  }
  return fn;
}

void output_result(std::string file_name){

    std::string path = "C:/Users/Tamwyn/Documents/Physik/ComputerPhysikUniKonstanz2023/Blatt_5/Results/";
    std::ofstream file(path + file_name, std::ios::trunc);
    if(file.is_open())
    {
        std::cout<<" Output File found \n";
        file<< "Position" << " " << "Amplitude"<<"\n" ;
        for ( Wavefunction row : results)
        {
            file<< row.position << " "<< row.amplitude << "\n";
        }
        return;
    }
    std::cout<<" Cant find or open file \n";
    return;
    
}

double newton(double start_x){
  double alpha_n = start_x;
  double dist = 100000.;
  double a, b, alpha_min_1, id;

   while (dist > 1e-10)
   {
    id = find_index_at_position(alpha_n);
    //std::cout<<"ID:"<<id;

    a = (results[id+1].amplitude - results[id].amplitude) / (H);
    b = results[id].amplitude - a*results[id].position;

    alpha_min_1 = alpha_n;
    alpha_n = -b/a;
    //std::cout<<"New point at:"<<alpha_n<<" with a:"<<a<<" b: "<<b<<"\n";

    dist = abs( alpha_n - alpha_min_1 );
    //std::cout<<" from "<< alpha_min_1<<" to "<<alpha_n<<" dist:"<<dist<<"\n";
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
      save_result(E+i, fn);
    }

    //searching for the *exact* nullpoints arround those E values in the curve
    double E_0 = newton(6.31);
    double E_1 = newton(7.15);
    double E_2 = newton(7.96);
    double E_3 = newton(8.74);
    double E_4 = newton(9.51);

    flush_save_buffer();

    std::vector<double> energies = {E_0,E_1,E_2,E_3,E_4};
    double i = 0;
    for (double E_i : energies)
    {
      std::cout<<"Generating Energie E_"<<i<<":"<<E_i<<"\n";
      shoot_numerov(E_i, true);

      output_result("Psi__E"+ std::to_string(i)+"__.dat");
      flush_save_buffer();
      i++;
    }
    
    // shoot_numerov(6.31);
    // double position;
    // while (true)
    // {
    //   std::cin>>position;
    //   std::cout<<"at ID"<<find_index_at_position(position)<<"\n";
    // }
    
   
    
    return 0;
}