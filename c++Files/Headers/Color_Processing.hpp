//
//  Color_Processing.hpp
//  LAR_1_0
//
//  Created by Air Marvin on 18.10.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#ifndef Color_Processing_hpp
#define Color_Processing_hpp

#include <stdio.h>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

/**
 \brief A structure that helps creating a color filter
 
 \discussion This structure contains several filter values for a given tag. It is able to store different values and dynamically change to another filter while keeping another filter in the back up. This allows the use of different filters to find a fitting one for a specific image
 */
struct HSVFilterRange {
    //works best for the good image
    //    cv::Scalar lb = cv::Scalar(30, 70, 70);
    ///the upper bound of the filter
    cv::Scalar ub = cv::Scalar(90, 255, 255);
    //    cv::Scalar lb = cv::Scalar(51, 100, 79);
    
    ///the lower bound of the filter
    cv::Scalar lb = cv::Scalar(48, 53, 102);
    //cv::Scalar ub = cv::Scalar(67, 143, 176);
    
    
private:
    ///the tage
    std::string quality = "bad";
    
public:
    //works best for the worst image
    //    cv::Scalar lb = cv::Scalar(30, 70, 70);
    //    cv::Scalar ub = cv::Scalar(90, 255, 255);
    
    HSVFilterRange(){}
    
    ///the saved tag
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
            //            this->lb = cv::Scalar(30, 70, 70);
            //            this->ub = cv::Scalar(90, 255, 255);
            //            this->ub = cv::Scalar(72, 255, 133);
            //            this->lb = cv::Scalar(27, 58, 66);
            this->lb = cv::Scalar(65, 30, 80);
            this->ub = cv::Scalar(75, 255, 255);
            
            this->quality = quality;
            saved_quality = quality;
        }
    }
};

// 123456789
// !“§$%&/|\
// ()
// []
// {}


/**
 \brief A class that helps to extract information about the color values of pixels in an image
 
 \discussion This class helps generating a color filter given an image. It will run through the pixels of an input image and store the highest and lowest HSV values. Given that information it creates a filter for that color range.
 Also it is possible to construct a black filter, meaning to get the lowest SV values of an Image. This way it is possible to extract dark regions of an image
 */
class Color_Processing {
public:
    Color_Processing(){};
    
    ///when implemeting a black filter the lowest SV values are stored in the range
    std::pair<double,double> range = std::pair<double,double> (0,0); // (45,82.5);
    
    ///the givent threshold - acts like an additional black filter
    std::pair<double,double> black_threshold = std::pair<double,double> (0,0);// (0.425*255,0.175*255);
    
    ///the pixels stored for evaluation
    std::vector<cv::Vec3b> pixels;
    
    ///load all relevant pixels for evaluation
    void load_pixels(cv::Mat &img);
    
    ///creating and return an HSVFilterRange filter object
    HSVFilterRange getFilter();
    
    ///find the lower values of s and v
    void find_black_threshold(cv::Mat &img);
    
    ///use an image to create a color filter
    void calibrate_color(const std::string filename);
    
    ///a demo function
    void demo(cv::Mat &img);
    
};

#endif /* Color_Processing_hpp */
