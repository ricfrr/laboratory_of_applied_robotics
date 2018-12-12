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

namespace Path2D {
/**
 * \brief describe a StraightLine in the path
 */
class StraightLine: public Line {
    
public:
    /**
     * \brief constructor of the StraightLine class
     * @param start_point start point of the line
     * @param end_point end point of the line
     */
    StraightLine(Position start_point, Position end_point);
    /**
     * \brief constructor of the StraightLine class
     * @param start_point start point of the line
     * @param length length of the line
     */
    StraightLine(Position start_point, double length);
    /**
     * \brief destructor of the StraightLine class
     */
    ~StraightLine();

private:
    double distance(cv::Point initial_point , cv::Point final_point);
};
    
}

#endif /* StraightLine_hpp */
