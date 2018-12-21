//
//  MissionPlanning.cpp
//  LAR_1_0
//
//  Created by Air Marvin on 14.12.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#include "MissionPlanning.hpp"

struct {
    bool operator()(People* a, People* b) const
    {
        return *a < *b;
    }
} customLess;


double dinstance (cv::Point &p, const cv::Point &o) {
    cv::Point point = p-o;
    return std::sqrt(std::pow(point.x,2) + std::pow(point.y,2));
}

MissionPlanning::MissionPlanning(){
    this->map_p = new Map;
}

MissionPlanning::MissionPlanning(Map *map){
    this->map_p = map;
}

MissionPlanning::~MissionPlanning(){}

bool comparePeople(People p1, People p2){
    return p1.name < p2.name;
}

void MissionPlanning::plan_mission_one(){
    
    std::cout << "\nplanning mission one \n" << std::endl;
    
    Digit_Recognition::PeopleStorage peops = map_p->getPeople();
    std::vector<People> people_v = peops.circles;
    std::vector<People*> people_s;
    
    for(std::vector<People>::iterator it=people_v.begin();it != people_v.end(); it++)
        people_s.push_back(&*it);

    std::sort(people_s.begin(), people_s.end(), customLess);
    
    if(people_s.empty())
    {
        std::cout << "no people detected.. cannot plan mission" << std::endl;
        return;
    }
    
    double or1 = 0*M_PI;
    double or2 = 1.5*M_PI;
    std::pair<cv::Point,double> exit = map_p->getExitPoint().getEntryPoint();
    Path2D::Position finalPos =  Path2D::Position(exit.first,exit.second);finalPos.orientation_locked = true;
    
    Path2D::Path* path = new Path2D::Path(Path2D::Position(map_p->robo->center,map_p->robo->angle),
                                          Path2D::Position(people_s[0]->center,or2),0.05,map_p);
    
    for(int i = 1; i<people_s.size();i++){
        Path2D::Path* p_path = new Path2D::Path(path->end_point,
                                        Path2D::Position(people_s[i]->center,or2),0.05,map_p);
        path = new Path2D::Path(*path,*p_path);
    }
    
    Path2D::Path* p_path = new Path2D::Path(path->end_point,
                                            finalPos,0.05,map_p);
    path = new Path2D::Path(*path,*p_path);
    
    Visualizer v(*map_p,path);
    v.visualize();
    v.simulate();
    
    std::cout << "---- DONE ----" << std::endl;
    
}


void MissionPlanning::plan_mission_two(){
    
    std::cout << "planning mission two" << std::endl;
    
    Digit_Recognition::PeopleStorage peops = map_p->getPeople();
    std::vector<People> people_v = peops.circles;
    std::vector<People*> people_s;
    
    cv::Point2d start(60,60);
    
    for(std::vector<People>::iterator it=people_v.begin();it != people_v.end(); it++)
        people_s.push_back(&*it);
    
    std::sort(people_s.begin(), people_s.end(), [start](People* a, People* b) {
        
        return dinstance(a->center, start) < dinstance(b->center,start);
    });
    
    if(people_s.empty())
    {
        std::cout << "no people detected.. cannot plan mission" << std::endl;
        return;
    }
    
    double or1 = 0*M_PI;
    double or2 = 1.5*M_PI;
    std::pair<cv::Point,double> exit = map_p->getExitPoint().getEntryPoint();
    Path2D::Position finalPos =  Path2D::Position(exit.first,exit.second);finalPos.orientation_locked = true;
    
    Path2D::Path* path = new Path2D::Path(Path2D::Position(map_p->robo->center,map_p->robo->angle),
                                          finalPos,0.05,map_p);
    
    if(path->lines.empty()){
        
        path = new Path2D::Path(Path2D::Position(cv::Point2d(60,60),or1),
                                              Path2D::Position(people_s[0]->center,or2),0.05,map_p);
        
        for(int i = 1; i<people_s.size();i++){
            
            Path2D::Path* b_path = new Path2D::Path(path->end_point,
                                                    finalPos,0.05,map_p);
            
            if(!b_path->lines.empty()){
                path = new Path2D::Path(*path,*b_path);
                break;
            }
            
            Path2D::Path* p_path = new Path2D::Path(path->end_point,
                                                    Path2D::Position(people_s[i]->center,or2),0.05,map_p);
            
            if(!p_path->lines.empty())
                path = new Path2D::Path(*path,*p_path);
        }
        
        Path2D::Path* p_path = new Path2D::Path(path->end_point,
                                                finalPos,0.05,map_p);
        path = new Path2D::Path(*path,*p_path);
    }
    
    Visualizer v(*map_p,path);
    v.visualize();
    v.simulate();
}


