//
//  Optical_Character_Recognition.cpp
//  LAR_1_0
//
//  Created by Air Marvin on 10.10.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#include "Optical_Character_Recognition.hpp"

Optical_Character_Recognition::Optical_Character_Recognition(){}

Optical_Character_Recognition::~Optical_Character_Recognition(){}

int Optical_Character_Recognition::detect_digit(tesseract::TessBaseAPI *&ocr, cv::Mat &image, cv::Rect &rect, cv::Mat &ROI){
    
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size((2*2) + 1, (2*2)+1));
    
    ROI = cv::Mat(image, rect); // extract the ROI containing the digit
    
    if (ROI.empty()) return -99;
    
    cv::resize(ROI, ROI, cv::Size(200, 200)); // resize the ROI
    cv::threshold( ROI, ROI, 100, 255, 0 ); // threshold and binarize the image, to suppress some noise
    
    // Apply some additional smoothing and filtering
    cv::erode(ROI, ROI, kernel);
    cv::GaussianBlur(ROI, ROI, cv::Size(5, 5), 2, 2);
    cv::erode(ROI, ROI, kernel);
    
    // Set image data
    ocr->SetImage(ROI.data, ROI.cols, ROI.rows, 3, ROI.step);
    
    // know your ASCI table
    int result = *ocr->GetUTF8Text() - '0';
    
    if(result == -16) return -99;
    
    // Run Tesseract OCR on image and print recognized digit
    std::cout << "Recognized digit: " << std::string(ocr->GetUTF8Text()) << std::endl;
    
    return result;
}

int Optical_Character_Recognition::detect_digit(cv::Mat &image, cv::Rect &rect, cv::Mat &ROI){
    
    // Create Tesseract object
    tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();
    // Initialize tesseract to use English (eng)
    ocr->Init(NULL, "eng");
    // Set Page segmentation mode to PSM_SINGLE_CHAR (10)
    ocr->SetPageSegMode(tesseract::PSM_SINGLE_CHAR);
    // Only digits are valid output characters
    ocr->SetVariable("tessedit_char_whitelist", "0123456789");
    
    // know your ASCI table
    int result = detect_digit(ocr, image, rect, ROI);
    
    ocr->End();
    
    return result;
}

void Optical_Character_Recognition::getResult(tesseract::TessBaseAPI *&ocr, cv::Mat &img, int &result){
    // Set image data
    ocr->SetImage(img.data, img.cols, img.rows, 3, img.step);
    
    // know your ASCI table
    result = *ocr->GetUTF8Text() - '0';
}

void Optical_Character_Recognition::processImage(const std::string& filename)
{
    cv::Mat filtered;
    std::vector<cv::Rect> boundRect;
    
    // Load image from file
    cv::Mat img = loadImage(filename);
    
    preprocessing(img, filtered, boundRect);
    
    // Create Tesseract object
    tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();
    // Initialize tesseract to use English (eng)
    ocr->Init(NULL, "eng");
    // Set Page segmentation mode to PSM_SINGLE_CHAR (10)
    ocr->SetPageSegMode(tesseract::PSM_SINGLE_CHAR);
    // Only digits are valid output characters
    ocr->SetVariable("tessedit_char_whitelist", "0123456789");
    
    //go through all rects and see if it contains a digit
    for (int i=0; i<boundRect.size(); ++i)
    {
        // roi is the image passed to the tesseract engine
        cv::Mat roi;
        
        // run the tesseract engine
        int result = detect_digit(ocr, filtered, boundRect[i], roi);
        
        double angle = 0;
        bool entered = false;
        
        if (result != -99) angle = 360;
        
        DigitResultDistribution dis = DigitResultDistribution();
        
        if(!roi.empty()){
            cv::imshow("Detecting", roi);}
        
        while(angle < 360 && !roi.empty()){
            entered = true;
            //std::cout << "rotating image and retrying" << std::endl;
            if(angle == 0){
                cv::imshow("Problem", roi);
            
            }

            cv::Mat roi2,roi3;
            rotate_image(roi, angle, roi2);
            
            //get the result
            getResult(ocr, roi2, result);
            
            if(result == -16) result = -99;
            
            if (result != -99) {
                dis.add(result);
            }

            angle += 45;
        }
        
        if (entered) {
            result = dis.best();
        }
        
        //no digit is result -99
        if (result != -99) {
            std::cout << "result was " << std::to_string(result)  << std::endl;
            //stop if detected image
            cv::waitKey(0);}
        else {
            std::cout << "read an image but could not recognize digit" << std::endl;
        }
    }
    
    ocr->End();

    
}
