//
//  PathEFinder.cpp
//  LAR_1_0
//
//  Created by Air Marvin on 11.12.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#include "../Headers/PathEFinder.hpp"

using namespace PathE2D;

PathEFinder::PathEFinder(PathECoordinates path_coordinates_i,Map* map_i){}

PathEFinder::PathEFinder(Position start_point, Position end_point, double curvature, Map *map_i){}



PathEFinder::PathEFinder(){}

PathEFinder::CollisionDetector::CollisionDetector() {
    
}

PathEFinder::CollisionDetector::~CollisionDetector() {
    
}



double distanceEuclidean(cv::Point2d initial_point, cv::Point2d final_point) {
    double res;
    res = sqrt(pow(initial_point.x - final_point.x, 2.0) +
               pow(initial_point.y - final_point.y, 2.0));
    return res;
}

double arcLength(cv::Point2d initial_point, cv::Point2d final_point, double curvature, double old_distance) {
    double d = distanceEuclidean(initial_point, final_point);
    curvature = abs(curvature);
    double res = 0;
    double sin_angle = (d * curvature) / 2.0;
    double asin_v = asin(sin_angle);
    double numerator = 2.0 * asin_v;
    res = numerator / curvature;
    double perimeter = (1.0 / curvature) * 2.0 * M_PI;
    // useful when then distance between start point and end point i greater than half perimeter
    if (res < old_distance) {
        res = perimeter - res;
        return res;
    }
    
    return res;
}

double distance(Position initial_point, Position final_point, double max_curvature, double old_distance) {
    double res = 0;
    if (initial_point.getOrientation() == final_point.getOrientation()) {
        res = distanceEuclidean(initial_point.getCoordinates(), final_point.getCoordinates());
    } else {
        res = arcLength(initial_point.getCoordinates(), final_point.getCoordinates(), max_curvature, old_distance);
        
    }
    return res;
}

// return a vector of intermediate points in of the line a given distance
std::vector<Point2d>* findIntermediatePoints(Line &line, double gap, double &max_curvature) {
    std::vector<Point2d>* tmp_intermediate_points = new std::vector<cv::Point2d>();
    Position next_position = line.getStartPoint();
    tmp_intermediate_points->push_back(next_position.getCoordinates());
    double points_distance = distance(next_position, line.getStartPoint(), line.getCurvature(), -1);
    while (points_distance < (line.getLength() - gap)) {
        // find the next point at given distance
        next_position = line.findPointDistance(line.getCurvature(), next_position, gap);
        tmp_intermediate_points->push_back(next_position.getCoordinates());
        points_distance = distance(next_position, line.getStartPoint(), line.getCurvature(), points_distance);
        
    }
    tmp_intermediate_points->push_back(line.getEndPoint().getCoordinates());
    return tmp_intermediate_points;
}

//bool collision(cv::Point2d point, Map *map) {
//
//    bool isCollision = map->getCell(point)->isObstacle();
//
//    if (isCollision) {
//        std::cout << "collision" << std::endl;
//    }
//    return isCollision;
//}

std::pair<bool,Cell*> collision(cv::Point2d point, Map *map) {
    
    Cell * cell = map->getCell(point);
    
    std::vector<Cell*> subcells = cell->getAllSubcells();
    
    
    if(subcells.empty()){
        if(cell->isObstacle(point)){
            //std::cout << "collision" << std::endl;
            return std::pair<bool,Cell*>(true,cell);
        }

    }
    
    
    for(int i=0;i<subcells.size();i++){
        //if(subcells[i]->contains_object() &&  subcells[i]->getSubcells().empty() && !subcells[i]->isRescue())
        if(subcells[i]->isObstacle(point) && subcells[i]->getSubcells().empty())
            if(subcells[i]->pointInside(point)){
                //std::cout << "collision" << std::endl;
                return std::pair<bool,Cell*>(true,subcells[i]);
            }
    }
    
    //bool isCollision = cell->isObstacle(point);
    
    return std::pair<bool,Cell*>(false,cell);
}


std::pair<bool,Cell*> PathEFinder::CollisionDetector::detectCollision(std::vector<Line> &lines_i, Map *map, double max_curvature) {

    int diameter = 5; //TODO fix the distance between points
    for (int i = 0; i < lines_i.size(); i++) {
        
        //find the intermediate point and detect if it is a collision
        std::vector<cv::Point2d>* intermediate_points;
        intermediate_points = findIntermediatePoints(lines_i[i], diameter, max_curvature);
        
        for (int j = 0; j < intermediate_points->size(); j++) {
            
            std::pair<bool,Cell*> coll = collision((*intermediate_points)[j], map);
            
            if (coll.first) {
                //std::cout << "collided at cell with center " << coll.second->center() << std::endl;
                return std::pair<bool,Cell*>(true,coll.second);
            }
        }
        lines_i[i].setIntermediatePoints(*intermediate_points);
    }
    return std::pair<bool,Cell*>(false,nullptr);;
}
