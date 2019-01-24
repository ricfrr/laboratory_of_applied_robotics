//
//  MissionPlanning.hpp
//  LAR_1_0
//
//  Created by Air Marvin on 14.12.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#ifndef MissionPlanning_hpp
#define MissionPlanning_hpp

#include <stdio.h>
#include "RoboticMapping.hpp"
#include "PathEPlanning.hpp"

#include "../Visualizing/Visualizer.hpp"


class MissionPlanning {

public:
    MissionPlanning();
    MissionPlanning(Map *map);
    ~MissionPlanning();
    
    Map* map_p;
    PathE2D::PathE* path_p;


        void plan_mission_one();
    void plan_mission_two();
    void plan_mission_two_fast();


private:

    void initializePeopleDimensionDinstance();
    double people_dimension=0;
    double curvature = 1.0/41.4; //  TODO 0.1 should be the real value
    
private:
    bool isExitReached(std::vector<PathE2D::Position*> &point_of_interests);
    
    PathE2D::PathE* findOptimalPathE(Position * start_point, std::vector<PathE2D::Position*> &point_of_interests);
    PathE2D::PathE *
    findOptimalFastPathE(Position *start_point, std::vector<PathE2D::Position *> &point_of_interests);
    
    PathE2D::PathE * passNearAnotherPerson(PathE2D::PathE * path, std::vector<Position*> &point_of_interest, int index, std::vector<int> &intermediate_person_index);
    
};
#endif /* MissionPlanning_hpp */
