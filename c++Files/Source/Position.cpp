//
// Created by riccardo on 11/11/2018.
//


#include "../Headers/Position.hpp"

using namespace cv;

Position::Position(cv::Point2d pos, double orient) {
    setPosition(pos, orient);
};

Position::Position() {
    //empty constructor
}

Position::~Position() {

};

void Position::setPosition(cv::Point2d position_p, double orientation_p) {
    coordinates = position_p;
    orientation = orientation_p;
};

cv::Point2d Position::getCoordinates() {
    return coordinates;
};

double Position::getOrientation() {
    return orientation;
};
