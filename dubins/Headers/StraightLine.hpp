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
#include <opencv2/core/core.hpp>

class StraightLine: public Line {
    
public:
    StraightLine(Position start_point, Position end_point);
    StraightLine(Position start_point, double length);

private:
    double distance(cv::Point initial_point , cv::Point final_point);
};

#endif /* StraightLine_hpp */
