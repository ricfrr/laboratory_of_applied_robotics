#ifndef Position_hpp
#define Position_hpp

#include <opencv2/core/core.hpp>

using namespace cv;

class Position {

public:
    Position();
    Position(cv::Point2d pos, double orient);

    ~Position();

    void setPosition(cv::Point2d position_p, double orientation_p);

    cv::Point2d getCoordinates();

    double getOrientation();

private:


    cv::Point2d coordinates;
    double orientation;
};

#endif /* Position_hpp */
