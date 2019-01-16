#ifndef Rectangle_hpp
#define Rectangle_hpp

#include <vector>

#include "Polygon.hpp"

using namespace cv;

namespace Geometry2D
{
/**
 \brief Class for handling square in the map
 */
class Rectangle : public Polygon {

public:
    /*!
     * constructor of square class
     */
    Rectangle();
    
    Rectangle(std::vector<cv::Point> points);
    
    /*!
     * destructor of square class
     */
    ~Rectangle();

    /*!
     * return the list of corners
     * @return the list of corners
     */
    std::vector<cv::Point> getCorners();
    /*!
     *
     * @param corners return the list of corners
     */
    void setCorners(std::vector<cv::Point> corners);

    cv::Point getTopLeft();

    void setTopLeft(cv::Point topLeft);

    cv::Point getTopRight();

    void setTopRight(cv::Point topRight);

    cv::Point getBottomLeft();

    void setBottomLeft(cv::Point bottomLeft);

    cv::Point getBottomRight();

    void setBottomRight(cv::Point bottomRight);
    void findHalf(int &half_h, int &half_w,
                  const std::vector<cv::Point> &corners);

    cv::Point top_left, top_right, bottom_left, bottom_right;



    ///assignes points to 1., 2., 3 etc point.
    void assign_points();
};
    
}

#endif /* Rectangle_hpp */
