#include <fstream>
#include <iostream>
#include <math.h>
#include <vector>

#define M_PI 3.14159265358979323846 // pi

struct R3 {
  double x;
  double y;
  double z;

  // addition
  R3 operator+(const R3 &other) const {
    R3 result;
    result.x = x + other.x;
    result.y = y + other.y;
    result.z = z + other.z;
    return result;
  }

  // substraction
  R3 operator-(const R3 &other) const {
    R3 result;
    result.x = x - other.x;
    result.y = y - other.y;
    result.z = z - other.z;
    return result;
  }
  // scalar porduct
  double operator*(const R3 &other) const { return x * other.x + y * other.y; }

  // real mutliplication
  R3 operator*(const double &other) const {
    R3 result;
    result.x = x * other;
    result.y = y * other;
    result.z = z * other;
    return result;
  }
};

double norm(R3 *vector) {
  return std::sqrt(std::pow((vector->x), 2) + std::pow((vector->y), 2)+ std::pow((vector->z), 2));
}
double norm(R3 vector) {
  return std::sqrt(std::pow((vector.x), 2) + std::pow((vector.y), 2) + std::pow((vector.z), 2));
}

class Magnet {
public:
  R3 location;
  Magnet(int index, int total, double radius);
  R3 force(R3 *particle_loc) {
    R3 force =
        ((*particle_loc) - (location)) * (1 / norm((*particle_loc) - location));
    //std::cout << "Magnet force X:" << force.x << " Y: " << force.y;
    return force;
  };
};
std::ofstream file_pendel("C:/Users/Tamwyn/Documents/Physik/ComputerPhysikUniKonstanz2023/Blatt_4/PendelLocation.dat");
Magnet::Magnet(int index, int total, double radius) {
  location.x = radius * std::cos((index * 2 * M_PI) / total);
  location.y = radius * std::sin((index * 2 * M_PI) / total);
  location.z = 0.;
  file_pendel << location.x<<" "
            << location.y <<" " <<0.<< "\n";
}

// simulation params
double max_time = 1000.;
double iteration_per_sec = 75.;
double h = 1 / iteration_per_sec;

// pysical params
double gamma = 0.2;
double k = 0.5;
double mass = 1;

// magnets
std::vector<Magnet> all_magnets;

// simulation start values
R3 speed;
R3 location;

R3 acceleration(R3 *location, R3 *speed) {
  R3 force_magnets;
  for (Magnet magnet : all_magnets) {
    force_magnets = force_magnets + magnet.force(location);
  }
  return force_magnets - (*speed) * gamma - (*location) * k;
}

void leap_frog(R3 *old_location, R3 *old_speed) {
  R3 temp_speed;
  (*old_location) = (*old_location) + (*old_speed) * h;
  (*old_speed) = (*old_speed) + acceleration(old_location, old_speed) * h;
  return;
}

int main(int argc, char *argv[]) {
  speed.x = 0;
  speed.y = 0;
  speed.z = 0.;
  location.x = -2.2;
  location.y = 2.2;
  location.z = 0.25;
  int num_magnets;
  std::cout << "How many pendels"
            << "\n";
  std::cin >> num_magnets;
  for (int i = 0; i < num_magnets; i++) {
    all_magnets.push_back(Magnet(i + 1, num_magnets, 1.));
  }
  R3 last_Loc;
  std::ofstream file("C:/Users/Tamwyn/Documents/Physik/ComputerPhysikUniKonstanz2023/Blatt_4/PendelResult_2.dat");
  
  file << "Time "
       << "x_x "
       << "x_y "
       << "x_z "
       << "v_x "
       << "v_y "
       << "\n";
  if (file.is_open()) {
    for (double time = 0; time < max_time; time = time + h) {
      last_Loc = location; 
      leap_frog(&location, &speed);
      if ((norm(location - last_Loc) < std::pow(10.,-23)) && (time>max_time/10))
      {
        std::cout<<"BREAK "<<norm(location - last_Loc)<<"\n";
        break;
      }
      file << time << " " << (location.x) << " " << (location.y) << " "<< (location.z) << " "
           << (speed.x) << " " << (speed.y) << "\n";
    }
  } else {
    std::cout << "Cant open file"
              << "\n";
  }
  return 0;
}
