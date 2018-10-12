#ifndef ExitPoint_hpp
#define ExitPoint_hpp
#include "Shape.hpp"
#include <vector>

using namespace cv;

class ExitPoint : public Shape
{

public:
  ExitPoint();
  ~ExitPoint();

  //

  void findExitPoint(Mat img);

  // setter and getter of the points
  std::vector<cv::Point> getCorners();
  void setCorners(std::vector<cv::Point> corners);

  cv::Point getTopLeft();
  void setTopLeft(cv::Point topLeft);

  cv::Point getTopRight();
  void setTopRight(cv::Point topRight);

  cv::Point getBottomLeft();
  void setBottomLeft(cv::Point bottomLeft);

  cv::Point getBottomRight();
  void setBottomRight(cv::Point bottomRight);

private:
  const int epsilon_approx = 20; // value for detect poly
  cv::Point top_left, top_right, bottom_left, bottom_right;
};

#endif /* ExitPoint_hpp */
