//
//  Robot.hpp
//  LAR_1_0
//
//  Created by Air Marvin on 23.11.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#ifndef Robot_hpp
#define Robot_hpp

#include <stdio.h>
#include "Triangle.hpp"
#include "Geometry.hpp"
#include "Inverse_Perspective_Mapping.hpp"

using namespace Geometry2D;

class Robot: public Triangle {
    
public:
    
    Robot(const std::vector<cv::Point> &points = {});
    ~Robot();
    
    const cv::Scalar color = cv::Scalar(0,0,255);
    
    void update(const std::vector<cv::Point> &points = {});
    
    bool findRobot(const cv::Mat &img);
    
    void move(const cv::Point &location, const double &angle);
    
    double radius;
    cv::Point center;
    cv::Point center_wheel;
    double angle;
    
    const int epsilon_approx = 7;

private:


};

#endif /* Robot_hpp */

