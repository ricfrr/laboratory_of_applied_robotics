#ifndef Clipper_hpp
#define Clipper_hpp

#include <stdio.h>
#include <opencv2/core.hpp>
#include <cmath>

namespace ImageProcessing {
class Clipper{

public:
    Clipper();
    ~Clipper();

    /**
     * given a list of point and an offster perform clipping and return a vector of points
     * @param points list of input points
     * @param offset offset values
     * @return vector of points after clipping
     */
    std::vector<cv::Point> clip(std::vector<cv::Point> &points, int offset);
    /**
     * brief clip the arena corners
     * @param points corners of the arena
     * @param offset amount of clipping
     * @return a vector of clipped points
     */
    std::vector<cv::Point> clipArena(std::vector<cv::Point> &points, int offset);

};
}

#endif /* Clipper_hpp */
