#include "../Headers/Triangle.hpp"

using namespace Geometry2D;

Triangle::Triangle() {
  // creator
}
Triangle::~Triangle() {
  // nada
}

Triangle::Triangle(const std::vector<cv::Point> &points): Polygon(points){
    assign_points();
}

void Triangle::assign_points(){
    
    if(this->points.size() != 3)
        std::runtime_error("not a triangle");
    
    setCorners(this->points);
    
    //    this->top_left      = points[0];
    //    this->top_right     = points[1];
    //    this->bottom_right  = points[2];
    //    this->bottom_right  = points[3];
}

void Triangle::setCorners(std::vector<cv::Point> corners) {
    if(corners.empty())
        return;
  Triangle::setFirstCorner(corners[0]);
  Triangle::setSecondCorner(corners[2]);
  Triangle::setThirdCorner(corners[1]);
};

std::vector<cv::Point> Triangle::getCorners() {
  std::vector<cv::Point> corners;
  corners.push_back(Triangle::getFirstCorner());
  corners.push_back(Triangle::getSecondCorner());
  corners.push_back(Triangle::getThirdCorner());
  return corners;
};

cv::Point Triangle::getFirstCorner() { return first; };
void Triangle::setFirstCorner(cv::Point firstC) { first = firstC; };

cv::Point Triangle::getSecondCorner() { return second; };
void Triangle::setSecondCorner(cv::Point secondC) { second = secondC; };

cv::Point Triangle::getThirdCorner() { return third; };
void Triangle::setThirdCorner(cv::Point thirdC) { third = thirdC; };
