//
//  Path.hpp
//  
//
//  Created by Air Marvin on 13.11.18.
//

#ifndef Path_hpp
#define Path_hpp

#include <stdio.h>
#include "Geometrie.hpp"
#include <vector>
#include <opencv2/core/types.hpp>
#include "DubinPath.hpp"

class Path {
    
public:
    Path();
    /**
     \brief given the start point and the end point is able to find the best path
     * @param start_point start point
     * @param end_point end point
     */
    Path(Position start_point, Position end_point, double curvature);
    ~Path();
    
    /**
        \brief function that detects the best path, for now uses only dubins path
     */
    void findPath();

    /**
     \brief function that computes lines based on the path type and the givent point set
     */
    void setLines(std::vector<Line>);
    std::vector<Line> getLines();

    void setStartPoint(Position start_point);
    Position getStartPoint();

    void setEndPoint(Position end_point);
    Position getEndPoint();

    void setMaxCurvature(double curvature);
    double getMaxCurvature();

    void setLength(double length);
    double getLength();

    Position start_point;
    Position end_point;
    double maxCurvature;
    std::vector<Line> lines;
    double length;
};

#endif /* Path_hpp */
