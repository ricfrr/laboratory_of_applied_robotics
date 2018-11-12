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
/**
 \brief class for undistorsion of the images
 */
class Undistorsion {
    
public:
    /*!
     * construcor of the Undistorion class
     * @param calibration_filename name of the calibration file
     */
    Undistorsion(std::string calibration_filename);
    /*!
     * destructor of the Undistortion class
     */
    ~Undistorsion();

    /*!
     * function that undistort the image 
     * @param frame
     * @param frameUndist
     * @param cameraMatrix
     * @param distCoeffs
     * @param newCameraMatrix
     */
    void undistort_image(cv::Mat frame, cv::Mat frameUndist,
                         InputArray cameraMatrix,
                         InputArray distCoeffs,
                         InputArray newCameraMatrix = noArray() );
    
private:
    std::string calibration_filename;
    void loadCoefficients(const std::string& filename,
                          cv::Mat& camera_matrix,
                          cv::Mat& distCoeffs);
};

#endif /* Undistortion_hpp */
