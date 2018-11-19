#ifndef Position_hpp
#define Position_hpp

#include <opencv2/core/core.hpp>

using namespace cv;
/**
 * \brief describes a position of a point using x and y coordinates and orientation
 */
class Position {

public:
    /**
     * \brief constructor of the Position class
     */
    Position();
    /**
     * \brief constructor of the Position class
     * @param pos x and y coordinates
     * @param orient orientation
     */
    Position(cv::Point2d pos, double orient);
    /**
     * \brief destructor of the Position class
     */
    ~Position();
    /**
     * \brief set the values of the position
     * @param position_p coordinates
     * @param orientation_p orientation
     */
    void setPosition(cv::Point2d position_p, double orientation_p);
    /**
     * \brief return the coordinates of the position
     * @return the coordinates of the position
     */
    cv::Point2d getCoordinates();
    /**
     * \brief return the orientation of the position
     * @return the orientation of the position
     */
    double getOrientation();

private:


    cv::Point2d coordinates;
    double orientation;
};

#endif /* Position_hpp */
