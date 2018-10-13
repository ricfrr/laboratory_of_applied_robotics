#ifndef Square_hpp
#define Square_hpp
#include <vector>

#include "Shape.hpp"

using namespace cv;

class Square : public Shape  {

public:
  Square();
  ~Square();

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
  void findHalf(int &half_h, int &half_w,
                const std::vector<cv::Point> &corners);

  cv::Point top_left, top_right, bottom_left, bottom_right;
};

#endif /* Square_hpp */
