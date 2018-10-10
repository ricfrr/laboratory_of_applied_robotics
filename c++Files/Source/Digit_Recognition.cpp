//
//  Digit_Recognition.cpp
//  LAR_1_0
//
//  Created by Air Marvin on 09.10.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#include "Digit_Recognition.hpp"

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

void Digit_Recognition::run_demo(const std::string filename){
    this->algortihm->processImage(filename);
}

///initialized the algorithm variable with the picked algorithm
void Digit_Recognition::initialize_algorithm(){
    switch (picked_algorithm) {
    case opencv:
            std::cout << "open cv algorithm is beeing used" << std::endl;
            break;
        case tesseractOCP:
            std::cout << "tesseract ocr algorithm is beeing used" << std::endl;
            this->algortihm = new Optical_Character_Recognition;
            break;
        default:
            std::cout << "no algorithm could be set" << std::endl;
    }
}

void Digit_Recognition::set_algo(DigitRecognitionAlgo algorithm){
    this->picked_algorithm = algorithm;
    initialize_algorithm();
}
