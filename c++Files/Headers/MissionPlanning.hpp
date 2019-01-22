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
#include "PathPlanning.hpp"

#include "../Visualizing/Visualizer.hpp"


class MissionPlanning {

public:
    MissionPlanning();
    MissionPlanning(Map *map);
    ~MissionPlanning();
    
    Map* map_p;
    Path2D::Path* path_p;

    std::vector<Line*> lines;

    void plan_mission_one();
    void plan_mission_two();

private:

    void initializePeopleDimensionDinstance();
    double people_dimension=0;
    double curvature = 0.1; //  TODO 0.1 should be the real value
    
private:
    bool isExitReached(std::vector<Path2D::Position*> &point_of_interests);
    
    Path2D::Path* findOptimalPath(Position * start_point, std::vector<Path2D::Position*> &point_of_interests);
    
    Path2D::Path * passNearAnotherPerson(Path2D::Path * path, std::vector<Position*> &point_of_interest, int index, std::vector<int> &intermediate_person_index);
    
};
#endif /* MissionPlanning_hpp */
