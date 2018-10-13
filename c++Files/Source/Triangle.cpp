#include "../Headers/Triangle.hpp"

Triangle::Triangle() {
  // creator
}
Triangle::~Triangle() {
  // nada
}

void Triangle::setCorners(std::vector<cv::Point> corners) {
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
