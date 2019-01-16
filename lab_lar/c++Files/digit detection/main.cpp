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
#include "Headers/Digit_Recognition.hpp"


int main(int argc, const char * argv[]) {
    
    
    Digit_Recognition dr = Digit_Recognition(DigitRecognitionAlgo::tesseractOCP);
    
    cv::Mat img = cv::imread("unwarped2.png");
    if(img.empty()) {
        throw std::runtime_error("Failed to open the file ");
    }
    HSVFilterRange filter("good");
    filter.saved_quality = "good";
    dr.set_filter(filter);
    dr.detect_digits_for_map(img);
    
    std::cout << "got...the good one" << std::endl;
    
    img = cv::imread("unwarped.png");
    if(img.empty()) {
        throw std::runtime_error("Failed to open the file ");
    }
    filter = HSVFilterRange ("medium");
    filter.saved_quality = "medium";
    dr.set_filter(filter);
    dr.detect_digits_for_map(img);
    
    std::cout << "got...the medium one" << std::endl;
    
    img = cv::imread("unwarped3.png");
    if(img.empty()) {
        throw std::runtime_error("Failed to open the file ");
    }
    filter = HSVFilterRange ("bad");
    filter.saved_quality = "bad";
    dr.set_filter(filter);
    dr.detect_digits_for_map(img);
    
    std::cout << "got...the worst one" << std::endl;
    
    
    return 0;
}
