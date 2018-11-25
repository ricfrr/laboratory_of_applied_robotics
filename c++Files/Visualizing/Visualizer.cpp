//
//  Visualizer.cpp
//  LAR_1_0
//
//  Created by Air Marvin on 22.11.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#include "Visualizer.hpp"

Visualizer::Visualizer(){
    this->car = new Robot;
}

Visualizer::Visualizer(Map &map){
    assign_map(map);
    this->car = new Robot;
}

Visualizer::Visualizer(Map &map, Path* &path){
    assign_map(map);
    assign_path(path);
    this->car = new Robot;
}

Visualizer::~Visualizer(){}

void Visualizer::assign_map(Map &map){
    this->p_map = &map;
}

void Visualizer::assign_path(Path *&path){
    this->p_path = path;
}

void Visualizer::play(){
    
    if(this->p_map == nullptr)
        std::runtime_error("che cazzo");
    
    int height = 0;
    int width = 0;
    
    this->p_map->getPixelDimensions(width, height);
    
    cv::Mat start(height,width,CV_8UC1,cv::Scalar(255,255,255));
    cv::Mat result;
    cv::cvtColor(start, result, CV_GRAY2RGB);
    
    print_arena(result);
    print_grid(result);
    print_shapes(result);
    print_path(result);
    
    cv::imshow(windowtitle, result);
}

void Visualizer::visualize(){
    
    play();
    cv::waitKey(0);
    
    
}

cv::Mat Visualizer::print_arena(cv::Mat &result){
    
    std::vector<Cell *> cells;
    p_map->getArenaCells(cells);
    
    for(int c = 0;c<cells.size();c++){
            
        
        int w_startingPoint   = cells[c]->getTopLeft().x;
        int w_endingPoint     = cells[c]->getTopRight().x;
        
        int h_startingPoint     = cells[c]->getTopLeft().y;
        int h_endingPoint     = cells[c]->getBottomLeft().y;
        
        int diff_x = w_endingPoint - w_startingPoint;
        int diff_y = h_endingPoint - h_startingPoint;
        
        for(int y = 0;y<diff_y;y++){
            
            for(int x = 0;x<diff_x;x++){
                
                if (cells[c]->isBorder()){
                    result.at<Vec3b>(Point(w_startingPoint + x,h_startingPoint + y)) = Vec3b(0,0,0);
                }
            }
            
        }
    }
    
    return result;
}
cv::Mat Visualizer::print_grid(cv::Mat &result){
    
    std::vector<std::vector<Cell*>> grid;
    this->p_map->getGrid(grid);
    
    for(int i=0;i<grid.size();i++){
        for(int j=0;j<grid[i].size();j++){
            draw_cell(result, grid[i][j]);
            
            std::vector<Cell*> subcells = grid[i][j]->getAllSubcells();
            
            for(int k=0;k<subcells.size();k++){
                draw_cell(result, subcells[k]);
            }
        }
    }
    return result;
}

void Visualizer::draw_cell(cv::Mat &result, Cell* &cell){
    cv::line(result, cell->getTopLeft(), cell->getBottomLeft(), cv::Scalar(50,50,50));
    cv::line(result, cell->getTopLeft(), cell->getTopRight(), cv::Scalar(50,50,50));
    cv::line(result, cell->getBottomLeft(), cell->getBottomRight(), cv::Scalar(50,50,50));
    cv::line(result, cell->getTopRight(), cell->getBottomRight(), cv::Scalar(50,50,50));
}

cv::Mat Visualizer::print_shapes(cv::Mat &result){
    
    //print obstacles
    Obstacle obstacle = p_map->getObstacles();
    
    //print triangles
    std::vector<Triangle> tri = obstacle.getTriangles();
    for (int i=0;i<tri.size();i++){
        cv::fillConvexPoly(result, tri[i].getCorners(), cv::Scalar(100,100,100));
    }
    
    //print Squares
    std::vector<Square> sqr = obstacle.getSquares();
    for (int i=0;i<sqr.size();i++){
        cv::fillConvexPoly(result, sqr[i].getCorners(), cv::Scalar(200,200,200));
    }
    
    //print Hexagons
    std::vector<Hexagon> hex = obstacle.getHexagons();
    for (int i=0;i<hex.size();i++){
        cv::fillConvexPoly(result, hex[i].getCorners(), cv::Scalar(150,150,150));
    }
    
    //print Pentagons
    std::vector<Pentagon> pen = obstacle.getPentagons();
    for (int i=0;i<pen.size();i++){
        cv::fillConvexPoly(result, pen[i].getCorners(), cv::Scalar(50,50,50));
    }
    
    //print exit point
    ExitPoint ex = p_map->getExitPoint();
    cv::rectangle(result, ex.getTopLeft(), ex.getBottomRight(), cv::Scalar(0,191,255), -1,LINE_8,0);
    
    //print people
    PeopleStorage p = p_map->getPeople();
    
    //print People
    std::vector<People> c = p.circles;
    for (int i=0;i<c.size();i++){
        
        cv::circle(result, c[i].getCenter(), c[i].getRadius()/2, cv::Scalar(30,90,180), -1,LINE_8,0);

        cv::putText(result, std::to_string(c[i].name), c[i].getCenter(),
                FONT_HERSHEY_COMPLEX_SMALL, 1.5, cvScalar(255,255,255), 1, CV_AA);
    }
    
    //print Car
    cv::circle(result, car->getCenter(), car->getRadius()/2, car->color, -1,LINE_8,0);
    
    
    return result;
}

cv::Mat Visualizer::print_path(cv::Mat &result){
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

void Visualizer::simulate(){
    
    //testing
    Position p1 = Position(cv::Point2d(this->car->getCenter()), 0);
    Position p2 = Position(cv::Point2d(300,300), 0);
    this->p_path = new Path(p1, p2,0);
    
    for(int i=0;i<40;i++){
        this->car->setCenter(cv::Point(i*10,i*10));
        play();
        cv::waitKey(33);
    }
    visualize();
}
