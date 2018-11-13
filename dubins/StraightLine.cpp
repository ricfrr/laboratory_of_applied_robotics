//
//  StraightLine.cpp
//  
//
//  Created by Air Marvin on 13.11.18.
//

#include "StraightLine.hpp"

StraightLine::StraightLine(std::pair<double,double> startPoint, std::pair<double,double> endPoint){
    
    this->startPoint = startPoint;
    this->endPoint = endPoint;
    
    recompute();
}

void StraightLine::recompute(){
    
    //given startpoint and endpoint the length should be set
    double x_0 = startPoint.first;
    double y_0 = startPoint.second;
    double x_1 = endPoint.first;
    double y_1 = endPoint.second;
    
    this->length = std::sqrt(pow(x_1 - x_0,2)+pow(y_1-y_0,2));
    
    //given startpoint and length the endpoint needs to be checked
    
}
