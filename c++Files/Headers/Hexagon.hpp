#ifndef Hexagon_hpp
#define Hexagon_hpp
#include <vector>

#include "Shape.hpp"

using namespace cv;

class Hexagon : public Shape
{

  public:
    Hexagon();
    ~Hexagon();

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

    cv::Point getSixthCorner();
    void setSixthCorner(cv::Point sixthC);

  private:
    void findHalf(int &half_h, int &half_w,
                  const std::vector<cv::Point> &corners);

    cv::Point first, second, third, fourth, fifth, sixth;
};

#endif /* Hexagon_hpp */
