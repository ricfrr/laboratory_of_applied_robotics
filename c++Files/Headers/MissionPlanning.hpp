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
    
    void plan_mission_one();
    void plan_mission_two();
    
};
#endif /* MissionPlanning_hpp */
