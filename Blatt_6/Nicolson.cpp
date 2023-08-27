#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

//is it possible to do matricies with the [][]?
#include <vector>

//time intervall and total simulation time
const double delta_t = 0.001;
const double total_time = 30.;

//location boundaries and location precision
const double start_loc = 0.;
const double end_loc = 1.;
const double delta_x = 0.001;

const int total_iterator_loc = 1000; //(end_loc - start_loc)/delta_x
//diffusion constant
double D = 0.001;

double alpha ;

//output data
struct Data
{
    double location;
    double concentration;
    double analytical;
};

//the starting concentration is a gauss peak
double gauss_curve(double location, double peak_loc, double sigma){
    return exp(-pow((location - peak_loc), 2)/(2*sigma*sigma));
}

//the analitical solution to compare the data with
double ana_concentration(double x, double x_0, double t, double sigma)
{
    return (sigma/(sqrt(2*D*t + pow(sigma,2))))*exp( -pow(x-x_0,2) / (2*pow(sigma,2)) );
}

//write the given data in a .dat file 
void output_to_file(std::string path, Data data_to_print[], int lenght)
{
    std::ofstream file(path, std::ios::trunc);
    //row names
    for(int i = 0; i < lenght; i++)
    {
        file << data_to_print[i].location <<" "<< data_to_print[i].concentration
        <<"\n";
    }
    return;
}

//the computing of the state of the system at time t=0
void init_datas(Data values[])
{
    for (int i = 0; i < total_iterator_loc ; i ++)
    {
        Data to_add;
        to_add.location = start_loc + i*delta_x;
        to_add.concentration = gauss_curve(to_add.location, 0.5, 0.02);
        to_add.analytical = ana_concentration(to_add.location, 0.5, 0., 0.02);
        values[i] = to_add;
    }
}

//updates rhe coefficient of the tridiagonal matrix for the thomas algorythme
void setup_coeffs(double coeffs_b[], double coeffs_d[], Data simulation_values[])
{
    int i;
    //std::cout<<"\nB:"<<"\n";
    for (i = 0; i < total_iterator_loc; i++)
    {
        coeffs_b[i] = -2.*( 1. + 1. / alpha);
        //std::cout<<coeffs_b[i]<<" ";
    }
    for (i = 1; i < total_iterator_loc - 1; i++ )
    {
        coeffs_d[i] = 2.*(1. - 1./alpha) * simulation_values[i].concentration 
                    - simulation_values[i+1].concentration 
                    - simulation_values[i-1].concentration;   
    }

    coeffs_d[0] = 2. * (1. - 1./alpha) * simulation_values[0].concentration - simulation_values[1].concentration;
    coeffs_d[total_iterator_loc - 1] = 2. * (1. - 1./alpha)*simulation_values[total_iterator_loc - 1].concentration 
                                    - simulation_values[total_iterator_loc - 2].concentration;

}

//we asume that a[i]=c[i]=0? NO there are all = 1 else the curve just goes smaller but not flatter
void simplified_thomas_algo(double coeffs_b[], double coeffs_d[], Data simulation_values[])
{
    int i;
    // modification of b[i] and d[i] then a[i]=c[i]=1
    for (i = 1; i < total_iterator_loc; i++)
    {
        double m = 1 / coeffs_b[i - 1];
        coeffs_b[i] = coeffs_b[i] - m * 1;
        coeffs_d[i] = coeffs_d[i] - m * coeffs_d[i - 1];
    }

    simulation_values[total_iterator_loc - 1].concentration = coeffs_d[total_iterator_loc - 1] / coeffs_b[total_iterator_loc - 1];

    for (i = total_iterator_loc - 2; i >= 0 ; i--)
    {
        simulation_values[i].concentration = (coeffs_d[i] - 1 * simulation_values[i + 1].concentration) / coeffs_b[i];
    }
}


void boundary_condition(Data simulation_values[], double time)
{
    simulation_values[0].concentration = 0.;
    simulation_values[total_iterator_loc - 1].concentration = 0.;
    simulation_values[total_iterator_loc - 1].analytical = ana_concentration(0., 0.5, time, 0.02);
    simulation_values[total_iterator_loc - 1].analytical = ana_concentration(total_iterator_loc*delta_x, 0.5, time, 0.02);
    return;
}
    
// to change alpha we change the diffusionkonstant
void change_alpha()
{
    D += 0.00005;
}

//integrates under the curve to know how much matter is in the system to see if the system diverges
double compute_stability(Data simulation_values[])
{
    //the surface should remain the same if the volumes flats itself
    double total_surface = 0;
    for (int i = 0; i < total_iterator_loc ; i++)
    {
        total_surface += abs(simulation_values[i].concentration);
    }
    return total_surface;
}

//calculate the  difference between the theorie and the results of Nicolson
double compute_difference(Data simulation_values[]){
    double total_diff = 0.;
    for(int i = 0; i<total_iterator_loc; i++)
    {
        total_diff += simulation_values[i].concentration - simulation_values[i].analytical;
        //std::cout<<"concentr: "<<simulation_values[i].concentration<< " ana: "<<simulation_values[i].analytical<<"\n";
    }
    return total_diff;
}

void print_sim_data_and_command(int c, double t, Data values_at_time[]){
    if (c % 100 == 0)
            {
                std::string path = "C:/Users/Tamwyn/Documents/Physik/ComputerPhysikUniKonstanz2023/Blatt_6/"
                "Results/Nicolson Simulation Datas/Nicolson_"+ std::to_string(t) +".dat";
                output_to_file(path, values_at_time, total_iterator_loc);
                //2std::cout<<"\"Nicolson_"<< std::to_string(t)<<".dat\" w l lw 3 linetype "<< c % 7<<" notitle, ";
            }
}

//generate formula for a 3D plot in GnuPlot
void write_3D_plot(std::string path, std::vector<std::vector<Data>> matrix)
{
    std::ofstream file(path, std::ios::trunc);
    //iterate over alpha
    int i;
    for(std::vector<Data> it_vector : matrix)
    {
        i= 0;
        //iterate over time
        for(Data it_elem : it_vector)
        {

            //get difference value
            /**Data
             * loc = time
             * concentr = alpha
             * ana = difference
             */
             file<<it_elem.concentration<<" "<<it_elem.location<<" "<<it_elem.analytical<<"\n";
            
            i++;
        }
        file<<"\n";
    }
}
int main(int argc, char * argv[])
{
    //output files
     std::string path_stability =
    "C:/Users/Tamwyn/Documents/Physik/ComputerPhysikUniKonstanz2023/Blatt_6/"
    "Results/Nicolson_Stability.dat";

    std::string path_precision =
    "C:/Users/Tamwyn/Documents/Physik/ComputerPhysikUniKonstanz2023/Blatt_6/"
    "Results/Nicolson_Precision.dat";

    int i = 0;
    Data stability_nicolson[20];

    std::vector<Data> precision_values;
    Data precision_simulation;

    std::vector<std::vector<Data>> precision_values_alpha;

    for (D = 0.0001; D < 0.001; change_alpha())
    {
        int c = 0;
        //datas
        Data values_at_time[total_iterator_loc];

        Data stability_simulation;

        //coeffs for thomas
        double coeffs_b[total_iterator_loc];
        double coeffs_d[total_iterator_loc];

        alpha = D* delta_t/pow(delta_x, 2);

        init_datas(values_at_time);

        //time loop
        for (double t = 0; t < total_time; t += delta_t)
        {
            //data output
            print_sim_data_and_command(c,t, values_at_time);

            //curve diffusion computing
            setup_coeffs(coeffs_b, coeffs_d, values_at_time);
            simplified_thomas_algo(coeffs_b, coeffs_d, values_at_time);
            boundary_condition(values_at_time, t);

            //outputs datas at a specified timeinterval
            if (c % 1 == 0)
            {            
                precision_simulation.location = t;
                precision_simulation.concentration = alpha;
                precision_simulation.analytical = compute_difference(values_at_time);
                precision_values.push_back(precision_simulation);
                //std::cout<<"Added Point c="<<c<<" ";
            }
            c++;
        }

        precision_values_alpha.push_back(precision_values);
        precision_values.clear();

        //create a stability point
        
        stability_simulation.location = D * (delta_t)/(pow(delta_x, 2));
        stability_simulation.concentration = compute_stability(values_at_time);

        stability_nicolson[i] = stability_simulation;

        std::cout<<"Stability for D="<<D<<" done.\n";

        i++;
    }

    //save and empty the data
    output_to_file(path_stability, stability_nicolson, 20);
    write_3D_plot(path_precision, precision_values_alpha);
    return 0;
}