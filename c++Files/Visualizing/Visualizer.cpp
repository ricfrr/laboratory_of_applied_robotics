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
    
    //testing
    colorNearestNeighbours(result);
    
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
        
        
        if(cells[c]->getSubcells().empty()){
            color_pixels_from(*cells[c], result, Vec3b(0,0,0));
            continue;
        }
        
        std::vector<Cell*> subcells = cells[c]->getAllSubcells();
        
        for(int i=0;i<subcells.size();i++){
            if(subcells[i]->contains_object())
                color_pixels_from(*subcells[i], result, Vec3b(0,0,0));
        }
        
        
    }
    
    return result;
}

void Visualizer::color_pixels_from(Cell &cell, cv::Mat &inImage, Vec3b color){
    
    int w_startingPoint   = cell.getTopLeft().x;
    int w_endingPoint     = cell.getTopRight().x;
    
    int h_startingPoint     = cell.getTopLeft().y;
    int h_endingPoint     = cell.getBottomLeft().y;
    
    int diff_x = w_endingPoint - w_startingPoint;
    int diff_y = h_endingPoint - h_startingPoint;
    
    for(int y = 0;y<diff_y;y++){
        
        for(int x = 0;x<diff_x;x++){
            
            if (cell.contains_object()){
                inImage.at<Vec3b>(Point(w_startingPoint + x,h_startingPoint + y)) = color;
            }
        }
        
    }
    
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
                
//                if(subcells[k]->contains_object() && !subcells[k]->getSubcells().empty() )
//                    color_pixels_from(*subcells[k], result, Vec3b(10,10,255));
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
    
    //print triangles;
    std::vector<Triangle*> tri = obstacle.getTriangles();
    for (int i=0;i<tri.size();i++){
        std::vector<Cell*> cs = tri[i]->getCell();
        colorAllCellsContainingObstacle(cs, result);
        std::vector<Cell*>tricells = tri[i]->getCell();
        //colorAllCellsContainingObjects(tricells, result, Vec3b(200,200,200));
        
        cv::fillConvexPoly(result, tri[i]->getCorners(), cv::Scalar(100,100,100));
    }
    
    //print Squares
    std::vector<Square*> sqr = obstacle.getSquares();
    for (int i=0;i<sqr.size();i++){
        std::vector<Cell*> cs = sqr[i]->getCell();
        colorAllCellsContainingObstacle(cs, result);
        cv::fillConvexPoly(result, sqr[i]->getCorners(), cv::Scalar(100,100,100));
    }
    
    //print Hexagons
    std::vector<Hexagon*> hex = obstacle.getHexagons();
    for (int i=0;i<hex.size();i++){
        std::vector<Cell*> cs = hex[i]->getCell();
        colorAllCellsContainingObstacle(cs, result);
        cv::fillConvexPoly(result, hex[i]->getCorners(), cv::Scalar(100,100,100));
    }
    
    //print Pentagons
    std::vector<Pentagon*> pen = obstacle.getPentagons();
    for (int i=0;i<pen.size();i++){
        
        std::vector<Cell*> cs = pen[i]->getCell();
        colorAllCellsContainingObstacle(cs, result);
        cv::fillConvexPoly(result, pen[i]->getCorners(), cv::Scalar(100,100,100));
    }
    
    //print exit point
    ExitPoint ex = p_map->getExitPoint();
    std::vector<Cell*> cells = ex.getCell();
    colorAllCellsContainingObjects(cells, result,Vec3b(0,191,200));
    
    cv::rectangle(result, ex.getTopLeft(), ex.getBottomRight(), cv::Scalar(0,191,255), -1,LINE_8,0);
    
    //print people
    ImageProcessing::Digit_Recognition::PeopleStorage p = p_map->getPeople();
    
    //print People
    std::vector<People> c = p.circles;
    for (int i=0;i<c.size();i++){
        
        cv::circle(result, c[i].getCenter(), c[i].getRadius(), cv::Scalar(30,90,180), -1,LINE_8,0);

        cv::putText(result, std::to_string(c[i].name), c[i].getCenter(),
                FONT_HERSHEY_COMPLEX_SMALL, 1.5, cvScalar(255,255,255), 1, CV_AA);
    }
    
    //print Car
    cv::circle(result, car->getCenter(), car->getRadius(), car->color, -1,LINE_8,0);
    
    
    return result;
}

void Visualizer::colorAllCellsContainingObjects(std::vector<Cell*> &cells, cv::Mat &inImg, Vec3b color){
    
    for(int c = 0;c<cells.size();c++){
        
        
        if(cells[c]->getSubcells().empty()){
            color_pixels_from(*cells[c], inImg, color);
            continue;
        }
        
        std::vector<Cell*> subcells = cells[c]->getAllSubcells();
        
        for(int i=0;i<subcells.size();i++){
            if(subcells[i]->contains_object() && subcells[i]->getSubcells().empty())
                color_pixels_from(*subcells[i], inImg, color);
        }
        
        
    }
    
}

void Visualizer::colorAllCellsContainingObstacle(std::vector<Cell*> &cells, cv::Mat &inImg, Vec3b color){
    
    for(int c = 0;c<cells.size();c++){
        
        
        if(cells[c]->getSubcells().empty()){
            color_pixels_from(*cells[c], inImg, color);
            continue;
        }
        
        std::vector<Cell*> subcells = cells[c]->getAllSubcells();
        
        for(int i=0;i<subcells.size();i++){
            if(subcells[i]->isObstacle() && subcells[i]->getSubcells().empty())
                color_pixels_from(*subcells[i], inImg, color);
            else if(subcells[i]->getSubcells().empty())
                color_pixels_from(*subcells[i], inImg, Vec3b(255,0,255));
        }
        
        
    }
    
}

cv::Mat Visualizer::print_path(cv::Mat &result){
    
    std::vector<Line> lines = p_path->getLines();
    
    
    for(int i=0;i<lines.size();i++){
//        cv::line(
//                 result,
//                 cv::Point(lines[i].getStartPoint().getCoordinates().x,lines[i].getStartPoint().getCoordinates().y),
//                 cv::Point(lines[i].getEndPoint().getCoordinates().x,lines[i].getEndPoint().getCoordinates().y)   , cv::Scalar(255,0,0),2);
        std::vector<cv::Point2d> ps = lines[i].getIntermediatePoints();
        for(int j=0;j<ps.size();j++){
            Cell *cell = p_map->getCell(ps[j]);
            if(cell->contains_object())
                cv::circle(result, ps[j], 1, cv::Scalar(255,255,50),-1);
            else{
                cv::circle(result, ps[j], 1, cv::Scalar(255,100,0),-1);
            }
        }
    }
    
    drawVector(p_path->start_point, result);
    drawVector(p_path->end_point, result);
    
    return result;
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
    
    std::vector<Line> lines = p_path->getLines();
    std::vector<cv::Point> points;
    for(int i=0;i<lines.size();i++){
        std::vector<cv::Point2d> ipoints = lines[i].getIntermediatePoints();
        for(int j=0;j<ipoints.size();j++)
            points.push_back(cv::Point(ipoints[j]));
    }
    
    bool stay = true;
    while(cvWaitKey(0) != '\33' && stay){
    for(int i=0;i<points.size();i++){
        this->car->setCenter(points[i]);
        play();
        cv::waitKey(25);
    }
    }
    //visualize();
}

void Visualizer::colorNearestNeighbours(cv::Mat &result){
    
    Obstacle ob = p_map->getObstacles();
    
    std::vector<Square*> sq = ob.getSquares();
    std::vector<Pentagon*> pg = ob.getPentagons();
    std::vector<Hexagon*> hx = ob.getHexagons();
    std::vector<Triangle*> tr = ob.getTriangles();
    
    std::vector<Cell*> cells;
    
    for(int i=0;i<sq.size();i++){
        std::vector<Cell*> c = sq[i]->getCell();
        for(int j=0;j<c.size();j++)
            cells.push_back(c[j]);
    }
    for(int i=0;i<pg.size();i++){
        std::vector<Cell*> c = pg[i]->getCell();
        for(int j=0;j<c.size();j++)
            cells.push_back(c[j]);
    }
    for(int i=0;i<hx.size();i++){
        std::vector<Cell*> c = hx[i]->getCell();
        for(int j=0;j<c.size();j++)
            cells.push_back(c[j]);
    }
    for(int i=0;i<tr.size();i++){
        std::vector<Cell*> c = tr[i]->getCell();
        for(int j=0;j<c.size();j++)
            cells.push_back(c[j]);
    }
    
    for(int i = 0;i<cells.size(); i++)
        if(cells[i]->getState() == MIXED){
            std::vector<std::vector<cv::Point>> n = p_map->getEmptyNearestNeighborsPoints(cells[i]);
            for(int j=0; j<n.size();j++)
                for(int k=0;k<n[j].size();k++)
                    placePoint(n[j][k], result, cv::Scalar(0,255,0));
        }
    
//    for(int i = 0;i<cells.size(); i++){
//        if(cells[i]->getState() == MIXED){
//            std::vector<std::vector<Cell*>> n = p_map->getNearestNeighbors(cells[i]);
//            for(int j=0; j<n.size();j++)
//                for(int k=0;k<n[j].size();k++){
//                    //color_pixels_from(*n[j][k], result,Vec3b(0,0,0));
//                    if(n[j][k]->getState() == EMPTY)
//                        placePoint(n[j][k]->center(), result, cv::Scalar(0,255,0));
//                    else
//                        placePoint(n[j][k]->center(), result);
//                }
//        }
//    }
}

void Visualizer::placePoint(cv::Point point,cv::Mat &result, cv::Scalar color){
    cv::circle(result, point, 1, color,-1);
}

void Visualizer::drawVector(Position pos, cv::Mat &result){
        
    double length = 30;
    double x = pos.getCoordinates().x + cos(pos.orientation)*length;
    double y = pos.getCoordinates().y + sin(pos.orientation)*length;
        
    cv::Point end = cv::Point(x,y);
    cv::line(result, pos.getCoordinates(), end, cv::Scalar(0,0,255),3);
    cv::circle(result, end, 6, cv::Scalar(0,0,255),-1);
    
        
        
    
}
