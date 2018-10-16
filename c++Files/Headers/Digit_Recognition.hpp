//
//  Digit_Recognition.hpp
//  LAR_1_0
//
//  Created by Air Marvin on 09.10.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#ifndef Digit_Recognition_hpp
#define Digit_Recognition_hpp

#include <stdio.h>
#include <iostream>

//tesseract doesn't work when using namespace std
//delete every using namespace std line in the previous code to compile this code
#include "Optical_Character_Recognition.hpp"
#include "Template_Character_Recognition.hpp"

struct PeopleData {
    double radius;
    cv::Point center;
    int digit;
    
    PeopleData(int digit, cv::Rect rect){
        this->digit = digit;
        this->radius = std::min(rect.height,rect.width);
        this->center = cv::Point(rect.x + rect.width/2, rect.y + rect.height/2);
    }
};

//different algorithms for recognizing digits
enum DigitRecognitionAlgo {
    templateMatching,
    tesseractOCP
};

///Class for Recognizing Digits
class Digit_Recognition {
    
public:
    Digit_Recognition();
    Digit_Recognition(DigitRecognitionAlgo algorithm);
    
    ///set detection algorithm for digits e.g. tesseract or template matching
    void set_algo(DigitRecognitionAlgo algorithm);
    
    ///run the algorithm on an image containing digits
    void run_demo(const std::string filename);
    
    ///detect a single digit in an prepared image
    ///will return first detected digit
    int detect_single_digit(cv::Mat &img);
    
    ///returns a list of digits recognized in an prepared image
    std::vector<int> detect_digits(cv::Mat &img);
    
    ///returns a list of PeopleData Objects recognized in an prepared image
    std::vector<PeopleData> detect_peopleData(cv::Mat &img);
    
    ///detects digit of prepared image and checks if the result was fitting the map requierements
    int detect_digit_for_map(cv::Mat &img);
    
    ///detects all the digits of an unprepared images and returns people information
    std::vector<PeopleData> detect_digits_for_map(const cv::Mat img_input);
    
    ///sets a hsv filter for better image recognition results
    void set_filter(HSVFilterRange filterRange);
    
    ///extracts the rect information of regions where the filter was applied
    std::vector<cv::Rect> get_regions_of_interest(cv::Mat &img);
    
private:
    ///the algorithm used to
    DigitRecognitionAlgo picked_algorithm;
    
    Character_Recognition_Algorithm * algortihm = nullptr;
    
    void initialize_algorithm();
    
    ///checks if the detected digit is between 1 and 4
    bool is_valid(int &detectedDigit);
};

#endif /* Digit_Recognition_hpp */
