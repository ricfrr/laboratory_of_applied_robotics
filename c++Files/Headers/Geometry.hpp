//
//  Geometry.hpp
//  LAR_1_0
//
//  Created by Air Marvin on 22.12.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#ifndef Geometry_hpp
#define Geometry_hpp

#include <opencv2/core.hpp>
#include <stdio.h>

#include "Settings.hpp"

using namespace cv;

struct Geometry{
    
public:
    
    Geometry()    {    }
    
    static double angle_deg(const Point start, const Point end)
    {
        cv::Point result = end - start;
        double rho = std::sqrt(std::pow(result.x,2) + std::pow(result.y,2));
        double angle = std::atan2(result.y/rho, result.x/rho);
        return (angle) * 57.2958;
    }
    
    static double angle_rad(const Point start, const Point end)
    {
        cv::Point result = end - start;
        double rho = std::sqrt(std::pow(result.x,2) + std::pow(result.y,2));
        double angle = std::atan2(result.y/rho, result.x/rho);
        return (angle);
    }

    static double angle_rad_for_robots(const Point start, const Point end)
    {
        cv::Point result = end - start;
        double rho = std::sqrt(std::pow(result.x,2) + std::pow(result.y,2));
        double angle = std::atan2(result.y/rho, result.x/rho);
        return (angle);
    }


    static std::pair<double,double> convertPixelToMillimeterInMapPlane(const cv::Point &point, const cv::Point &reference){
        
        return convertPixelToMillimeter(point, reference, ImageProcessing::Settings::PIXEL_SCALE);
    }
    
    static std::pair<double,double> convertPixelToMillimeterInRoboPlane(const cv::Point &point, const cv::Point &reference){
        
        return convertPixelToMillimeter(point, reference, ImageProcessing::Settings::ROBO_PIXEL_SCALE);
    }
    
private:
    static std::pair<double,double> convertPixelToMillimeter(const cv::Point &point, const cv::Point &reference, const double &scale){
        
        double x = (point.x - reference.x) /scale;
        double y = (point.y - reference.y) /scale;
        
        return std::pair<double,double>(x,y);
    }
    
};
#endif /* Geometry_hpp */
