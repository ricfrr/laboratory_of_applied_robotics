//
//  Visualizer.hpp
//  LAR_1_0
//
//  Created by Air Marvin on 22.11.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#ifndef Visualizer_hpp
#define Visualizer_hpp

#include "../Headers/RoboticMapping.hpp"
#include "../Headers/PathPlanning.hpp"
#include "../Headers/Robot.hpp"


#include <stdio.h>

/**
 \brief class that visualizes the steps taken by robotic mapping and path planning
 */
class Visualizer {
    
public:
    Visualizer();
    Visualizer(Map &map);
    Visualizer(Map &map, Path* &path);
    ~Visualizer();
    
    ///override the map object to visualize
    void assign_map(Map &map);
    ///override path object to visualize
    void assign_path(Path *&path);
    ///create an image based on the information
    void visualize();
    ///simulate the robots way along the path
    void simulate();
    
    ///hightlight nearest neighbours
    void colorNearestNeighbours(cv::Mat &result);
    
    void placePoint(cv::Point point,cv::Mat &result ,cv::Scalar color = cv::Scalar(0,0,255));
    
private:
    Map * p_map;
    Path * p_path;
    
    const std::string windowtitle = "window";
    
    void play();
    
    cv::Mat print_arena(cv::Mat &result);
    cv::Mat print_grid(cv::Mat &result);
    cv::Mat print_shapes(cv::Mat &result);
    cv::Mat print_path(cv::Mat &result);
    
    cv::Mat merge(cv::Mat &input, cv::Mat &overlay, cv::Scalar color);
    void draw_cell(cv::Mat &result, Cell* &cell);
    
    void color_pixels_from(Cell &cell, cv::Mat &inImage, Vec3b color = Vec3b(100,100,100));
    
    void colorAllCellsContainingObjects(std::vector<Cell*> &cells, cv::Mat &inImg, Vec3b color = Vec3b(200,200,200));
    
    void colorAllCellsContainingObstacle(std::vector<Cell*> &cells, cv::Mat &inImg, Vec3b color = Vec3b(200,200,200));
    
    void drawVector(Position pos, cv::Mat &result);
    
    Robot * car;
};

#endif /* Visualizer_hpp */
