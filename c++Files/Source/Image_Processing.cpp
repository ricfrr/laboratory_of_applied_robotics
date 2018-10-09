//
//  Image_Processing.cpp
//  LAR_1_0
//
//  Created by Air Marvin on 05.10.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#include "../Headers/Image_Processing.hpp"

void Image_Processing::loadAllParameters(const std::string& filename,
                       cv::Mat& camera_matrix,
                       cv::Mat& dist_coeffs,
                       double& pixel_scale,
                       cv::Mat& persp_transf)
{
    cv::FileStorage fs( filename, cv::FileStorage::READ );
    if (!fs.isOpened())
    {
        throw std::runtime_error("Could not open file " + filename);
    }
    fs["camera_matrix"] >> camera_matrix;
    fs["dist_coeffs"] >> dist_coeffs;
    fs["pixel_scale"] >> pixel_scale;
    fs["persp_transf"] >> persp_transf;
    fs.release();
}


void Image_Processing::run(std::string calibration, std::string imagefilename)
{
    cv::Mat camera_matrix, dist_coeffs, persp_transf;
    double pixel_scale;
    loadAllParameters(calibration, camera_matrix, dist_coeffs,
                      pixel_scale, persp_transf);
    
    std::string filename = imagefilename;
    cv::Mat img = cv::imread(filename.c_str());
    cv::Mat img_undist, img_warped, concat;
    cv::undistort(img, img_undist, camera_matrix, dist_coeffs);
    cv::warpPerspective(img_undist, img_warped, persp_transf, img_undist.size());
    cv::hconcat(img, img_warped, concat);
    cv::resize(concat, concat, cv::Size(1280,512));
    cv::imshow("Win1", concat);
    cv::waitKey(0);
}
