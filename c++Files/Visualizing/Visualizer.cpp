//
//  Visualizer.cpp
//  LAR_1_0
//
//  Created by Air Marvin on 22.11.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#include "Visualizer.hpp"

Visualizer::Visualizer(){}

Visualizer::Visualizer(Map &map){
    assign_map(map);
}

Visualizer::Visualizer(Map &map, Path* &path){
    assign_map(map);
    assign_path(path);
}

Visualizer::~Visualizer(){}

void Visualizer::assign_map(Map &map){
    this->p_map = &map;
}

void Visualizer::assign_path(Path *&path){
    this->p_path = path;
}

void Visualizer::visualize(){
    
    cv::Mat result;
    
    cv::Mat arena = print_arena();
    cv::Mat grid = print_grid();
    cv::Mat shapes = print_shapes();
    cv::Mat path = print_path();
    
    cv::imshow("arena", arena);
    cv::imshow("grid", grid);
    //cv::imshow("shapes", shapes);
    //cv::imshow("path", path);
    //cv::waitKey(0);
    
    result = merge(arena, grid, cv::Scalar(0,0,0));
    
    cv::imshow("result", result);
    cv::waitKey(0);
    
}

cv::Mat Visualizer::print_arena(){
    return cv::Mat();
}
cv::Mat Visualizer::print_grid(){
    return cv::Mat();
}
cv::Mat Visualizer::print_shapes(){
    return cv::Mat();
}
cv::Mat Visualizer::print_path(){
    return cv::Mat();
}

cv::Mat  Visualizer::merge(cv::Mat &input, cv::Mat &overlay, cv::Scalar color){
    
    //filtered cv::Scalar values are the ones that will be overlayed in the original image
    cv::Mat mask_inv, filtered(input.rows, input.cols, CV_8UC3, color);
    
    //only get the shapes in black color - so we need to invert because only shapes are white and the rest is black
    cv::bitwise_not(overlay, mask_inv); // generate binary mask with inverted pixels w.r.t. green mask -> black numbers are part of this mask
    
    input.copyTo(filtered, mask_inv);   // create copy of image without green shapes
    
    return filtered;
}
