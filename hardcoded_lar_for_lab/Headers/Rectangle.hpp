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
    /**
     * \brief function that returns the top left corner
     * @return the top left corner
     */
    cv::Point getTopLeft();
    /**
     * \brief function that sets the top left corner
     * @param topLeft the top left corner
     */
    void setTopLeft(cv::Point topLeft);
    /**
     * \brief function that return the top right corner
     * @return the top right corner
     */
    cv::Point getTopRight();
    /**
     * \brief function that set the top right corner
     * @param topRight the top right corner
     */
    void setTopRight(cv::Point topRight);
    /**
     * function that return the bottom left corner
     * @return the bottom left corner
     */
    cv::Point getBottomLeft();
    /**
     * \brief function that set the bottom left corner
     * @param bottomLeft the bottom left corner
     */
    void setBottomLeft(cv::Point bottomLeft);
    /**
     * function that return the bottom right corner
     * @return the bottom right corner
     */
    cv::Point getBottomRight();
    /**
     * function that set the bottom right corner
     * @param bottomRight set the bottom right corner
     */
    void setBottomRight(cv::Point bottomRight);
    /**
     * function that find the half w and half h given the corners
     * @param half_h half of the height
     * @param half_w half of the width
     * @param corners corner of the rectangle
     */
    void findHalf(int &half_h, int &half_w,
                  const std::vector<cv::Point> &corners);
    // corners of the rectangle
    cv::Point top_left, top_right, bottom_left, bottom_right;



    ///assignes points to 1., 2., 3 etc point.
    void assign_points();
};
    
}

#endif /* Rectangle_hpp */
