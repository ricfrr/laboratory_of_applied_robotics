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
        bool r = *a < *b;
        std::cout << "result comp = " << r << std::endl;
        return *a < *b;
    }
} customLess;

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
    
    std::cout << "planning mission one" << std::endl;
    
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
    
    Path2D::Path* path = new Path2D::Path(Path2D::Position(cv::Point2d(60,60),or1),
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
    
    
        
        
}


