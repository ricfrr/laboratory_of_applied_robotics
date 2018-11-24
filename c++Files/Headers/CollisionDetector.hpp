#ifndef CollisionDetector_hpp
#define CollisionDetector_hpp

#include <opencv2/core/core.hpp>
#include <cmath>
#include "Line.hpp"
#include "Map.hpp"

using namespace cv;
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
    bool detectCollision(std::vector<Line> lines_i, Map* map, double max_curvature);

};



#endif /* PossiblePath_hpp */
