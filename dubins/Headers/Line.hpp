//
//  Line.hpp
//  
//
//  Created by Air Marvin on 13.11.18.
//

#ifndef Line_hpp
#define Line_hpp

#include <stdio.h>
#include <opencv2/core/core.hpp>
#include "Position.hpp"


class Line {
public:
    Line();

    Line(Position start_point, Position end_point);

    ~Line();

    void setStartPoint(Position start_point_i);

    Position getStartPoint();

    void setEndPoint(Position end_point_i);

    Position getEndPoint();

    void setLength(double length_i);

    double getLength();

    void setOrientation(double orientation_i);

    double getOrientation();

    Position findEndPoint(double k, Position start, double length);

    double sinc(double inp);

    double mod2pi(double ang);

private:

    Position start_point;
    Position end_point;
    double length;

};

#endif /* Line_hpp */
