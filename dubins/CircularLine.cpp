//
//  CircularLine.cpp
//  
//
//  Created by Air Marvin on 13.11.18.
//

#include "CircularLine.hpp"

CircularLine::CircularLine(std::pair<double,double> startPoint, double angle, double curvature){
    
    this->startPoint = startPoint;
    this->angle = angle;
    this->curvature = curvature;
    
    recompute();
    
}

CircularLine::CircularLine(double curvature){
    
    this->curvature = curvature;
}

CircularLine::CircularLine::recompute(){}
