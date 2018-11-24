//
//  main.cpp
//  LAR_1_0
//
//  Created by Air Marvin on 04.10.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//


//map classes
#include <iostream>
#include "Headers/Calibration_Intrinsic.hpp"
#include "Headers/Undistortion.hpp"
#include "Headers/Inverse_Perspective_Mapping.hpp"
#include "Headers/Image_Processing.hpp"
#include "Headers/Arena.hpp"
#include "Headers/ExitPoint.hpp"
#include "Headers/Map.hpp"
#include "Headers/Obstacle.hpp"


// path planning classes
#include "Headers/Position.hpp"
#include "Headers/PathCoordinates.hpp"
#include "Headers/Path.hpp"
#include "Headers/CircularLine.hpp"

using namespace std;

const string intrinsic_calibration = "../config/intrinsic_calibration.xml";

PathCoordinates initialize(){
    // initial position
    int in_x = 100;
    int in_y = 400;
    double in_orient = -M_PI/2.0; //-1.0/3*M_PI;
    double Kmax = 0.2; //max curvature 1/kmax give us the radius
    cv::Point2d in_coordinates = Point2d(in_x,in_y);
    Position in_position = Position(in_coordinates,in_orient);
    // final position
    int f_x = 100;
    int f_y = 450;
    double f_orient = M_PI/2.0;
    cv::Point2d f_coordinates = Point2d(f_x,f_y);
    Position f_position = Position(f_coordinates,f_orient);


    PathCoordinates car = PathCoordinates(in_position,f_position,Kmax);
    return car;
}



int main(int argc, const char *argv[])
{

    //Calibration_Instrinsic::performCalibration(argv[1]);
    //Undistorsion undistorsion = Undistorsion(intrinsic_path);

    cv::Mat persp_img;
    Inverse_Perspective_Mapping ipm = Inverse_Perspective_Mapping();
    persp_img = ipm.run(intrinsic_calibration, argv[1], "../config/fullCalibration.yml");

//    cv::Mat persp_img = cv::imread(argv[1]);
    // MAP
    Map map = Map();
    map.createMap(persp_img);


    // Path finding
    PathCoordinates pathCoordinates = initialize();
    Path path = Path(pathCoordinates.getInitialPosition(),pathCoordinates.getFinalPosition(),pathCoordinates.getMaxCurvature(),&map);
    std::vector<Line> lines = path.getLines();

    return 0;
}
