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
#include "Arena.hpp"
#include "Settings.hpp"

using namespace cv;

namespace ImageProcessing
{
/*!
 * \brief produce a perspective transformation around the arena given a photo
 */
class Inverse_Perspective_Mapping {
public:
    /*!
     * constructor of  inverse perspective mapping class
     */
    Inverse_Perspective_Mapping();

    /*!
     * destructor of inverse perspective mapping class
     */
    ~Inverse_Perspective_Mapping();


    /*!
     *
     * @param filename intrinsic calibration file name
     * @param camera_matrix camerat matrix file
     * @param dist_coeffs distortion coefficients
     */
    void loadCoefficients(const std::string &filename,
                          cv::Mat &camera_matrix,
                          cv::Mat &dist_coeffs);

    /*!
     * function to determine the perspective transformation of a
     * rectangle on the ground plane
     * @param calib_image_name image
     * @param camera_matrix camera matrix
     * @param dist_coeffs distortion coefficents
     * @param pixel_scale pixel scaling
     * @param persp_img image after trasformation
     * @return
     */
    Mat findTransform(const std::string &calib_image_name,
                      const cv::Mat &camera_matrix,
                      const cv::Mat &dist_coeffs,
                      double &pixel_scale,
                      cv::Mat &persp_img);
    
    Mat findTransform(const cv::Mat &img,
                      const cv::Mat &camera_matrix,
                      const cv::Mat &dist_coeffs,
                      double &pixel_scale,
                      cv::Mat &persp_img);
    /*!
     *  Store all the parameters to a file, for a later use, using the FileStorage
     *  class methods
     * @param filename
     * @param camera_matrix
     * @param dist_coeffs
     * @param pixel_scale
     * @param persp_transf
     */
    // Store all the parameters to a file, for a later use, using the FileStorage
    // class methods
    void storeAllParameters(const std::string &filename,
                            const cv::Mat &camera_matrix,
                            const cv::Mat &dist_coeffs,
                            double pixel_scale,
                            const Mat &persp_transf);

    /*!
     * perform a perspective transformation
     * @param intrinsic_conf
     * @param image
     * @param outputfilename
     * @return the image after transformation
     */
    cv::Mat run(std::string intrinsic_conf, std::string image, std::string outputfilename);
    
    cv::Mat run(std::string intrinsic_conf, const cv::Mat &image, std::string outputfilename);

private:

    Settings settings;
    std::string outputfilename;
};
}
#endif /* Inverse_Perspective_Mapping_hpp */
