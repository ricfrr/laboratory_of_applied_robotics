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

const string intrinsic_calibration = "config/intrinsic_calibration.xml";

int main(int argc, const char * argv[]) {

    //Calibration_Instrinsic cal = Calibration_Instrinsic();
    //cal.performCalibration(argv[1]);
    Calibration_Instrinsic::performCalibration("calib_config.xml");

    
    //Undistorsion undistorsion = Undistorsion(intrinsic_path);
    Inverse_Perspective_Mapping ipm = Inverse_Perspective_Mapping();
    ipm.run(intrinsic_calibration,"../data/map/01.jpg","config/fullCalibration.yml");

    
    return 0;
}
