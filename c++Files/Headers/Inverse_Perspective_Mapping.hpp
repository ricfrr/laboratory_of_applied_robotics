//
//  Inverse_Perspective_Mapping.hpp
//  LAR_1_0
//
//  Created by Air Marvin on 05.10.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#ifndef Inverse_Perspective_Mapping_hpp
#define Inverse_Perspective_Mapping_hpp

#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <atomic>
#include <unistd.h>

using namespace cv;

struct UserData {
    cv::Mat result, bg_img;
    int idx = 0;
    std::string name;
    std::atomic<bool> done;
    int n;
};

class Inverse_Perspective_Mapping {

public:
    Inverse_Perspective_Mapping();
    ~Inverse_Perspective_Mapping();
    
    UserData *data = new UserData;
    
    std::string outputfilename;
    
void loadCoefficients(const std::string& filename,
                      cv::Mat& camera_matrix,
                      cv::Mat& dist_coeffs);



// Example of function to determine the perspective transformation of a
// rectangle on the ground plane (with manual intervention from the user, that
// is required to select the 4 corner points of the rectangle, starting from the
// top-left corner and proceeding in clockwise order, and the origin (top-left)
// and scale of the transformed top view image).
// Since the real size of the rectangle is known (width: 1m, height: 1.5m),
// the fucntion returns also the pixel_scale, i.e. the size (in mm) of each
// pixel in the top view image
Mat findTransform(const std::string& calib_image_name,
                  const cv::Mat& camera_matrix,
                  const cv::Mat& dist_coeffs,
                  double& pixel_scale,
                  cv::Mat &persp_img);

// Store all the parameters to a file, for a later use, using the FileStorage
// class methods
void storeAllParameters(const std::string& filename,
                        const cv::Mat& camera_matrix,
                        const cv::Mat& dist_coeffs,
                        double pixel_scale,
                        const Mat& persp_transf);
    
cv::Mat run(std::string intrinsic_conf, std::string image, std::string outputfilename );
};

#endif /* Inverse_Perspective_Mapping_hpp */
