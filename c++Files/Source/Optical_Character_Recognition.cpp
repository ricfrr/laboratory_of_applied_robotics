//
//  Optical_Character_Recognition.cpp
//  LAR_1_0
//
//  Created by Air Marvin on 10.10.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#include "../Headers/Optical_Character_Recognition.hpp"

Optical_Character_Recognition::Optical_Character_Recognition(){
    // Initialize tesseract to use English (eng)
    ocr->Init(NULL, "eng");
    // Set Page segmentation mode to PSM_SINGLE_CHAR (10)
    ocr->SetPageSegMode(tesseract::PSM_SINGLE_CHAR);
    // Only digits are valid output characters
    ocr->SetVariable("tessedit_char_whitelist", "0123456789");
    
    this->filter = HSVFilterRange("bad");
}

Optical_Character_Recognition::~Optical_Character_Recognition(){
    ocr->End();
}

int Optical_Character_Recognition::detect_digit(tesseract::TessBaseAPI *&OCR, cv::Mat &image){
    
    // Set image data
    OCR->SetImage(image.data, image.cols, image.rows, 3, image.step);
    
    //std::cout << "result with confidence " << OCR->MeanTextConf() << std::endl;
    
    // know your ASCI table
    int result = *OCR->GetUTF8Text() - '0';
    
    if(result == -16) return -99;
    
    // Run Tesseract OCR on image and print recognized digit
    //std::cout << "\nRecognized digit: " << OCR->GetUTF8Text() << "with confidence of " << std::to_string(OCR->MeanTextConf()) << std::endl;
    
    return result;
}

std::pair<int,int> Optical_Character_Recognition::detect_digit(cv::Mat &image){
    
    // Create Tesseract object
    tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();
    // Initialize tesseract to use English (eng)
    ocr->Init(NULL, "eng");
    // Set Page segmentation mode to PSM_SINGLE_CHAR (10)
    ocr->SetPageSegMode(tesseract::PSM_SINGLE_CHAR);
    // Only digits are valid output characters
    ocr->SetVariable("tessedit_char_whitelist", "0123456789");
    
    // know your ASCI table
    int result = detect_digit(ocr, image);
    int confidence = ocr->MeanTextConf();
    
    ocr->End();
    
    return std::pair<int,int>(result,confidence);
}

void Optical_Character_Recognition::getResult(tesseract::TessBaseAPI *&ocr, cv::Mat &img, int &result){
    // Set image data
    ocr->SetImage(img.data, img.cols, img.rows, 3, img.step);
    
    // know your ASCI table
    result = *ocr->GetUTF8Text() - '0';
}
