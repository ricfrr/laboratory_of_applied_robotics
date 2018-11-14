//
//  Template_Character_Recognition.hpp
//  LAR_1_0
//
//  Created by Air Marvin on 11.10.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#ifndef Template_Character_Recognition_hpp
#define Template_Character_Recognition_hpp

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

#include <opencv2/opencv.hpp>

#include "Character_Recognition_Algorithm.hpp"

/**
 \brief Class that performs the Template Matching Method to identify characters
 */
class Template_Character_Recognition: public Character_Recognition_Algorithm {
    
public:
    Template_Character_Recognition();
    ~Template_Character_Recognition();
    
    ///path to templates
    const std::string template_path = "../data/template/";
    
    ///detect a digit in a prepared image
    std::pair<int,int> detect_digit(cv::Mat &image);
    
private:
    std::pair<int,int> getResult(std::vector<cv::Mat> &templROIs, cv::Mat &image);
    
    int thres_score = 300000000;
};
#endif /* Template_Character_Recognition_hpp */
