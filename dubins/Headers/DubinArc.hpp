#ifndef DubinArc_hpp
#define DubinArc_hpp

#include <opencv2/core/core.hpp>
#include <math.h>
#include <cmath>
#include "Position.hpp"


using namespace cv;

class DubinArc {

public:

    DubinArc(Position start_pos_i,double curvature_i, double length_i);

    ~DubinArc();

    //compute the end point given a initial position lenght and curvature
    void circline(double k, Position start, double length);
    double mod2pi(double ang);
    Position getInitialPosition();
    Position getFinalPosition();
    double getLength();
private:
    double sinc(double inp);


    Position start_pos = Position(cv::Point2d(), 0);
    Position fin_pos = Position(cv::Point2d(), 0);
    double length;
    double curvature;
    double PI = 3.14159265358979323846;

};

#endif /* DubinArc_hpp */
