//
//  Character_Recognition_Algorithm.hpp
//  LAR_1_0
//
//  Created by Air Marvin on 10.10.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#ifndef Character_Recognition_Algorithm_hpp
#define Character_Recognition_Algorithm_hpp

#include <stdio.h>
#include <string>

#include <iostream>

#include "Color_Processing.hpp"

template < typename T>
int findInVector(const std::vector<T>  & vecOfElements, const T  & element)
{
    int result;
    
    // Find given element in vector
    auto it = std::find(vecOfElements.begin(), vecOfElements.end(), element);
    
    if (it != vecOfElements.end())
    {
        result = distance(vecOfElements.begin(), it);
    }
    else
    {
        result = -99;
    }
    
    return result;
}

struct DigitResultDistribution {
    int zero = 0;
    int one = 0;
    int two = 0;
    int three = 0;
    int four = 0;
    int five = 0;
    int six = 0;
    int seven = 0;
    int eight = 0;
    int nine = 0;
    
    int best(){
        std::vector<int> v{zero,one,two,three,four,five,six,seven,eight,nine};
        auto it = *std::max_element(std::begin(v), std::end(v));

        int i = findInVector(v,it);
        
        //no result
        if(i==0 && zero == 0) return -99;
        
        return i;
    }
    
    void add(int number){
        switch (number) {
            case 0:
                zero += 1;
                break;
            case 1:
                one += 1;
                break;
            case 2:
                two += 1;
                break;
            case 3:
                three += 1;
                break;
            case 4:
                four += 1;
                break;
            case 5:
                five += 1;
                break;
            case 6:
                six += 1;
                break;
            case 7:
                seven += 1;
                break;
            case 8:
                eight += 1;
                break;
            case 9:
                nine += 1;
                break;
                
            default:
                break;
        }
    }
};


/// abstract class for character recognition algorithms
class Character_Recognition_Algorithm {

public:
    Character_Recognition_Algorithm();
    ~Character_Recognition_Algorithm(){}
    
    const double MIN_AREA_SIZE = 200;
    
    HSVFilterRange filter = HSVFilterRange();
    
    //the folowing functions are used for preparing an image to find the parts containing digits
    //these steps are similar for every recognition algorithm
    
    ///calls cv::imshow
    void displayImage(cv::Mat & image,std::string windowTitle);
    
    ///computes an angle based on the fitline method
    double determine_orientation(cv::Mat image);
    
    ///the (virtual) function that runs the recognition engine
    virtual std::pair<int,int> detect_digit(cv::Mat &image) = 0;
    
    ///calls cv::imread
    cv::Mat loadImage(const std::string& filename);
    
    ///convertion if color sheme
    void convert_bgr_to_hsv(cv::Mat &original, cv::Mat &converted);
    
    ///calls cv::inrange function
    void apply_mask(cv::Mat &original, cv::Mat &converted, cv::Scalar lowerbound, cv::Scalar upperbound);
    
    ///dilate and erode image
    cv::Mat apply_some_filtering(cv::Mat &img);
    
    ///resizing to 200x200px, threshold, erode and blur
    void prepare_uniform_window(cv::Mat &img);
    
    ///extract regions that circular
    std::vector<cv::Rect> extract_regions_of_interest(cv::Mat &original_img,
                                                      cv::Mat & filtered_img,
                                                      cv::Mat &returnedImg);
    
    ///inverts the image colors black white - white black
    std::tuple<cv::Mat,cv::Mat> invert_masked_image(cv::Mat &original, cv::Mat &masked_image);
    
    ///rotates the image and produces a new image with black background
    void rotate_image(cv::Mat &src, double angle, cv::Mat &result);
    
    ///applies the color filter and detects circular objects and returns images of found regions
    std::vector<cv::Mat> preprocessing(cv::Mat &img, cv::Mat &filtered, std::vector<cv::Rect> &boundRect);
    
    ///set the lower value for filter
    void set_lower_bound_filter(double hue, double saturation, double value);
    
    ///set the upper value for filter
    void set_upper_bound_filter(double hue, double saturation, double value);
    
    ///calls rotate image
    void turn_image(cv::Mat input, cv::Mat & output, double angle);
    
    ///angle by which the image is turned each step from 0 to 360 degrees to find the right orientation
    double delta_angle = 15;
};

#endif /* Character_Recognition_Algorithm_hpp */
