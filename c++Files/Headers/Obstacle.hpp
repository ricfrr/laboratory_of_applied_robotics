#ifndef Obstacle_hpp
#define Obstacle_hpp
#include "Square.hpp"
#include "Triangle.hpp"
#include "Pentagon.hpp"
#include "Hexagon.hpp"
#include "Shape.hpp"
#include <vector>

using namespace cv;

class Obstacle : public Shape
{

public:
  Obstacle();
  ~Obstacle();

  void findObstacles(const Mat &img); // return a list of obstacles
  
  std::vector<Triangle> getTriangles();
  std::vector<Square> getSquares();
  std::vector<Pentagon> getPentagons();

private:
  const int epsilon_approx = 7;
  std::vector<Square> squares;
  std::vector<Triangle> triangles;
  std::vector<Pentagon> pentagons;
  std::vector<Hexagon> hexagons;
};

#endif /* Obstacle_hpp */
