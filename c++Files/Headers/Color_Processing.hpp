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

struct HSVFilterRange {
    //works best for the good image
    //    cv::Scalar lb = cv::Scalar(30, 70, 70);
    cv::Scalar ub = cv::Scalar(90, 255, 255);
    //    cv::Scalar lb = cv::Scalar(51, 100, 79);
    
    cv::Scalar lb = cv::Scalar(48, 53, 102);
    //cv::Scalar ub = cv::Scalar(67, 143, 176);
    
    
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


class Color_Processing {
public:
    Color_Processing(){};
    
    std::pair<double,double> range = std::pair<double,double> (0,0); // (45,82.5);
    
    std::pair<double,double> black_threshold = std::pair<double,double> (0,0);// (0.425*255,0.175*255);
    
    std::vector<cv::Vec3b> pixels;
    
    void load_pixels(cv::Mat &img);
    
    HSVFilterRange getFilter();
    
    void find_black_threshold(cv::Mat &img);
    
    void calibrate_color(const std::string filename);
    
    void demo(cv::Mat &img);
    
};

#endif /* Color_Processing_hpp */
