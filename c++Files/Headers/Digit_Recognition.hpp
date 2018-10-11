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

//different algorithms for recognizing digits
enum DigitRecognitionAlgo {
    opencv,
    tesseractOCP
};

///Class for Recognizing Digits
class Digit_Recognition {
    
public:
    Digit_Recognition();
    Digit_Recognition(DigitRecognitionAlgo algorithm);
    
    void set_algo(DigitRecognitionAlgo algorithm);
    
    void run_demo(const std::string filename);
    
private:
    ///the algorithm used to
    DigitRecognitionAlgo picked_algorithm;
    
    Character_Recognition_Algorithm * algortihm;
    
    void initialize_algorithm();
};

#endif /* Digit_Recognition_hpp */
