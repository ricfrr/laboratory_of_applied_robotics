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

cv::Mat Optical_Character_Recognition::loadImage(const std::string& filename){
    // Load image from file
    cv::Mat img = cv::imread(filename.c_str());
    if(img.empty()) {
        throw std::runtime_error("Failed to open the file " + filename);
    }
    return img;
}

void Optical_Character_Recognition::displayImage(cv::Mat & image,std::string windowTitle){
    cv::imshow(windowTitle, image);
}

void Optical_Character_Recognition::convert_bgr_to_hsv(cv::Mat &original, cv::Mat &converted){
    // Convert color space from BGR to HSV
    cv::cvtColor(original, converted, cv::COLOR_BGR2HSV);
}

void Optical_Character_Recognition::apply_mask(cv::Mat &original, cv::Mat &converted, cv::Scalar lowerbound, cv::Scalar upperbound){
    cv::inRange(original,lowerbound,upperbound, converted);
}

cv::Mat Optical_Character_Recognition::apply_some_filtering(cv::Mat &img){
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size((1*2) + 1, (1*2)+1));
    cv::dilate(img, img, kernel);
    cv::erode(img, img, kernel);
    return kernel;
}

std::vector<cv::Rect> Optical_Character_Recognition::extract_regions_of_interest(cv::Mat &original_img,cv::Mat & filtered_img,cv::Mat &returnedImg){
    
    std::vector<std::vector<cv::Point>> contours, contours_approx;
    std::vector<cv::Point> approx_curve;
    
    returnedImg = original_img.clone();
    cv::findContours(filtered_img, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    std::vector<cv::Rect> boundRect(contours.size());
    for (int i=0; i<contours.size(); ++i)
    {
        double area = cv::contourArea(contours[i]);
        if (area < this->MIN_AREA_SIZE) continue; // filter too small contours to remove false positives
        approxPolyDP(contours[i], approx_curve, 2, true);
        contours_approx = {approx_curve};
        drawContours(returnedImg, contours_approx, -1, cv::Scalar(0,170,220), 3, cv::LINE_AA);
        boundRect[i] = boundingRect(cv::Mat(approx_curve)); // find bounding box for each green blob
    }
    
    return boundRect;
}

std::tuple<cv::Mat,cv::Mat> Optical_Character_Recognition::invert_masked_image(cv::Mat &original, cv::Mat &masked_image){
    
    cv::Mat mask_inv, filtered(original.rows, original.cols, CV_8UC3, cv::Scalar(255,255,255));
    cv::bitwise_not(masked_image, mask_inv); // generate binary mask with inverted pixels w.r.t. green mask -> black numbers are part of this mask
    original.copyTo(filtered, mask_inv);   // create copy of image without green shapes
    return std::tuple<cv::Mat,cv::Mat>(mask_inv, filtered);
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
    
    // Run Tesseract OCR on image and print recognized digit
    std::cout << "Recognized digit: " << std::string(ocr->GetUTF8Text()) << std::endl;
    
    int result = *ocr->GetUTF8Text() - '0';
    
    ocr->End();
    
    return result;
}

void Optical_Character_Recognition::processImage(const std::string& filename)
{
    // Load image from file
    cv::Mat img = loadImage(filename);
    
    // Display original image
    displayImage(img, "Original");
    
    // Convert color space from BGR to HSV
    cv::Mat hsv_img;
    convert_bgr_to_hsv(img, hsv_img);
    
    
    // Find green regions
    cv::Mat green_mask;
    apply_mask(hsv_img, green_mask, cv::Scalar(45, 30, 30), cv::Scalar(75, 255, 255));
    
    // Apply some filtering
    cv::Mat kernel = apply_some_filtering(green_mask);
    
    // Display image
    displayImage(green_mask, "GREEN_filter");
    
    
    // Find contours
    cv::Mat contours_img;
    std::vector<cv::Rect> boundRect = extract_regions_of_interest(img, green_mask,contours_img);
    
    //displaying
    displayImage(contours_img, "Original");
    cv::waitKey(0);
    
    //invert the pixels black white
    std::tuple<cv::Mat,cv::Mat> inversionResult = invert_masked_image(img, green_mask);
    cv::Mat green_mask_inv  = std::get<0>(inversionResult); //only for displaying purposes
    cv::Mat filtered        = std::get<1>(inversionResult); // needed to detect digit
    
    //displaying some more
    displayImage(green_mask_inv, "Numbers");
    cv::waitKey(0);
    
    //go through all rects and see if it contains a digit
    for (int i=0; i<boundRect.size(); ++i)
    {
        cv::Mat roi;
        int result = detect_digit(filtered, boundRect[i], roi);
        
        //no digit is result -99
        if (result != -99) {
            // Show the actual image passed to the ocr engine
            cv::imshow("ROI", roi);
            //stop if detected image
            cv::waitKey(0);}
    }

    
}
