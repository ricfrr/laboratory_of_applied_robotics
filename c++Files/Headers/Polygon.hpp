//
//  Polygon.hpp
//  LAR_1_0
//
//  Created by Air Marvin on 15.11.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#ifndef Polygon_hpp
#define Polygon_hpp

#include <stdio.h>
#include "Shape.hpp"

class Polygon: public Shape {

public:
    Polygon();
    Polygon(std::vector<cv::Point> points);
    ~Polygon();
    
    ///brief the points of the polygon
    std::vector<cv::Point> points;
    
    ///assignes the points
    virtual void assign_points() = 0;
    
};

#endif /* Polygon_hpp */
