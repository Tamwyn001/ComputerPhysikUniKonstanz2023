#include <iostream>
#include <vector>

  // physical systems constants
  // feder constant
  const double k = 1.0;

  // mass
  const double m = 1.0;
struct Data
{
  double location;
  double speed;
};

struct Point {
  double time;
  Data speed_loc;
  double energie;
};

// explizit verfahren
void euler(Data* Value, double interval) 
{
  Data Old;
  Old = (*Value);
  Value->location = Old.location + interval * (Old.speed);
  Value->speed = (Old.speed) - interval * (k / m) * (Old.speed);
}

int main(int argc, char *argv[]) 
{
  Point value;
  value.speed_loc.location = 0.0;
  value.speed_loc.speed = 1.0;

  // simulation constant
  // time intervall

  const double iterations = 1000.;
  const double duration = 10.;

  double interval = duration / iterations;
  std::vector<Point> simulation_result;

  for (int i = 0; i < iterations; i++) {
    euler(&value.speed_loc,interval);
    value.time = value.time + interval;
    value.energie = ((value.speed_loc.speed) * (value.speed_loc.speed) + (k / m) * (value.speed_loc.location) * (value.speed_loc.location)) / 2.0;
    simulation_result.push_back(value);
    std::cout << i * interval << " " << value.speed_loc.location <<" "<<value.speed_loc.speed << " " << value.energie << "\n";
  }

  return 0;
}