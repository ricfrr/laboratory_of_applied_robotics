//
//  CircularLine.hpp
//  
//
//  Created by Air Marvin on 13.11.18.
//

#ifndef CircularLine_hpp
#define CircularLine_hpp

#include <stdio.h>
#include "Line.hpp"

class CircularLine: public Line {
    
public:
    CircularLine(std::pair<double,double> startPoint, double angle, double curvature);
    CircularLine(double curvature);
    
    double angle;
    
    void recompute();
    
private:
    double curvature;
};

#endif /* CircularLine_hpp */
