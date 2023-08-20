#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <string>

#define M_PI       3.14159265358979323846   // pi

double delta_t = 0.001;
double total_time = 30.;

double start_loc = 0.;
double end_loc = 1.;
double delta_x = 0.001;

double D = 0.0001;

//number of location iterations
double loc_iteration = (end_loc - start_loc) / delta_x;

struct Data
{
    double location;
    double concentration;
    double analytical;
};

std::vector<Data> values_at_time;
std::vector<Data> values_next_time;

double gauss_curve(double location, double peak_loc, double sigma){
    return exp(-pow((location - peak_loc), 2)/(2*sigma*sigma));
}

double ana_concentration(double x, double x_0, double t, double sigma)
{
    return (sigma/(sqrt(2*D*t + pow(sigma,2))))*exp( -pow(x-x_0,2) / (2*pow(sigma,2)) ); // /2*M_PI
}

void start_state(std::vector<Data>* values){
    for(double location = start_loc; location < end_loc; location += delta_x)
    {
        Data new_sim_point;
        new_sim_point.location = location;
        new_sim_point.concentration = gauss_curve(location, 0.5, 0.02);
        new_sim_point.analytical = ana_concentration(location, 0.5, 0., 0.02);

        values->push_back(new_sim_point);
    }
}

double FTCS_at_position(int index){

    double phi_n_right = values_at_time[index - 1].concentration;
    double phi_n_left = values_at_time[index + 1].concentration;
    double phi_n = values_at_time[index].concentration;
    //std::cout<<"phi_i-1:+ "<<phi_n_right<<" phi_n: "<< phi_n<<" phi_i+1: "<<phi_n_left<<"\n";
        //phi_i_n+1
    return phi_n + D * (delta_t)/(pow(delta_x, 2)) * (phi_n_left - 2 * phi_n + phi_n_right);

}

void write_state_to_file(std::string path, std::vector<Data>& output_data, int num_collumn){

  std::ofstream file(path, std::ios::trunc);
  //row names
  for(Data point_data : output_data)
  {
    switch (num_collumn)
    {
    case 2:
        file << point_data.location <<" "<< point_data.concentration
        <<"\n";
        break;
    case 3:
        file << point_data.location <<" "<< point_data.concentration<< " "<<point_data.analytical<<"\n";
    
    default:
        break;
    }
    
  }
}

void print_data_simulation(int c, double t){
    std::string path = "C:/Users/Tamwyn/Documents/Physik/ComputerPhysikUniKonstanz2023/Blatt_6/"
        "Results/FTCS_"+ std::to_string(t) +".dat";
    if (c % 1000 == 0)
    {
        write_state_to_file(path, values_at_time,3);
        std::cout<<"plot \"FTCS_"<< std::to_string(t)<<".dat\" w l lw 3 linetype "<< c % 7<<" title \"FCTS\", \"FTCS_"<< std::to_string(t)<<".dat\" w l lw 3 linetype 0 title \"Analytical\" "<<"\n";
    }
}

double compute_difference(std::vector<Data> simulation_values){
    double total_diff = 0.;
    for(Data it : simulation_values)
    {
        total_diff += it.concentration - it.analytical;
    }
    return total_diff;
}


int main(int argc, char* argv[])
{
    start_state(&values_at_time);
    std::string path_precision =
    "C:/Users/Tamwyn/Documents/Physik/ComputerPhysikUniKonstanz2023/Blatt_6/"
    "Results/FTCS_Precision.dat";

    std::vector<Data> precision_values;
    Data precision_simulation;
    int c = 1;
    //iterate over time
    for (double t = delta_t; t < total_time; t += delta_t )
    {
        
        Data temp_data;

        //Empties the saved datas of the previous simulation. Not at the end because we need to know the last state of the system
        //to compute the stability
        values_next_time.clear();
        values_at_time.clear();

        //border condition start
        temp_data.location = 0.;
        temp_data.concentration = 0.;
        temp_data.analytical = ana_concentration(start_loc, 0.5, 0., 0.02);
        values_next_time.push_back(temp_data);

        //for each timeframe we generate the curve
        for (int i = 1; i < loc_iteration - 1; i++)
        {
            //save location and concentration at each point of the curve
            temp_data.location = i*delta_x;
            temp_data.concentration = FTCS_at_position(i);
            temp_data.analytical = ana_concentration(i*delta_x, 0.5, t, 0.02);

            values_next_time.push_back(temp_data);
        }
        //border condition end
        temp_data.location = loc_iteration*delta_x;
        temp_data.concentration = 0.;
        temp_data.analytical = ana_concentration(loc_iteration*delta_x, 0.5, t, 0.02);
        values_next_time.push_back(temp_data);


        values_at_time = values_next_time;

        precision_simulation.location = t;
        precision_simulation.concentration = compute_difference(values_at_time);
        precision_values.push_back(precision_simulation);
        //std::cout<<"Total diff at time t="<<t<<"s is:"<< precision_simulation.concentration<<"\n";
        print_data_simulation(c, t);
        
    c++;
    }
    write_state_to_file(path_precision, precision_values, 2);

    

    

    return 0;
}
