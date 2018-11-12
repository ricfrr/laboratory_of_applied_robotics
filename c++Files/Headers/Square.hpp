#ifndef Square_hpp
#define Square_hpp

#include <vector>

#include "Shape.hpp"

using namespace cv;

/**
 \brief Class for handling square in the map
 */
class Square : public Shape {

public:
    /*!
     * constructor of square class
     */
    Square();
    /*!
     * destructor of square class
     */
    ~Square();

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
};

#endif /* Square_hpp */
