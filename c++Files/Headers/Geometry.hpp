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

using namespace std;
using namespace cv;

struct Geometry{
    
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
    
};
#endif /* Geometry_hpp */
