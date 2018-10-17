#ifndef Map_hpp
#define Map_hpp
#include "Arena.hpp"
#include "Cell.hpp"
#include "ExitPoint.hpp"
#include "Hexagon.hpp"
#include "Obstacle.hpp"
#include "Pentagon.hpp"
#include "Square.hpp"
#include "Triangle.hpp"
#include "People.hpp"
#include "Circle.hpp"
#include <vector>


using namespace cv;

class Map {

public:
  Map();
  ~Map();

  void createMap(const Mat &img);

private:
  // grid of the map
  // create a map with empty cells
  void initializeGrid(Arena &arena, ExitPoint &exit_point, Obstacle &obstacles);
  // detect if a pixel is in contact with an object
  void findMaxMinY(int &max_y, int &min_y, const std::vector<cv::Point> poly);
  bool contact(std::vector<cv::Point> cell, const std::vector<cv::Point> poly);

  bool isOutofArena(std::vector<cv::Point> corners, Arena arena);

  void checkObstacles(Cell &cell, Obstacle obstacles);

  void checkPeople(Cell &cell, People people);


  ExitPoint exit_point;
  Obstacle obstacles;
  Arena arena;
  People people;

  std::vector<std::vector<Cell>> grid;

  int map_pixel_w = 400;
  int map_pixel_h = 600;

  int n_row = 300;
  int n_col = 200;
};

#endif /* Map_hpp */
