//
//  Polygon.cpp
//  LAR_1_0
//
//  Created by Air Marvin on 15.11.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#include "../Headers/Polygon.hpp"

using namespace Geometry2D;

Polygon::Polygon(){}
Polygon::Polygon(const std::vector<cv::Point> &points)
{
    this->points = points;
}
Polygon::~Polygon(){}



void Polygon::setClippedCorners(std::vector<cv::Point> &_clipped_corners) {
    clipped_corners = _clipped_corners;
}
std::vector<cv::Point> Polygon::getClippedCorners(){
    return clipped_corners;
};
