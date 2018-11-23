//
//  StraightLine.cpp
//  
//
//  Created by Air Marvin on 13.11.18.
//

#include "../Headers/StraightLine.hpp"

StraightLine::StraightLine(Position start_point, Position end_point) {
    setStartPoint(start_point);
    setEndPoint(end_point);
    double tmp_dst = distance(start_point.getCoordinates(),end_point.getCoordinates());
    setLength(tmp_dst);
}

double StraightLine::distance(cv::Point initial_point , cv::Point final_point){
    double res;
    res = sqrt(pow(initial_point.x-final_point.x,2.0)+pow(initial_point.y-final_point.y,2.0));
    return res;
}

StraightLine::StraightLine(Position start_point, double length) {
    setStartPoint(start_point);
    setLength(length);
    Position end_tmp = findEndPoint(0,start_point,length);
    setEndPoint(end_tmp);
}

StraightLine::~StraightLine() {

}
