#ifndef Triangle_hpp
#define Triangle_hpp

#include <vector>

#include "Polygon.hpp"

using namespace cv;
namespace Geometry2D {

/**
 \brief class for handling triangle obstacles
 */
class Triangle : public Polygon {

public:
    /*!
     * constructor of triangle class
     */
    Triangle();

    Triangle(std::vector<cv::Point> points);
    
    /*!
     * destructor of triangle class
     */
    ~Triangle();

    /*!
     * return the list of corners
     * @return list of corners of the triangle
     */
    std::vector<cv::Point> getCorners();

    /*!
     * set corners of the triangle
     * @param corners list of corners
     */
    void setCorners(std::vector<cv::Point> corners);
    
    ///assignes points to 1., 2., 3 etc point.
    void assign_points();


private:

    cv::Point getFirstCorner();

    void setFirstCorner(cv::Point firstC);

    cv::Point getSecondCorner();

    void setSecondCorner(cv::Point secondC);

    cv::Point getThirdCorner();

    void setThirdCorner(cv::Point thirdC);

    void findHalf(int &half_h, int &half_w,
                  const std::vector<cv::Point> &corners);

    cv::Point first, second, third;
};

}
#endif /* Triangle_hpp */
