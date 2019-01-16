//
//  Robot.cpp
//  LAR_1_0
//
//  Created by Air Marvin on 23.11.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#include "../Headers/Robot.hpp"

Robot::Robot(const std::vector<cv::Point> &points): Triangle(points){
    
    if(points.size() > 0)
        update(points);
}
Robot::~Robot(){}

void Robot::update(const std::vector<cv::Point> &points){
    
    this->points = points;
    setCorners(points);
    
    center = points[0] + points[1] + points[2];
    center = center/3;
    
    //find shorter side
    int a_x = std::abs(points[0].x - points[1].x);
    int b_x = std::abs(points[0].x - points[2].x);
    int c_x = std::abs(points[1].x - points[2].x);

    int a_y = std::abs(points[0].y - points[1].y);
    int b_y = std::abs(points[0].y - points[2].y);
    int c_y = std::abs(points[1].y - points[2].y);
    
    cv::Point a(a_x,a_y);
    cv::Point b(b_x,b_y);
    cv::Point c(c_x,c_y);
    
    double La = sqrt(pow(a_x,2) + pow(a_y,2));
    double Lb = sqrt(pow(b_x,2) + pow(b_y,2));
    double Lc = sqrt(pow(c_x,2) + pow(c_y,2));
    
    cv::Point result;
    double deg = 0.0;
    
    if(La <= Lb && La <= Lc){
        cv::Point start = points[1] + a/2;
        cv::Point end = points[2];
        angle = Geometry::angle_rad(start, end);
    }
    else if(Lb < La && Lb < Lc){
        cv::Point start = points[2] + b/2;
        cv::Point end = points[1];
        angle = Geometry::angle_rad(start, end);
    }
    else if(Lc < La && Lc < Lb){
        cv::Point start = points[2] + c/2;
        cv::Point end = points[0];
        angle = Geometry::angle_rad(start, end);
    }
    else{
        std::cout << "can not find orientation of triangle" << std::endl;
        deg = 0;
    }
    angle+=M_PI;
    // center of the wheel of the robot
    center_wheel.x = (int)(center.x + 50*ImageProcessing::Settings::PIXEL_SCALE*cos(angle));
    center_wheel.y = (int)(center.y + 50*ImageProcessing::Settings::PIXEL_SCALE*sin(angle));

    this->radius = max(La,Lb);
    this->radius = max(this->radius,Lc);
    this->radius = this->radius/2;
    
    


}

double robotPerimeter(std::vector<cv::Point> corner){
    double first =abs(cv::norm(corner[0] - corner[1]));
    double second =abs(cv::norm(corner[1] - corner[2]));
    double third =abs(cv::norm(corner[2] - corner[0]));
    return first+second+third;
}

bool Robot::findRobot(const cv::Mat &img){

    // Convert color space from BGR to HSV
    cv::Mat hsv_img;
    cv::cvtColor(img, hsv_img, cv::COLOR_BGR2HSV);
    //cv::imshow("blue mask 1", hsv_img);
    //cv::waitKey(0);
    // Preparing the kernel matrix
    cv::Mat kernel = cv::getStructuringElement(
                                               cv::MORPH_RECT, cv::Size((1 * 2) + 1, (1 * 2) + 1));
    
    // Definining contour containers and hierarchy to find the right contours
    cv::Mat contours_img;
    std::vector<std::vector<cv::Point>> contours, contours_approx;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<cv::Point> approx_curve, corners;
    
    // Find red regions: h values around 0 (positive and negative angle: [0,15] U [160,179])
    cv::Mat blue_mask;
    cv::inRange(hsv_img, cv::Scalar(75, 50, 50), cv::Scalar(130, 255, 255), blue_mask);
    
    // Filter (applying dilation, blurring, dilation and erosion) the image
    kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size((2 * 2) + 2, (2 * 2) + 2));
    // Filter (applying an erosion and dilation) the image
    //cv::GaussianBlur(blue_mask, blue_mask, cv::Size(7, 7), 6, 6);
    //cv::imshow("blue mask 1", blue_mask);
    //cv::waitKey(0);
    cv::erode(blue_mask, blue_mask, kernel);
    cv::dilate(blue_mask, blue_mask, kernel);
    cv::erode(blue_mask, blue_mask, kernel);


    //cv::imshow("blue mask", blue_mask);
    //cv::waitKey(0);
    // Process red mask
    
    contours_img = img.clone();
    cv::findContours(blue_mask, contours, hierarchy, cv::RETR_LIST,
                     cv::CHAIN_APPROX_SIMPLE);
    drawContours(contours_img, contours, -1, cv::Scalar(40, 190, 40), 1,
                 cv::LINE_AA);
    int perimeter =0;
    for (int i = 0; i < contours.size(); ++i)
    {
        approxPolyDP(contours[i], approx_curve, epsilon_approx, true);
        drawContours(contours_img, contours_approx, -1, cv::Scalar(0, 170, 220), 3, cv::LINE_AA);
        contours_approx = {approx_curve};
        if (approx_curve.size() ==3 && robotPerimeter(approx_curve)>perimeter)
        {
            Triangle triangle = Triangle();
            perimeter = robotPerimeter(approx_curve);
            triangle.setCorners(approx_curve);

           // std::cout << "Triangle : " << triangle.getCorners() << std::endl;
            drawContours(contours_img, contours_approx, -1, cv::Scalar(0, 170, 220), 3, cv::LINE_AA);
            
            triangle.points = triangle.getCorners();
            
            if(!triangle.points.empty()){
            update(triangle.points);
                
            }
        }
    }
    circle(contours_img, this->center, 3, Scalar(0, 255, 0), -2, 8, 0);
    circle(contours_img, this->center_wheel, 3, Scalar(255, 255, 0), -2, 8, 0);

    imshow("contour robot", contours_img);
    waitKey(0);
    if(perimeter>0){
        return true;
    }else{
        return false;
    }
}

cv::Point Robot::getPosition(){
    
    //both in robot
    return center_wheel - initialPosition;
}

cv::Point Robot::getPosition2d(const cv::Point &ref,const cv::Point &error){
    
    //ref is the arena start point and error the distance to the white circle
    std::pair<double, double> result = Geometry::convertPixelToMillimeterInMapPlane(getPosition(), ref-error);
    
    return cv::Point2d(result.first,result.second);
}

void Robot::move(const cv::Point &location, const double &angle){
    
    if(angle == NAN)
        return;
    if(points.empty())
        return;

    
    cv::Point d = location - center;
    
    cv::Point d_0 = location - points[0];
    cv::Point d_1 = location - points[1];
    cv::Point d_2 = location - points[2];
    
    cv::Matx23d rot_mat = getRotationMatrix2D( center,this->angle-angle, 1 );
    
    std::vector<cv::Point> pointz;
    
    if(rot_mat.val[0] != NAN)
        cv::transform(points, pointz, rot_mat);
    
    std::cout <<(pointz[0]) << std::endl;
    
    if(pointz[0].x > 1000)
        return;
    if(pointz[0].x < 0)
        return;
    
    pointz[0] += d;
    pointz[1] += d;
    pointz[2] += d;
    
    if(pointz[0].x >= 0 && pointz[1].x && pointz[1].x &&
       pointz[0].y >= 0 && pointz[1].y && pointz[1].y &&
       angle != NAN)
        points = pointz;
    
    setCorners(points);
    
    update(points);
    
}

