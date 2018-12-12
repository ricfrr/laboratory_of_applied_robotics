//
//  Color_Processing.cpp
//  LAR_1_0
//
//  Created by Air Marvin on 18.10.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#include "../Headers/Color_Processing.hpp"
using namespace ImageProcessing;


void Color_Processing::load_pixels(cv::Mat &img){

    //clear the pixels
    this->pixels.clear();
    
    //start the algorithm
    for (int r = 0;r<img.rows;r++){
        
        for(int c = 0;c<img.cols;c++){
            
            //get the pixel
            cv::Vec3b pixel = img.at<cv::Vec3b>(r, c);
            
            //check the hue vaue
            if(pixel[0] >= range.first && pixel[0] <= range.second){
                
                //check the other values
                if (pixel[1] >= black_threshold.first &&
                    pixel[2] >= black_threshold.second)
                    //save
                    this->pixels.push_back(pixel);
            }
        }
    }
}

HSVFilterRange Color_Processing::getFilter(){
    
    double high_hue = 0;
    double high_sat = 0;
    double high_val = 0;
    double low_hue = 180;
    double low_sat = 255;
    double low_val = 255;
    
    //get the highest and smallest values
    for (int i = 0;i<pixels.size();i++){
        
        //hue
        if(pixels[i].val[0] > high_hue)
            high_hue = pixels[i].val[0];
        
        if(pixels[i].val[0] < low_hue)
            low_hue = pixels[i].val[0];
        
        //saturation
        if(pixels[i].val[1] > high_sat)
            high_sat = pixels[i].val[1];
        
        if(pixels[i].val[1] < low_sat)
            low_sat = pixels[i].val[1];
        
        //value
        if(pixels[i].val[2] > high_val)
            high_val = pixels[i].val[2];
        
        if(pixels[i].val[2] < low_val)
            low_val = pixels[i].val[2];
    }
    
    HSVFilterRange filter = HSVFilterRange("custom");
    filter.lb = cv::Scalar(low_hue,low_sat,low_val);
    filter.ub = cv::Scalar(high_hue,high_sat,high_val);
    filter.saved_quality = "custom";

    
    return filter;
}

void Color_Processing::demo(cv::Mat &img){
    load_pixels(img);
    HSVFilterRange filter = getFilter();
}

void Color_Processing::find_black_threshold(cv::Mat &img){
    

    double sat = 255;
    double val = 255;
    double minhue = 180;
    double maxhue = 0;
    
    double dim = img.rows * img.cols;
    
    for (int r = 0;r<img.rows;r++){
        
        for(int c = 0;c<img.cols;c++){
            
            //get the pixel
            cv::Vec3b pix = img.at<cv::Vec3b>(r, c);
            
            if(pix.val[0] == 0){
                continue;
            }
            
            if(sat > pix.val[1]){
                sat = pix.val[1];
            }
            if(val > pix.val[2]){
                val = pix.val[2];
            }
            if(minhue > pix.val[0]){
                minhue = pix.val[0];
            }
            if(maxhue < pix.val[0]){
                maxhue = pix.val[0];
            }
        }
    }
    
    this->black_threshold.first = sat;
    this->black_threshold.second = val;
    
    if(range.first == 0 && range.second == 0){
        this->range.first = minhue;
        this->range.second = maxhue;
    }
}

void Color_Processing::calibrate_color(const std::string filename){
    
    cv::Mat img = cv::imread(filename);
    cv::Mat hsv;
    
    //an error here is mostly coming from a wrong filename
    cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);
    
    find_black_threshold(hsv);
    
    load_pixels(hsv);
}
