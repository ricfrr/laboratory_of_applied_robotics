//
//  Template_Character_Recognition.cpp
//  LAR_1_0
//
//  Created by Air Marvin on 11.10.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#include "../Headers/Template_Character_Recognition.hpp"

Template_Character_Recognition::Template_Character_Recognition(){
    this->filter = HSVFilterRange("bad");
}

Template_Character_Recognition::~Template_Character_Recognition(){}

void Template_Character_Recognition::processImage(const std::string& filename)
{
    // Load image from file
    cv::Mat img = cv::imread(filename.c_str());
    if(img.empty()) {
        throw std::runtime_error("Failed to open the file " + filename);
    }
    
    cv::Mat filtered;
    std::vector<cv::Rect> boundRect;
    
    preprocessing(img, filtered, boundRect);
    
    detection_algorithm(boundRect, filtered);
    
}

int Template_Character_Recognition::getResult(std::vector<cv::Mat> &templROIs, cv::Mat &ROI){
    // Find the template digit with the best matching
    double maxScore = 0;
    int maxIdx = -99;
    for (int j=0; j<templROIs.size(); ++j) {
        cv::Mat result;
        cv::matchTemplate(ROI, templROIs[j], result, cv::TM_CCOEFF);
        double score;
        cv::minMaxLoc(result, nullptr, &score);
        if (score > maxScore) {
            maxScore = score;
            maxIdx = j;
        }
    }
    
    //wanna have a high maxScore = ressemblance
    //empirical tested this number to be a good fit
    if (maxScore < 625000000){
        return -99;
    }
//    else {
//        std::cout << "detected digit with max score " << std::to_string(maxScore) << std::endl;
//    }
    return maxIdx;
}

int Template_Character_Recognition::detect_digit(cv::Mat &image){
    
//    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size((2*2) + 1, (2*2)+1));
//
//    // Load digits template images
//    std::vector<cv::Mat> templROIs;
//    for (int i=0; i<=9; ++i) {
//        templROIs.emplace_back(cv::imread(template_path + std::to_string(i) + ".png"));
//    }
//
//    ROI = cv::Mat(image, rect); // extract the ROI containing the digit
//
//        if (ROI.empty()) return -99;
//
//        cv::resize(ROI, ROI, cv::Size(200, 200)); // resize the ROI
//        cv::threshold( ROI, ROI, 100, 255, 0 ); // threshold and binarize the image, to suppress some noise
//
//        // Apply some additional smoothing and filtering
//        cv::erode(ROI, ROI, kernel);
//        cv::GaussianBlur(ROI, ROI, cv::Size(5, 5), 2, 2);
//        cv::erode(ROI, ROI, kernel);
//
//        // Show the actual image used for the template matching
//        // cv::imshow("ROI", ROI);
//
//        // Find the template digit with the best matching
//        int maxIdx = getResult(templROIs, ROI);
//
//        //std::cout << "Best fitting template: " << maxIdx << std::endl;
//
//    return maxIdx;
    
    return 0;
}

std::vector<std::pair<int,cv::Rect>> Template_Character_Recognition::detection_algorithm(std::vector<cv::Rect> &boundRect, cv::Mat &filtered){
    
    std::vector<std::pair<int,cv::Rect>> results;
    
    cv::Mat roi;
    // For each green blob in the original image containing a digit
    for (int i=0; i<boundRect.size(); ++i)
    {
        
        cv::Mat img = cv::Mat(filtered,boundRect[i]);
        int result = detect_digit(img);
        
        double angle = 0;
        bool entered = false;
        
        if (result != -99) angle = 360;
        
        DigitResultDistribution dis = DigitResultDistribution();
        
//        if(!roi.empty()){
//            cv::imshow("Detecting", roi);}
        
        while(angle < 360 && !roi.empty()){
            entered = true;
            //std::cout << "rotating image and retrying" << std::endl;
//            if(angle == 0){
//                cv::imshow("Problem", roi);
//
//            }
            
            cv::Mat roi2,roi3;
            rotate_image(roi, angle, roi2);
            
            //get the result
            // Load digits template images
            std::vector<cv::Mat> templROIs;
            for (int i=0; i<=9; ++i) {
                templROIs.emplace_back(cv::imread(template_path + std::to_string(i) + ".png"));
            }
            result = getResult(templROIs, roi2);
            
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
            //std::cout << "result was " << std::to_string(result)  << std::endl;
            results.push_back(std::pair<int, cv::Rect>(result,boundRect[i]));
            //results.push_back(result);
        }
        else {
            //std::cout << "read an image but could not recognize digit" << std::endl;
        }
    }
    
    return results;
}
