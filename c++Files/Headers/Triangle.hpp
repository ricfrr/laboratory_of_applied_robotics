#ifndef Triangle_hpp
#define Triangle_hpp
#include <vector>

#include "Shape.hpp"

using namespace cv;

class Triangle : public Shape
{

  public:
    Triangle();
    ~Triangle();

    // setter and getter of the points
    std::vector<cv::Point> getCorners();
    void setCorners(std::vector<cv::Point> corners);

    cv::Point getFirstCorner();
    void setFirstCorner(cv::Point firstC);

    cv::Point getSecondCorner();
    void setSecondCorner(cv::Point secondC);

    cv::Point getThirdCorner();
    void setThirdCorner(cv::Point thirdC);

  private:
    void findHalf(int &half_h, int &half_w,
                  const std::vector<cv::Point> &corners);

    cv::Point first, second, third;
};

#endif /* Triangle_hpp */
