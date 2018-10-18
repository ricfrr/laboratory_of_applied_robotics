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

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <string>

#include "Character_Recognition_Algorithm.hpp"

///OCP algorithm from tesseract library
class Optical_Character_Recognition: public Character_Recognition_Algorithm
{
    
public:
    Optical_Character_Recognition();
    ~Optical_Character_Recognition();
    
    ///demo function for displaying performance of algorithm
    void processImage(const std::string& filename);
    
    int detect_digit(cv::Mat &image, cv::Rect &rect, cv::Mat &ROI);
    
    int detect_digit(tesseract::TessBaseAPI *&OCR, cv::Mat &image, cv::Rect &rect, cv::Mat &ROI);
    
    void getResult(tesseract::TessBaseAPI *&ocr, cv::Mat &img, int &result);
    
    std::vector<std::pair<int,cv::Rect>> detection_algorithm(std::vector<cv::Rect> &boundRect, cv::Mat &filtered);
    
    int maxConf = 85;
    
    tesseract::TessBaseAPI * ocr = new tesseract::TessBaseAPI();
    

};
#endif /* Optical_Character_Recognition_hpp */
