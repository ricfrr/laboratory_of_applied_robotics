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

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

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

struct HSVFilterRange {
    //works best for the good image
    cv::Scalar lb = cv::Scalar(30, 70, 70);
    cv::Scalar ub = cv::Scalar(90, 255, 255);
    
private:
    std::string quality = "bad";
    
public:
    //works best for the worst image
//    cv::Scalar lb = cv::Scalar(30, 70, 70);
//    cv::Scalar ub = cv::Scalar(90, 255, 255);
    
    HSVFilterRange(){}
    
    std::string saved_quality = "bad";
    
    HSVFilterRange(std::string quality){
        if (quality == "good"){
            this->lb = cv::Scalar(65, 30, 80);
            this->ub = cv::Scalar(75, 255, 255);
            this->quality = quality;
            saved_quality = quality;
        }
        else if (quality == "medium"){
            this->lb = cv::Scalar(30, 100, 55);
            this-> ub = cv::Scalar(90, 255, 255);
            this->quality = quality;
            saved_quality = quality;
        }
        else if (quality == "bad"){
            this->lb = cv::Scalar(30, 70, 70);
            this->ub = cv::Scalar(90, 255, 255);
            this->quality = quality;
            saved_quality = quality;
        }
    }
};

/// abstract class for character recognition algorithms
class Character_Recognition_Algorithm {

public:
    Character_Recognition_Algorithm();
    ~Character_Recognition_Algorithm(){}
    
    const double MIN_AREA_SIZE = 100;
    
    HSVFilterRange filter = HSVFilterRange();
    
    //pure virtual function, to run the demo like the public code gave us
    ///a demo function displaying the performance of a given algorithm
    virtual void processImage(const std::string& filename) = 0;
    
    ///the (virtual) function that runs the recognition engine
    virtual int detect_digit(cv::Mat &image, cv::Rect &rect, cv::Mat &ROI) = 0;
    
    virtual std::vector<std::pair<int,cv::Rect>> detection_algorithm(std::vector<cv::Rect> &boundRect, cv::Mat &filtered) = 0;
    
    //the folowing functions are used for preparing an image to find the parts containing digits
    //these steps are similar for every recognition algorithm
    
    void displayImage(cv::Mat & image,std::string windowTitle);
    
    cv::Mat loadImage(const std::string& filename);
    
    void convert_bgr_to_hsv(cv::Mat &original, cv::Mat &converted);
    
    void apply_mask(cv::Mat &original, cv::Mat &converted, cv::Scalar lowerbound, cv::Scalar upperbound);
    
    cv::Mat apply_some_filtering(cv::Mat &img);
    
    std::vector<cv::Rect> extract_regions_of_interest(cv::Mat &original_img,
                                                      cv::Mat & filtered_img,
                                                      cv::Mat &returnedImg);
    
    std::tuple<cv::Mat,cv::Mat> invert_masked_image(cv::Mat &original, cv::Mat &masked_image);
    
    void rotate_image(cv::Mat &src, double angle, cv::Mat &result);
    
    void preprocessing(cv::Mat &img, cv::Mat &filtered, std::vector<cv::Rect> &boundRect);
    
    void set_lower_bound_filter(double hue, double saturation, double value);
    
    void set_upper_bound_filter(double hue, double saturation, double value);
    
    double delta_angle = 45;
};

#endif /* Character_Recognition_Algorithm_hpp */
