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
    
    std::pair<int,int> detect_digit(cv::Mat &image);
    
    int detect_digit(tesseract::TessBaseAPI *&OCR, cv::Mat &image);
    
    void getResult(tesseract::TessBaseAPI *&ocr, cv::Mat &img, int &result);
    
    tesseract::TessBaseAPI * ocr = new tesseract::TessBaseAPI();
    

};
#endif /* Optical_Character_Recognition_hpp */
