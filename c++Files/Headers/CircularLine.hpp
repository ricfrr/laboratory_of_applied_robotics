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

/**
 \brief Contains Datatypes for planning and constructing 2D paths.
 */
namespace Path2D {
    /**
     \brief Contains basic elements needed for describing a path
     */
    namespace Element{
/**
 \brief Class for managing circle in the pat
 */
class CircularLine : public Line {

public:
    /**
     \brief constructor of the class
     * @param start_point  start position of the line
     * @param curvature curvature of the path
     * @param length length of the path
     */
    CircularLine(Position start_point, double curvature, double length);
    /**
     \brief destructor of the class
     */
    ~CircularLine();

};
    
}
}
#endif /* CircularLine_hpp */
