#ifndef Pentagon_hpp
#define Pentagon_hpp
#include <vector>

#include "Shape.hpp"

using namespace cv;

class Pentagon : public Shape {

public:
  Pentagon();
  ~Pentagon();

  // setter and getter of the points
  std::vector<cv::Point> getCorners();
  void setCorners(std::vector<cv::Point> corners);

  cv::Point getFirstCorner();
  void setFirstCorner(cv::Point firstC);

  cv::Point getSecondCorner();
  void setSecondCorner(cv::Point secondC);

  cv::Point getThirdCorner();
  void setThirdCorner(cv::Point thirdC);

  cv::Point getFourthCorner();
  void setFourthCorner(cv::Point fourthC);

  cv::Point getFifthCorner();
  void setFifthCorner(cv::Point fifthC);

private:
  void findHalf(int &half_h, int &half_w,
                const std::vector<cv::Point> &corners);

  cv::Point first, second, third, fourth, fifth;
};

#endif /* Pentagon_hpp */
