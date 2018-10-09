//
//  Undistortion.hpp
//  LAR_1_0
//
//  Created by Air Marvin on 05.10.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#ifndef Undistortion_hpp
#define Undistortion_hpp

#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>

using namespace cv;
using namespace std;

class Undistorsion {
    
public:
    Undistorsion(string calibration_filename);
    ~Undistorsion();
    
    void processVideo();
    void undistort_image(cv::Mat frame, cv::Mat frameUndist,
                         InputArray cameraMatrix,
                         InputArray distCoeffs,
                         InputArray newCameraMatrix = noArray() );
    
private:
    string calibration_filename;
    void loadCoefficients(const std::string& filename,
                          cv::Mat& camera_matrix,
                          cv::Mat& distCoeffs);
};

#endif /* Undistortion_hpp */
