#include <fstream>
#include <iostream>
#include <math.h>
#include <vector>


#include <array>
struct Point {
  double x;
  double y;
};

double DoublePrecision() {
  double e = 1.0;
  bool repeat = true;
  while (repeat) {
    e /= 2;
    double x = 1 + e;
    x -= 1.0;
    repeat = x > 0;
  }
  return e;
}

float FloatPrecision() {
  float e = 1.0;
  bool repeat = true;
  while (repeat) {
    e /= 2;
    float x = 1 + e;
    x -= 1.0;
    repeat = x > 0;
  }
  return e;
}

void CosLimit() {
  std::vector<Point> OutTable;
  std::ofstream OutData;
  OutData.open("CosGraph.dat", std::ios::out | std::ios::trunc);
  OutData << "  x  |  y  "
          << "\n";

  for (int i = 0; i < 1000; i++) {
    Point Temp;
    Temp.x = 1 / i;
    Temp.y = cos(Temp.x - 1) / (Temp.x * Temp.x);
    OutTable.push_back(Temp);
    OutData << Temp.x << " | " << Temp.y << "\n";
  }
  return;
}

int Facotrial(int n) {
  if (n == 0)
    return 1;
  int out;
  for (int i = 1; i <= n; i++) {
    out *= i;
  }
  return out;
}

float Deteminant(std::vector<std::vector<float>> Matrix) {
    return (Matrix[0][0]*Matrix[1][1]-Matrix[1][0]*Matrix[0][1]);

}

std::vector<std::vector<float>> ReadMatrixFromFile(std::string FileName){
    std::ofstream FileToOpen;
    FileToOpen.open(FileName , std::ios::in);
    


}
int main(int argc, char *argv[]) {23
  std::cout << "Double:" << DoublePrecision() << "\n";
  std::cout << "Float:" << FloatPrecision() << "\n";
  CosLimit();

  int PascalDepth;
  std::cout << "Which depth should the pascal triangle have: \n";
  std::cin >> PascalDepth;
  int StageWidth = 1;
  std::vector<std::vector<int>> Triangle;
  for (int i = 0; i < PascalDepth; i++) {
    std::vector<int> PascalStage;
    for (int j = 0; j < StageWidth; i++) {
      int PascalElem = (i) / (Facotrial(j) * Facotrial(i - j));
      PascalStage.emplace_back(PascalElem);
    }
    Triangle.emplace_back(PascalStage);
    StageWidth += 2;
  }
  return 0;
}