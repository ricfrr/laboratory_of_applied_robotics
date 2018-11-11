//
//  Digit_Recognition.cpp
//  LAR_1_0
//
//  Created by Air Marvin on 09.10.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#include "../Headers/Digit_Recognition.hpp"

Digit_Recognition::Digit_Recognition(){
    std::cout << "basic digit recognition constructor selected" << std::endl;
    //initiate variables
    set_algo(DigitRecognitionAlgo::tesseractOCP);
}

Digit_Recognition::Digit_Recognition(DigitRecognitionAlgo algorithm){
    std::cout << "specific digit recognition constructor selected" << std::endl;
    //initiate variables
    set_algo(algorithm);
}


///initialized the algorithm variable with the picked algorithm
void Digit_Recognition::initialize_algorithm(){
    
    
    HSVFilterRange filter = HSVFilterRange("bad");
    if(this->algorithm != nullptr){
        filter = this->algorithm->filter;
    }
    
    switch (picked_algorithm) {
    case templateMatching:
            std::cout << "open cv algorithm is beeing used" << std::endl;
            this->algorithm = new Template_Character_Recognition;
            set_filter(filter);
            break;
        case tesseractOCP:
            std::cout << "tesseract ocr algorithm is beeing used" << std::endl;
            this->algorithm = new Optical_Character_Recognition;
            set_filter(filter);
            break;
        default:
            std::cout << "no algorithm could be set" << std::endl;
    }
}

void Digit_Recognition::set_algo(DigitRecognitionAlgo algorithm){
    this->picked_algorithm = algorithm;
    initialize_algorithm();
}

void Digit_Recognition::set_filter(HSVFilterRange filterRange){
    this->algorithm->filter = filterRange;
}

std::vector<cv::Rect> Digit_Recognition::get_regions_of_interest(cv::Mat &img){
    
    cv::Mat fil;
    std::vector<cv::Rect> bR;
    
    //do the preprocessing to apply filters and extract filtered regions
    this->algorithm->preprocessing(img, fil, bR);
    
    std::string name = "filtered_" + std::to_string(this->algorithm->filter.lb[0]) + "_" + std::to_string(this->algorithm->filter.lb[1]) + "_" + std::to_string(this->algorithm->filter.lb[2]);
    
    cv::imshow("input_", fil);
    
    std::vector<cv::Rect> results;
    
    //return all non empty rects
    for(int i = 0; i<bR.size();i++)
        if(!bR[i].empty())
            results.push_back(bR[i]);
    
    return results;
}

std::vector<PeopleData> Digit_Recognition::detect_digits_for_map(const cv::Mat img_input){
    
    //*** new Marvin
    
    cv::Mat filtered;
    std::vector<cv::Rect> rects;
    
    cv::Mat source = img_input.clone();
    
    std::vector<cv::Mat> digit_images = this->algorithm->preprocessing(source, filtered, rects);
    
    std::vector<PeopleData> results;
    
    if(digit_images.size() != rects.size())
        std::runtime_error("too many rects and not enough digits");
    
    for(int i = 0;i<digit_images.size();i++){
        //cv::imshow("digit_"+std::to_string(i), digit_images[i]);
        
        //do the noise reduction
        this->algorithm->prepare_uniform_window(digit_images[i]);
        
        //detect orientation
        double angle = this->algorithm->determine_orientation(digit_images[i]);
        
        //two different orientations
        cv::Mat orientation_0, orientation_1, orientation_2;
        orientation_0 = digit_images[i].clone();
        
        //rotate the image
        this->algorithm->rotate_image(digit_images[i], angle, orientation_1);
        angle = 180;
        this->algorithm->rotate_image(orientation_1, angle, orientation_2);
        
//        cv::imshow("orientation_0", orientation_0);
//        cv::imshow("orientation_1", orientation_1);
//        cv::imshow("orientation_2", orientation_2);
//        cv::waitKey(0);
        
        //detect the digit
        std::pair<int,int> digit_0 = this->algorithm->detect_digit(orientation_0);
        std::cout << "result for orientation_0 " << digit_0.first << std::endl;
        std::pair<int,int> digit_1 = this->algorithm->detect_digit(orientation_1);
        std::cout << "result for orientation_1 " << digit_1.first << std::endl;
        std::pair<int,int> digit_2 = this->algorithm->detect_digit(orientation_2);
        std::cout << "result for orientation_2 " << digit_2.first << std::endl;
        
        std::pair<int,int> digit = digit_0;
        
        if(digit_0.second < digit_1.second){
            digit = digit_1;
            if(digit_1.second < digit_2.second)
                digit = digit_2;
        }
       
        else if(digit_0.second < digit_2.second)
            digit = digit_2;
        
        double angle2 = this->algorithm->delta_angle;
        
        while(angle2 < 360){
            if (digit.second > 70 && digit.first >= 1 && digit.first <= 4)
                break;
            
            cv::Mat result;
            std::pair<int,int> digit_4;
            
            //rotate the image
            this->algorithm->rotate_image(digit_images[i], angle2, result);
            digit_4 = this->algorithm->detect_digit(result);
            angle2 += this->algorithm->delta_angle;
//            cv::imshow("rotated", result);
//            std::cout << digit_4.first << " " << digit_4.second <<  std::endl;
//            cv::waitKey(0);
            
            if(digit.second < digit_4.second)
                digit = digit_4;
            
            if(digit.second > 80)
                break;
        }
        
        //create the people data
        if(digit.first > 0 && digit.first <= 4)
            results.push_back(PeopleData(digit,rects[i]));
        else if (digit.first == 7)
            results.push_back(PeopleData({1,1},rects[i]));
        else
            results.push_back(PeopleData({0,0},rects[i]));
            
    }
    
    //*** old Marvin
//    cv::Mat img = img_input;
//    std::vector<cv::Rect> rects = get_regions_of_interest(img);
//
//    std::vector<PeopleData> results;
//
//    bool one = false;
//    bool two = false;
//    bool three = false;
//    bool four = false;
//
//    std::cout << "detected " << rects.size() << " images" << std::endl;
//
//    for(int i = 0; i<rects.size();i++){
//
//        //crop the image to get the part with digit
//        cv::Mat newimg(img, rects[i]);
//
//        //detect the digit
//        int result = detect_digit_for_map(newimg);
//
//        //check if the result makes sence
//        if(is_valid(result)){
//            //save the result
//            results.push_back(PeopleData(result,rects[i]));
//            std::cout << "got a result of " << result << std::endl;
//            if(result == 1)
//                one = true;
//            else if(result == 2)
//                two = true;
//            else if(result == 3)
//                three = true;
//            else if(result == 4)
//                four = true;
//        }
//        else {
//            results.push_back(PeopleData(0,rects[i]));
//        }
//    }
//
//    if(!one){
//        std::cout << "ONE is missing" << std::endl;
//    }
//    if(!two){
//        std::cout << "TWO is missing" << std::endl;
//    }
//    if(!three){
//        std::cout << "THREE is missing" << std::endl;
//    }
//    if(!four){
//        std::cout << "FOUR is missing" << std::endl;
//    }
//   // cv::waitKey(0);
    
    return results;
}

bool Digit_Recognition::is_valid(int &detectedDigit){
    switch (detectedDigit) {
        case 1:
            return true;
        case 2:
            return true;
        case 3:
            return true;
        case 4:
            return true;
    
        default:
            return false;
    }
}
