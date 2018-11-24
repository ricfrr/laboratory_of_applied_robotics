//
//  Polygon.cpp
//  LAR_1_0
//
//  Created by Air Marvin on 15.11.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#include "../Headers/Polygon.hpp"

Polygon::Polygon(){}
Polygon::Polygon(std::vector<cv::Point> points)
{
    this->points = points;
}
Polygon::~Polygon(){}
