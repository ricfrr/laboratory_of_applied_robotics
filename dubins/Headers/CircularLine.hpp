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
#include <cmath>

class CircularLine : public Line {

public:

    CircularLine(Position start_point, double curvature, double length);

    ~CircularLine();

    void setCurvature(double curvature_i);

    double getCurvature();


private:

    double curvature;

};
#endif /* CircularLine_hpp */
