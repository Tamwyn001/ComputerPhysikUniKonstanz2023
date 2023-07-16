#include <fstream>
#include <iostream>
#include <math.h>
#include <vector>

#define HBAR 1.054571817e-34;

double xmax = 10.;
double step = 0.001;
double start_pos = 0.;

double mass = 1.;

struct Wavefunction
{
    double position;
    double amplitude;
};

std::vector<Wavefunction> results;

double F (float x){
    return 0.;
}

double potential(double x){
    if(x<0){
        return INFINITY;
    }
    return x;
}
double k2 (double x, double m, double E){
    return 2*m/HBAR + (E - potential(x));
}

double numerov(double E){

    double fn = start_pos + step, fn_min_1 = start_pos, fn_pl_1;

    int i = 0;
    results[i++].amplitude = fn;
    results[i].position = start_pos;

    double k2_temp = 0.;
    double h_12 = step*step/12;

    for (double position = start_pos; position < xmax; position += step)
    {
        k2_temp = k2(position, mass, E);
        fn_pl_1 = fn*(2-5./6.*step*step*k2_temp)-fn_min_1*(1+h_12*k2(position-h,mass,E))/(1+ h_12*k2_temp);
        
        fn_min_1 = fn;
        fn = fn_pl_1;

        results[i++].amplitude = fn;
    results[i].position = position;
    }
    

}
