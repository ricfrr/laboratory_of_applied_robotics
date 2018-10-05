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

int main(int argc, const char * argv[]) {
    
    Settings s = Calibration_Instrinsic::performCalibration(argv[1]);
    Undistorsion undistorsion = Undistorsion(s.outputFileName);
    undistorsion.processVideo();
    
    return 0;
}
