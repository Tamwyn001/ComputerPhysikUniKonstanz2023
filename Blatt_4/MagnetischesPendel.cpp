#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

#define M_PI 3.14159265358979323846 // pi

//Datenstruktur eines R3 Vektor
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

//Mathematische Norm eines R3 Vektor
double norm(R3 *vector) {
  return std::sqrt(std::pow((vector->x), 2) + std::pow((vector->y), 2) +
                   std::pow((vector->z), 2));
}
double norm(R3 vector) {
  return std::sqrt(std::pow((vector.x), 2) + std::pow((vector.y), 2) +
                   std::pow((vector.z), 2));
}

//Ein Klasse für Magneten, um die Kraft zu berechen und später die Konvergenz gegen Magnet mithilfe
// einer "Magnet Index" vgereinfachen

class Magnet {
public:
  R3 location;
  const double magnetical_strength = 1000.;
  int index;

  Magnet(int id, int total, double radius);

  //magntische Kraft
  R3 force(R3 *particle_loc) {
    R3 connection_vector = (location) - (*particle_loc);
    connection_vector.z = 0; // projection on the plane
    R3 force = (connection_vector) *
               (1 / std::pow(norm(location - (*particle_loc)), 3)) *
               magnetical_strength;

    return force;
  };
};

//Konstruktor um die Position auf dem Einheitskreis zu teilen
Magnet::Magnet(int id, int total, double radius) {
  index = id;
  location.x = radius * std::cos((index * 2 * M_PI) / total);
  location.y = radius * std::sin((index * 2 * M_PI) / total);
  location.z = 0.;
  
}

// simulation params
double max_time = 1000.;
double iteration_per_sec = 350.;
double h = 1 / iteration_per_sec;

// pysical params
double gamma = 0.2;
double k = 0.1;
double mass = 1;

// magnets
std::vector<Magnet> all_magnets;

// simulation start values
R3 speed;
R3 location;

//Summe der Kräft nach Newton
R3 acceleration(R3 *location, R3 *speed) {
  R3 force_magnets;
  for (Magnet magnet : all_magnets) {
    force_magnets = force_magnets + magnet.force(location);
  }
  return (force_magnets - (*speed) * gamma - (*location) * k) * (1 / mass);
}

//das Leap Frog Verfahren
void leap_frog(R3 *old_location, R3 *old_speed) {
  R3 temp_speed;
  (*old_location) = (*old_location) + (*old_speed) * h;
  (*old_speed) = (*old_speed) + acceleration(old_location, old_speed) * h;
  return;
}

//Aufhörenbedingung des Simulation
bool does_pendel_move(R3 *pendel_location, std::vector<Magnet> *all_magnets, bool * is_successful_end, int * count) {
  double distance = 10000.;
  double temp_distance;
  if ((*count) > 10000)
  {
    std::cout<<"Sinulation canceled"<< "\n";
    (*is_successful_end) = false;
    return false;
  }
  //wir rechnen  die Distanz zu jeden Magneten und nehmen die kleinste
  for (Magnet comparing_magnet : *all_magnets) {
    R3 difference_vector = comparing_magnet.location - (*pendel_location);
    difference_vector.z = 0;

    temp_distance = norm(difference_vector);

    if (temp_distance < distance) {
      distance = temp_distance;
    
    //das Teilchen ist als gefangen betrachten, wenn sie nah am Pendel fliegt
      if (distance < 0.001) {

        (*is_successful_end) = true;
        return false;
      }
    }
  }
  (*is_successful_end) = false;
  return true;
}

//greift bei welchen Magnet das pendel gefangen ist.
int retrive_pendel(R3 *location, std::vector<Magnet> *Magnets) {
  double last_distance = 10000;
  double temp_dist;
  int out_id = 0;

  for (Magnet comparing_magnet : *Magnets) {
    temp_dist = norm((*location - comparing_magnet.location));

    if (temp_dist < last_distance) {
      last_distance = temp_dist;
      //std::cout<<" ID:"<<comparing_magnet.index<<" dist:"<<last_distance<<" ";
      out_id = comparing_magnet.index;
    }
  }

  //std::cout<<"Out ID:"<< out_id;
  return out_id;
}

//das simulation für eine Pendel
void calculate_trajectorie(R3 *location, R3 *speed,
                           std::vector<Magnet> *all_magnets,
                           std::string pendel_id, bool * is_successful_end) {
  std::string path =
      "C:/Users/Tamwyn/Documents/Physik/ComputerPhysikUniKonstanz2023/Blatt_4/"
      "PendelResult_" +
      pendel_id + ".dat";
  std::ofstream file(path, std::ios::trunc);
  if (file.is_open()) {
    file << "Time "
         << "x_x "
         << "x_y "
         << "x_z "
         << "v_x "
         << "v_y "
         << "\n";
    double time = 0;
    int count = 0;

    //schleife für jede zeitschritt
    while (does_pendel_move(location, all_magnets, is_successful_end, &count)) {
      
      time += h;

      leap_frog(location, speed);
      (*location).z = 0.25;
      file << time << " " << ((*location).x) << " " << ((*location).y) << " "
           << ((*location).z) << "\n";
      count++;
    }
    //std::cout << "End (*location) is:" << (*location).x << " ; "
    //         << (*location).y << "\n";
  } else {
    std::cout << "Cant open file"
              << "\n";
  }
  return;
}
int main(int argc, char *argv[]) {

  bool is_successful_end;
  int num_magnets;
  int mode;
  std::cout << "Compute image (1) or trajectorie (0)"
            << "\n";
  std::cin >> mode;
  std::cout << "How many pendels"
            << "\n";
  std::cin >> num_magnets;

  std::ofstream file_pendel(
      "C:/Users/Tamwyn/Documents/Physik/"
      "ComputerPhysikUniKonstanz2023/Blatt_4/PendelLocation.dat",
      std::ios::trunc);
  std::ofstream file_matrix("C:/Users/Tamwyn/Documents/Physik/"
                            "ComputerPhysikUniKonstanz2023/Blatt_4/Matrix.dat",
                            std::ios::trunc);

  std::ofstream file_lerp("C:/Users/Tamwyn/Documents/Physik/"
                            "ComputerPhysikUniKonstanz2023/Blatt_4/Matrix_Lerp.dat",
                            std::ios::trunc);

  for (int i = 0; i < num_magnets; i++) {
    Magnet new_magnet = Magnet(i + 1, num_magnets, 1.);
    all_magnets.push_back(new_magnet);
    file_pendel << new_magnet.location.x << " " << new_magnet.location.y << " "
                << new_magnet.index << "\n";
  }
  R3 last_Loc;

  //nur für eine Pendel
  if (mode == 0) {
    speed.x = 0;
    speed.y = 0;
    speed.z = 0.;
    location.x = 1.15;
    location.y = -0.63;
    location.z = 0.25;
    std::string tag = "chaos";
    calculate_trajectorie(&location, &speed, &all_magnets, tag, &is_successful_end);
  } else {
    //Bild Erstellung
    int resolution;

    std::cout << "Resolution:"
              << "\n";
    std::cin >> resolution;
    std::string tag;
    std::string gnuplot_command = "plot ";
    for (int j = 0; j < resolution; j++) {
      for (int i = 0; i < resolution; i++) {
        speed.x = 0.;
        speed.y = 0.;
        speed.z = 0.;
        location.x = -1 + double(2*i) / double(resolution-1);
        location.y = -1 + double(2*j) / double(resolution-1);
        //std::cout << "Start x: " << location.x << " "
        //          << "Start y: " << location.y << "\n";
        location.z = 0.25;

        tag = std::to_string(i) + "_" + std::to_string(j);
        calculate_trajectorie(&location, &speed, &all_magnets, tag, &is_successful_end);
        gnuplot_command = gnuplot_command + "\"PendelResult_" +
                          std::to_string(i) + "_" + std::to_string(j) +
                          ".dat\" u 2:3 w l lw 4 title \"Particle " +
                          std::to_string((i + 1) * (j + 1)) + "\", ";
        if (is_successful_end)
        {
          //Abbildungsmatrix
           file_matrix<<retrive_pendel(&location, &all_magnets)<< " ";
        }
        else
        {
           file_matrix<< 0 << " ";
        }
       
      }
      std::cout << "\n";
      file_matrix << "\n";
    }
    std::cout
        << gnuplot_command
        << "\"PendelLocation.dat\" u 1:2 w p pointsize 4 title \"Magnets\""
        << "\n";
  }

  return 0;
}
