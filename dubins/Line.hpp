//
//  Line.hpp
//  
//
//  Created by Air Marvin on 13.11.18.
//

#ifndef Line_hpp
#define Line_hpp

#include <stdio.h>

class Line {
public:
    Line();
    Line(std::pair<double,double> startPoint, double length = 1);
    ~Line();
    
    std::pair<double,double> startPoint, endPoint;
    double length;
    
    //when the startpoint or the length or other values have changed it is necessary to recompute the endPoint right?
    void recompute() = 1;
};

#endif /* Line_hpp */
