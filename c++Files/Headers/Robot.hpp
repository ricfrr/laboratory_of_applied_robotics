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
#include "Circle.hpp"
#include "PathCoordinates.hpp"

class Robot: public Circle {
    
public:
    
    Robot();
    ~Robot();
    
    const cv::Scalar color = cv::Scalar(0,0,255);
    
    Path2D::PathCoordinates* data;
    
    Path2D::PathCoordinates initialize();
    
};

#endif /* Robot_hpp */

