//
//  PathEFinder.hpp
//  LAR_1_0
//
//  Created by Air Marvin on 11.12.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#ifndef PathEFinder_hpp
#define PathEFinder_hpp

#include <opencv2/core/core.hpp>
#include <limits>
#include "PathECoordinates.hpp"
#include "Line.hpp"
#include "CircularLine.hpp"
#include "StraightLine.hpp"
#include "Map.hpp"


using namespace cv;

namespace PathE2D {
    
    using namespace Element;

/**
 \brief abstract class for finding a path between two Positions
 \discussion This class is a basis for more defined classes based on this class structure. For example a DubinsPathEFinder class could be used to find a path using DubinCurves, or an SplinePathEFinder could be made to find a SplineCurve. Anyway this class does not just contain the basic structure of future specialized implementations but also usefull tools every PathEFinding algorithm needs, such as a collision detection class nested inside. Given a map object this class can be used to see if the picked path is colliding with obstacles in the map.
 */
class PathEFinder {
  
public:
    /**
     \brief constructor of the PathEFinder class
     * @param path_coordinates_i coordinates of the path that we want to find
     */
    PathEFinder(PathECoordinates path_coordinates_i,Map* map_i);
    PathEFinder(Position start_point, Position end_point, double curvature, Map *map_i);
    /**
     * \brief destructor of the PathEFinder class
     */
    PathEFinder();
    /**
     * \brief function that find the dubins shortest path and return a vector of line
     * @arg alternative_Points a point set for every obsticle that was found along the way
     * @return vector of lines which describes the path
     */
    virtual std::vector<Line> shortestPathE(std::vector<cv::Point> &alternative_Points) = 0;
    
    PathECoordinates path_coordinates = PathECoordinates(Position(), Position(), 0);
    
    double length;
    Map* map;
    
    /**
     * \brief detector of collision between obstacles and path
     */
    class CollisionDetector {
        
    public:
        /**
         * \brief constructor of CollisionDetector_hpp class
         */
        CollisionDetector();
        /**
         * \brief destructor of CollisionDetector_hpp class
         */
        ~CollisionDetector();
        /**
         * \brief given a vector of lines detect if the path is feasible
         * @param lines_i vector of line that we want to check
         * @param map map of the arena
         * @param max_curvature curvature of the path
         * @return true if there is a collision false otherwise
         */
        std::pair<bool,Cell*> detectCollision(std::vector<Line> &lines_i, Map* map, double max_curvature);
        
    };
    
    CollisionDetector collisionDetector;
};
    
}

#endif /* PathEFinder_hpp */
