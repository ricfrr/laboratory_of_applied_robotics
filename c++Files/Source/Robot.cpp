//
//  Robot.cpp
//  LAR_1_0
//
//  Created by Air Marvin on 23.11.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#include "../Headers/Robot.hpp"

Robot::Robot(){
    this->radius = 10;
    this->center = cv::Point(20,20);
    this->data = nullptr;
}
Robot::~Robot(){}

Path2D::Element::PathCoordinates Robot::initialize(){
    // initial position
    int in_x = 0;
    int in_y = 1;
    double in_orient = -1.0/3*M_PI;
    double Kmax = 1.0; //max curvature 1/kmax give us the radius
    cv::Point2d in_coordinates = Point2d(in_x,in_y);
    Path2D::Element::Position in_position = Path2D::Element::Position(in_coordinates,in_orient);
    // final position
    int f_x = 4;
    int f_y = 3;
    double f_orient = M_PI/3.0;
    cv::Point2d f_coordinates = Point2d(f_x,f_y);
    Path2D::Element::Position f_position = Path2D::Element::Position(f_coordinates,f_orient);
    
    
    Path2D::Element::PathCoordinates car = Path2D::Element::PathCoordinates(in_position,f_position,Kmax);
    return car;
}
