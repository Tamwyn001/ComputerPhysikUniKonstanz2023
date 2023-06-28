#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

#define M_PI 3.14159265358979323846 // pi

struct R3 {
  double x = 0;
  double y = 0;
  double z = 0;

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
  return std::sqrt(std::pow((vector->x), 2) + std::pow((vector->y), 2) +
                   std::pow((vector->z), 2));
}
double norm(R3 vector) {
  return std::sqrt(std::pow((vector.x), 2) + std::pow((vector.y), 2) +
                   std::pow((vector.z), 2));
}

class Magnet {
public:
  R3 location;
  const double magnetical_strength = 10.;

  Magnet(int index, int total, double radius);
  R3 force(R3 *particle_loc) {
    R3 connection_vector = (location) - (*particle_loc);
    connection_vector.z = 0; // projection on the plane
    R3 force = (connection_vector) *
               (1 / std::pow(norm(location - (*particle_loc)), 3)) *
               magnetical_strength;

    return force;
  };
};

Magnet::Magnet(int index, int total, double radius) {
  location.x = radius * std::cos((index * 2 * M_PI) / total);
  location.y = radius * std::sin((index * 2 * M_PI) / total);
  location.z = 0.;
}

// simulation params
double max_time = 1000.;
double iteration_per_sec = 7500.;
double h = 1 / iteration_per_sec;

// pysical params
double gamma = .2;
double k = 0.1;
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
  return (force_magnets - (*speed) * gamma - (*location) * k) * (1 / mass);
}

void leap_frog(R3 *old_location, R3 *old_speed) {
  R3 temp_speed;
  (*old_location) = (*old_location) + (*old_speed) * h;
  (*old_speed) = (*old_speed) + acceleration(old_location, old_speed) * h;
  return;
}
bool does_pendel_move(R3 *pendel_location, std::vector<Magnet> *all_magnets) {
  double distance = 10000.;
  double temp_distance;

  for (Magnet comparing_magnet : *all_magnets) {
    R3 difference_vector = comparing_magnet.location - (*pendel_location);
    difference_vector.z = 0;

    temp_distance = norm(difference_vector);

    if (temp_distance < distance) {
      // std::cout<<"Magnet Loc x"<<comparing_magnet.location.x<<distance<<"\n";
      distance = temp_distance;
      if (distance < 0.001) {
        return false;
      }
    }
  }
  return true;
}

int main(int argc, char *argv[]) {
  speed.x = 0;
  speed.y = 0;
  speed.z = 0.;
  location.x = 1;
  location.y = -0.73;
  location.z = 0.25;
  int num_magnets;
  std::cout << "How many pendels"
            << "\n";
  std::cin >> num_magnets;

  std::ofstream file(
      "C:/Users/Tamwyn/Documents/Physik/ComputerPhysikUniKonstanz2023/Blatt_4/"
      "PendelResult.dat",
      std::ios::trunc);
  std::ofstream file_pendel(
      "C:/Users/Tamwyn/Documents/Physik/"
      "ComputerPhysikUniKonstanz2023/Blatt_4/PendelLocation.dat",
      std::ios::trunc);

  for (int i = 0; i < num_magnets; i++) {
    Magnet new_magnet = Magnet(i + 1, num_magnets, 1.);
    all_magnets.push_back(new_magnet);
    file_pendel << new_magnet.location.x << " " << new_magnet.location.y
                << " " << 0. << "\n";
  }
  R3 last_Loc;

  file << "Time "
       << "x_x "
       << "x_y "
       << "x_z "
       << "v_x "
       << "v_y "
       << "\n";
  if (file.is_open()) {

    double time = 0;
    int counter = 0;
    while (does_pendel_move(&location, &all_magnets)) {
      time += h;

      leap_frog(&location, &speed);
      location.z = 0.25;
      file << time << " " << (location.x) << " " << (location.y) << " "
           << (location.z) << " " << (speed.x) << " " << (speed.y) << "\n";
    }
    std::cout << "End location is:" << location.x << " ; " << location.y
              << "\n";
  } else {
    std::cout << "Cant open file"
              << "\n";
  }
  return 0;
}
