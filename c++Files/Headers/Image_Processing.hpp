//
//  Image_Processing.hpp
//  LAR_1_0
//
//  Created by Air Marvin on 05.10.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#ifndef Image_Processing_hpp
#define Image_Processing_hpp

#include <stdio.h>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <atomic>
#include <unistd.h>

class Image_Processing {
    
public:
    static void loadAllParameters(const std::string& filename,
                       cv::Mat& camera_matrix,
                       cv::Mat& dist_coeffs,
                       double& pixel_scale,
                       cv::Mat& persp_transf);


    static void run(std::string calibrationfile, std::string imagefilename);
    
};
#endif /* Image_Processing_hpp */
