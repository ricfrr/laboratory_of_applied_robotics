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


int main(int argc, const char * argv[]) {
    
    Settings s = Calibration_Instrinsic::performCalibration(argv[1]);
    Undistorsion undistorsion = Undistorsion(s.outputFileName);
    undistorsion.processVideo();
    
    Inverse_Perspective_Mapping * ipm = new Inverse_Perspective_Mapping;
    ipm->run(s.outputFileName,"../data/persp_transf.jpg","../config/fullCalibration.yml");
    
    Image_Processing::run(ipm->outputfilename,"../data/img.jpg");
    
    return 0;
}
