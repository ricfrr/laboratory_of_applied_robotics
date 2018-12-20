//
//  Digit_Recognition.cpp
//  LAR_1_0
//
//  Created by Air Marvin on 09.10.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#include "../Headers/Digit_Recognition.hpp"
using namespace ImageProcessing;

Digit_Recognition::Digit_Recognition(const DigitRecognitionAlgo &algorithm, const unsigned int &suff_confidence, const unsigned int &search_angle, const double &d_angle, const unsigned int &extra_eroding){
    
    set_algo(algorithm, suff_confidence,search_angle,d_angle,extra_eroding);
    
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

void Digit_Recognition::set_algo(const DigitRecognitionAlgo &algorithm, const unsigned int &suff_confidence, const unsigned int &search_angle, const double &d_angle, const unsigned int &extra_eroding){
    
    this->picked_algorithm = algorithm;
    
    initialize_algorithm();
    
    this->algorithm->suf_conf = suff_confidence;
    this->algorithm->search_angle = search_angle;
    this->algorithm->delta_angle = d_angle;
    this->algorithm->erode_times = extra_eroding;
    
    
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
    
    //cv::imshow("input_", fil);
    
    std::vector<cv::Rect> results;
    
    //return all non empty rects
    for(int i = 0; i<bR.size();i++)
        if(!bR[i].empty())
            results.push_back(bR[i]);
    
    return results;
}

std::vector<LAR::People> Digit_Recognition::detect_digits_for_map(const cv::Mat img_input){
    
    //*** new Marvin
    
    std::cout << "\nstarted detecting the digits ";
    
    cv::Mat filtered;
    std::vector<cv::Rect> rects;

    cv::Mat source = img_input.clone();

    std::vector<cv::Mat> digit_images = this->algorithm->preprocessing(source, filtered, rects);
    

    std::vector<LAR::People> results;
    
    if(digit_images.size() != rects.size())
        std::runtime_error("too many rects and not enough digits");

    for(int i = 0;i<digit_images.size();i++){
        //cv::imshow("digit_"+std::to_string(i), digit_images[i]);

        std::cout << ".";
        //do the noise reduction
        this->algorithm->prepare_uniform_window(digit_images[i]);

        //detect orientation
        double angle = this->algorithm->determine_orientation(digit_images[i]);

        //two different orientations
        cv::Mat orientation_0, orientation_1, orientation_2;
        orientation_0 = digit_images[i].clone();

        //rotate the image
        this->algorithm->rotate_image(digit_images[i], -angle, orientation_1);
        this->algorithm->rotate_image(orientation_1, 180, orientation_2);

//        cv::imshow("orientation_0", orientation_0);
//        cv::imshow("orientation_1", orientation_1);
//        cv::imshow("orientation_2", orientation_2);
//        cv::waitKey(0);

//        set_algo(templateMatching);

        if(this->picked_algorithm == templateMatching){
            cv::bitwise_not(orientation_0, orientation_0);
            cv::bitwise_not(orientation_1, orientation_1);
            cv::bitwise_not(orientation_2, orientation_2);

//            cv::imshow("orientation_0", orientation_0);
//            cv::imshow("orientation_1", orientation_1);
//            cv::imshow("orientation_2", orientation_2);
//            cv::waitKey(0);
        }

        //detect the digit
        std::pair<int,int> digit_0 = this->algorithm->detect_digit(orientation_0);
        //std::cout << "result for orientation_0 " << digit_0.first << std::endl;
        std::pair<int,int> digit_1 = this->algorithm->detect_digit(orientation_1);
        //std::cout << "result for orientation_1 " << digit_1.first << std::endl;
        std::pair<int,int> digit_2 = this->algorithm->detect_digit(orientation_2);
        //std::cout << "result for orientation_2 " << digit_2.first << std::endl;

        std::pair<int,int> digit = digit_0;

        //starting to turn the digit
        double angle2 =  - (double)this->algorithm->search_angle;

        while(angle2 <=  (double)this->algorithm->search_angle){

            if (digit_1.second > this->algorithm->suf_conf - 10 &&
                is_valid(digit.first)){
                digit = digit_1;
                
                if (digit_2.second > digit_1.second &&
                    is_valid(digit.first)){
                    digit = digit_2;
                    break;
                }
                
                break;
            }else if (digit_2.second > this->algorithm->suf_conf - 10 &&
                                  is_valid(digit.first)){
                digit = digit_2;
                break;
            }
            

            cv::Mat result1, result2;
            std::pair<int,int> digit_4;
            std::pair<int,int> digit_5;

            //rotate the image
            this->algorithm->rotate_image(orientation_1, angle2, result1);
            this->algorithm->rotate_image(orientation_2, angle2, result2);
            
            digit_4 = this->algorithm->detect_digit(result1);
            digit_5 = this->algorithm->detect_digit(result2);
            
            angle2 += this->algorithm->delta_angle;
            
//            cv::imshow("rotated4", result1);
//            cv::imshow("rotated5", result2);
            
           // cv::waitKey(0);

            if(digit_1.second < digit_4.second)
                digit_1 = digit_4;
            if(digit_2.second < digit_5.second)
                digit_2 = digit_5;

            if(digit_1.second > this->algorithm->suf_conf ){
                digit = digit_1;
                break;
            }
            else if(digit_2.second > this->algorithm->suf_conf ){
                digit = digit_2;
                break;
            }
        }

        //create the people data
        if(is_valid(digit.first))
            results.push_back(LAR::People(digit,rects[i]));
        else if (digit.first == 7)
            results.push_back(LAR::People({1,1},rects[i]));
        else
            results.push_back(LAR::People({0,0},rects[i]));

    }
    
    std::cout << "done!\n" << std::endl;
    
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
        case 7:
            return false;
    
        default:
            return true;
    }
}
