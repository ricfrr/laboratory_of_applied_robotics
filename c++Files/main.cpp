//
//  main.cpp
//  LAR_1_0
//
//  Created by Air Marvin on 04.10.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#include <iostream>
#include "Headers/Calibration_Intrinsic.hpp"
#include "Headers/Undistortion.hpp"
#include "Headers/Inverse_Perspective_Mapping.hpp"
#include "Headers/Image_Processing.hpp"
#include "Headers/Arena.hpp"
#include "Headers/ExitPoint.hpp"
#include "Headers/Pentagon.hpp"
#include "Headers/Obstacle.hpp"

using namespace std;

const string intrinsic_calibration = "config/intrinsic_calibration.xml";

int main(int argc, const char *argv[])
{

    //Calibration_Instrinsic::performCalibration(argv[1]);
    //Undistorsion undistorsion = Undistorsion(intrinsic_path);

    cv::Mat persp_img;
    Inverse_Perspective_Mapping ipm = Inverse_Perspective_Mapping();
    persp_img = ipm.run(intrinsic_calibration, argv[1], "config/fullCalibration.yml");

    //shape detection
    Arena arena = Arena();
    arena.findArena(persp_img);

    ExitPoint exit_point = ExitPoint();
    exit_point.findExitPoint(persp_img);

    Obstacle obstacle_finder = Obstacle();
    obstacle_finder.findObstacles(persp_img);
    std::vector<Pentagon> pentagons = obstacle_finder.getPentagons();
    
    
    
    return 0;
}
