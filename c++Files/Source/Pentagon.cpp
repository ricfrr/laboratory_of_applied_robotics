#include "../Headers/Pentagon.hpp"

Pentagon::Pentagon() {
  // creator
}

Pentagon::Pentagon(std::vector<cv::Point> points): Polygon(points){
    assign_points();
}

Pentagon::~Pentagon() {
  // nada
}

void Pentagon::assign_points(){
    
    if(this->points.size() != 5)
        std::runtime_error("not a pentagon");
    
    setCorners(this->points);
    
    //    this->top_left      = points[0];
    //    this->top_right     = points[1];
    //    this->bottom_right  = points[2];
    //    this->bottom_right  = points[3];
}

void Pentagon::setCorners(std::vector<cv::Point> corners) {
  Pentagon::setFirstCorner(corners[0]);
  Pentagon::setSecondCorner(corners[1]);
  Pentagon::setThirdCorner(corners[2]);
  Pentagon::setFourthCorner(corners[3]);
  Pentagon::setFifthCorner(corners[4]);
};

std::vector<cv::Point> Pentagon::getCorners() {
  std::vector<cv::Point> corners;
  corners.push_back(Pentagon::getFirstCorner());
  corners.push_back(Pentagon::getSecondCorner());
  corners.push_back(Pentagon::getThirdCorner());
  corners.push_back(Pentagon::getFourthCorner());
  corners.push_back(Pentagon::getFifthCorner());
  return corners;
};

cv::Point Pentagon::getFirstCorner() { return first; };
void Pentagon::setFirstCorner(cv::Point firstC) { first = firstC; };

cv::Point Pentagon::getSecondCorner() { return second; };
void Pentagon::setSecondCorner(cv::Point secondC) { second = secondC; };

cv::Point Pentagon::getThirdCorner() { return third; };
void Pentagon::setThirdCorner(cv::Point thirdC) { third = thirdC; };

cv::Point Pentagon::getFourthCorner() { return fourth; };
void Pentagon::setFourthCorner(cv::Point fourthC) { fourth = fourthC; };

cv::Point Pentagon::getFifthCorner() { return fifth; };
void Pentagon::setFifthCorner(cv::Point fifthC) { fifth = fifthC; };

