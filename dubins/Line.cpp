//
//  Line.cpp
//  
//
//  Created by Air Marvin on 13.11.18.
//

#include "Line.hpp"

Line::Line(){}

Line::Line(std::pair<double,double> startPoint, double length){
    this->startPoint = startPoint;
    this->length = length;
}

Line::~Line(){}
