//
//  StraightLine.hpp
//  
//
//  Created by Air Marvin on 13.11.18.
//

#ifndef StraightLine_hpp
#define StraightLine_hpp

#include <stdio.h>
#include "Line.hpp"

class StraightLine: public Line {
    
    StraightLine(std::pair<double,double> startPoint, std::pair<double,double> endPoint);
public:
    void recompute();
};

#endif /* StraightLine_hpp */
