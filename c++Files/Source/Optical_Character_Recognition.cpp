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
    
    std::cout << "result with confidence " << OCR->MeanTextConf() << std::endl;
    
    // know your ASCI table
    int result = *OCR->GetUTF8Text() - '0';
    
    if(result == -16) return -99;
    
    // Run Tesseract OCR on image and print recognized digit
    //std::cout << "\nRecognized digit: " << OCR->GetUTF8Text() << "with confidence of " << std::to_string(OCR->MeanTextConf()) << std::endl;
    
    return result;
}

int Optical_Character_Recognition::detect_digit(cv::Mat &image){
    
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
    
    ocr->End();
    
    return result;
}

void Optical_Character_Recognition::getResult(tesseract::TessBaseAPI *&ocr, cv::Mat &img, int &result){
    // Set image data
    ocr->SetImage(img.data, img.cols, img.rows, 3, img.step);
    
    // know your ASCI table
    result = *ocr->GetUTF8Text() - '0';
}

std::vector<std::pair<int,cv::Rect>> Optical_Character_Recognition::detection_algorithm(std::vector<cv::Rect> &boundRect, cv::Mat &filtered){
    
    std::vector<std::pair<int,cv::Rect>> results;
    
    //go through all rects and see if it contains a digit
    for (int i=0; i<boundRect.size(); ++i)
    {
        // roi is the image passed to the tesseract engine
        cv::Mat roi;
        
        cv::Mat final_roi;
        
        roi = cv::Mat(filtered,boundRect[i]);
        
        // run the tesseract engine
        int result = detect_digit(ocr,filtered);
        
        double angle = 0;
        bool entered = false;
        
        int conf = ocr->MeanTextConf();
        
        if (conf < maxConf) result = -99;
        
        if (result != -99) {
            final_roi = roi;
            angle = 360;}
        
        DigitResultDistribution dis = DigitResultDistribution();
        
        int level = conf;
        int r = result;
        
        while(angle < 360 && !roi.empty()){
            entered = true;
//            std::cout << "rotating image and retrying" << std::endl;
//            if(angle == 0){
//                cv::imshow("Problem", roi);
//                cv::waitKey(0);
//            }
            
            cv::Mat roi2;
            rotate_image(roi, angle, roi2);
            
            //get the result
            getResult(ocr, roi2, result);
            
            if(result == -16) result = -99;
            
            if(ocr->MeanTextConf() < maxConf){
                result = -99;
            }
            else if (ocr->MeanTextConf() > level) {
                level = ocr->MeanTextConf();
                r = result;
                final_roi = roi2;
            }
            
            if (result != -99) {
                dis.add(result);
            }
            
            angle += delta_angle;
        }
        
        if (entered) {
            result = dis.best();
        }
        
        //no digit is result -99
        if (result != -99) {
//            std::cout << "result was " << std::to_string(result)  << std::endl;
//            std::cout << "with a confidence level of " << std::to_string(level) << std::endl;
//            std::cout << "result saved for biggest confidence " << std::to_string(r) << std::endl;
//            if(final_roi.empty() == false)
//                cv::imshow("succesfull result of " + std::to_string(result), final_roi);
            //stop if detected image
            //PeopleData result_people = PeopleData(result,boundRect[i]);
            results.push_back(std::pair<int, cv::Rect>(result,boundRect[i]));
        }
        else {
            //std::cout << "read an image but could not recognize digit" << std::endl;
        }
    }
    
    return results;
}

void Optical_Character_Recognition::processImage(const std::string& filename)
{
    cv::Mat filtered;
    std::vector<cv::Rect> boundRect;
    
    // Load image from file
    cv::Mat img = loadImage(filename);
    
    preprocessing(img, filtered, boundRect);
    
    displayImage(filtered, "result");
    
    // Create Tesseract object
    tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();
    // Initialize tesseract to use English (eng)
    ocr->Init(NULL, "eng");
    // Set Page segmentation mode to PSM_SINGLE_CHAR (10)
    ocr->SetPageSegMode(tesseract::PSM_SINGLE_CHAR);
    // Only digits are valid output characters
    ocr->SetVariable("tessedit_char_whitelist", "0123456789");
    
    detection_algorithm(boundRect, filtered);
    
    ocr->End();
}
