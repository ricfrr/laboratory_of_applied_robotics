#ifndef Position_hpp
#define Position_hpp

#include <opencv2/core/core.hpp>

using namespace cv;

namespace PathE2D {
    
    namespace Element {
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
     * \brief construction of the Position class
     * @param pos coordinate of the position
     */
    Position(cv::Point2d pos);

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

    /**
     * set the orientation of the path
     * @param _orientation the orientation of the path
     */
    void setOrientation(double _orientation);
    /**
     * set the weight of the position;
     * @param _weight the weight of the position
     */
    void setWeight (double _weight);

    /**
     * return the weight of the position
     * @return the weight of the position
     */
    double getWeight();

    bool orientation_locked = false;
    
    double orientation;

private:
    double weight= 0;

    cv::Point2d coordinates;
    
};
    
}
    
}

#endif /* Position_hpp */
