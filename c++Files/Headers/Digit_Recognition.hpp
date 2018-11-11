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

/// \brief a Data object that is beeing used to build a People Object for the map representation
/// \see People.hpp
struct PeopleData {
    /// radius of the circle
    double radius;
    /// center of the circle in the source image
    cv::Point center;
    /// detected digit
    int digit;
    /// confidence that digit is correct
    int confidence;
    
    PeopleData(std::pair<int,int> digit, cv::Rect rect){
        this->digit = digit.first;
        this->radius = std::min(rect.height,rect.width);
        this->center = cv::Point(rect.x + rect.width/2, rect.y + rect.height/2);
        this->confidence = digit.second;
    }
};

/// \brief different algorithms for recognizing digits
enum DigitRecognitionAlgo {
    templateMatching,
    tesseractOCP
};

/**
 \brief The Digit_Recognition class is used to detect digits in the arena and export PeopleData.
 
 \discussion The Digit_Recognition class is used to detect digits in the arena and export PeopleData.
 The class contains a Character_Recognition_Algorithm member that is used to detect digits. Changing the DigitRecognitionAlgo type results in the use of another implementation of Character_Recognition_Algorithm derived classes.
 New types of Character_Recognition_Algorithm subclasses can be added to improve digit recognition performance over time.
 The class contains HSVFilterRange object that is able to automatically construct a color filter from a given input image.
 When detect_digit_for_map is called the class will identify circles, use the filter to extract the digits, perform the digit recognition and export a PeopleData object that can be used to create People objects and feed the map.
 \see People
 \see Map
 */
class Digit_Recognition {
    
public:
    Digit_Recognition();
    
    /// \brief constructing the class with a set DigitRecognitionAlgo type
    /// \param algorithm a DigitRecognitionAlgo enum type that specifies the type of Character_Recognition_Algorithm used to perform the recognition of the digit
    /// \see Optical_Recognition_Algorithm, Template_Character_Recognition
    Digit_Recognition(DigitRecognitionAlgo algorithm);
    
    ///set detection algorithm for digits e.g. tesseract or template matching
    void set_algo(DigitRecognitionAlgo algorithm);
    
    ///detects digit of prepared image and checks if the result was fitting the map requierements
    int detect_digit_for_map(cv::Mat &img);
    
    ///detects all the digits of an unprepared images and returns people information
    std::vector<PeopleData> detect_digits_for_map(const cv::Mat img_input);
    
    /// \brief sets a hsv filter for better image recognition results
    /// \param filterRange a HSVFilterRange object that automatically creates a filter based on an input image
    void set_filter(HSVFilterRange filterRange);
    
    /// \brief extracts the rect information of regions where the filter was applied
    /// \return a vector of cv::Rect objects containing information about location and size of the digits in tne arg image
    std::vector<cv::Rect> get_regions_of_interest(cv::Mat &img);
    
    ///the algorithm typed used to detect digits
    DigitRecognitionAlgo picked_algorithm;
    
private:
    Character_Recognition_Algorithm * algorithm = nullptr;
    
    void initialize_algorithm();
    
    ///checks if the detected digit is between 1 and 4
    bool is_valid(int &detectedDigit);
};

#endif /* Digit_Recognition_hpp */
