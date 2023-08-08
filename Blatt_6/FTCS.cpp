#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <string>

double delta_t = 0.001;
double total_time = 100.;

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
};

std::vector<Data> values_at_time;
std::vector<Data> values_next_time;

double gauss_curve(double location, double peak_loc, double sigma){
    return exp(-pow((location - peak_loc), 2)/(2*sigma*sigma));
}


void start_state(std::vector<Data>* values){
    for(double location = start_loc; location < end_loc; location += delta_x)
    {
        Data new_sim_point;
        new_sim_point.location = location;
        new_sim_point.concentration = gauss_curve(location, 0.5, 0.02);

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

void write_state_to_file(std::string path, std::vector<Data>& output_data){

  std::ofstream file(path, std::ios::trunc);
  //row names
  for(Data point_data : output_data)
  {
    file << point_data.location <<" "<< point_data.concentration
        <<"\n";
  }
}

int main(int argc, char* argv[])
{
    start_state(&values_at_time);
    std::string path =
    "C:/Users/Tamwyn/Documents/Physik/ComputerPhysikUniKonstanz2023/Blatt_6/"
    "Results/FTCS_0.000000.dat";
    write_state_to_file(path, values_at_time);

    int c = 1;
    //iterate over time
    std::cout<<"plot \"FTCS_0.000000.dat\" w l lw 3 linetype 6 notitle, ";
    for (double t = delta_t; t < total_time; t += delta_t )
    {
        Data temp_data;

        //border condition start
        temp_data.location = 0.;
        temp_data.concentration = 0.;
        values_next_time.push_back(temp_data);

        //for each timeframe we generate the curve
        for (int i = 1; i < loc_iteration - 1; i++)
        {
            //save location and concentration at each point of the curve
            temp_data.location = i*delta_x;
            temp_data.concentration = FTCS_at_position(i);

            values_next_time.push_back(temp_data);
        }
        //border condition end
        temp_data.location = loc_iteration*delta_x;
        temp_data.concentration = 0.;
        values_next_time.push_back(temp_data);

        //std::cout<<"\n";
        values_at_time = values_next_time;
        values_next_time.clear();
        //std::cout<<"\n";
        std::string path = "C:/Users/Tamwyn/Documents/Physik/ComputerPhysikUniKonstanz2023/Blatt_6/"
            "Results/FTCS_"+ std::to_string(t) +".dat";
        if (c % 1000 == 0)
        {
            write_state_to_file(path, values_at_time);
            std::cout<<"\"FTCS_"<< std::to_string(t)<<".dat\" w l lw 3 linetype "<< c % 7<<" notitle, ";
        }
        // for (Data toshow_data : values_at_time)
        // {
        //     std::cout<<toshow_data.concentration<< " ";
        // }


    c++;
    }


    return 0;
}



