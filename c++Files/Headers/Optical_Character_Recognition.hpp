//
//  Optical_Character_Recognition.hpp
//  LAR_1_0
//
//  Created by Air Marvin on 10.10.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#ifndef Optical_Character_Recognition_hpp
#define Optical_Character_Recognition_hpp

#include <stdio.h>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include "Character_Recognition_Algorithm.hpp"

///OCP algorithm from tesseract library
class Optical_Character_Recognition: public Character_Recognition_Algorithm
{
    
public:
    Optical_Character_Recognition();
    ~Optical_Character_Recognition();
    
    const double MIN_AREA_SIZE = 100;
    
    ///demo function for displaying performance of algorithm
    void processImage(const std::string& filename);
    
    void displayImage(cv::Mat & image,std::string windowTitle);
    
    int detect_digit(cv::Mat &image, cv::Rect &rect, cv::Mat &ROI);
    
private:
    cv::Mat loadImage(const std::string& filename);
    void convert_bgr_to_hsv(cv::Mat &original, cv::Mat &converted);
    void apply_mask(cv::Mat &original, cv::Mat &converted, cv::Scalar lowerbound, cv::Scalar upperbound);
    cv::Mat apply_some_filtering(cv::Mat &img);
    std::vector<cv::Rect> extract_regions_of_interest(cv::Mat &original_img,
                                                      cv::Mat & filtered_img,
                                                      cv::Mat &returnedImg);
    
    std::tuple<cv::Mat,cv::Mat> invert_masked_image(cv::Mat &original, cv::Mat &masked_image);
    //void invertPixels()
};
#endif /* Optical_Character_Recognition_hpp */
