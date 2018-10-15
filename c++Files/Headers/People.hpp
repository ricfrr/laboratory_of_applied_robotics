#ifndef People_hpp
#define People_hpp
#include "Circle.hpp"
#include "Shape.hpp"
#include <vector>

using namespace cv;

class People : public Shape
{

public:
  People();
  ~People();

  void findCircles(const Mat &img); // return a list of Peoples
  
  std::vector<Circle> getCircles();

private:
  const int epsilon_approx = 7;
  std::vector<Circle> circles;

};

#endif /* People_hpp */
