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

std::pair<int,int> Template_Character_Recognition::detect_digit(cv::Mat &image){
    
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
    
    return {0,0};
}
