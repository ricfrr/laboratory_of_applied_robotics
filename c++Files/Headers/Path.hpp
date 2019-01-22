//
//  Path.hpp
//  
//
//  Created by Air Marvin on 13.11.18.
//

#ifndef Path_hpp
#define Path_hpp

#include <stdio.h>
#include <vector>
#include <opencv2/core/types.hpp>
#include "DubinPathFinder.hpp"
#include "Map.hpp"

namespace Path2D
{
class Path {
    
public:
    Path();
    /**
     \brief given the start point and the end point is able to find the best path
     * @param start_point start point
     * @param end_point end point
     */
    Path(Position start_point, Position end_point, double curvature, Map *map_i);
    template <class T>
    Path(Position start_point, Position end_point, double curvature, Map *map_i, T* pathFinder, bool complex = true);
    template <class T>
    explicit Path(PathCoordinates coordinates, Map *map, T* pathFinder, bool complex = true);
    ~Path();
    
    ///constructs a new path out of two existant path objects
    Path(Path path1, Path path2);
    
    /**
        \brief function that detects the best path, for now uses only dubins path
     */
    void findPath();
    
    void findPathSimple();
    
    /**
     \brief splits a path so it goes over an interediate point
     */
    template <class T>
    static void split(Path &path, cv::Point intermediate);

    /**
     \brief set the lines of the path
     */
    void setLines(std::vector<Line>);
    /**
     * \brief return a vector of lines
     * @return vector of lines
     */
    std::vector<Line> getLines();
    
    std::vector<Line*> getLinesP();
    /**
     * \brief set the start point of the path
     * @param start_point start point of the path
     */
    void setStartPoint(Position start_point);
    /**
     * \brief return the start point of the path
     * @return the start point of the path
     */
    Position getStartPoint();
    /**
     * \brief set the end point of the path
     * @param end_point the end point of the path
     */
    void setEndPoint(Position end_point);
    /**
     * \brief return the end point of the path
     * @return the end point of the path
     */
    Position getEndPoint();
    /**
     * \brief set max curvature of the path
     * @param curvature max curvature of the path
     */
    void setMaxCurvature(double curvature);
    /**
     * \brief return of max curvature of the path
     * @return the max curvature of the path
     */
    double getMaxCurvature();
    /**
     * \brief set the length of the path
     * @param length length of the path
     */
    void setLength(double length);
    /**
     * \brief return the length of the path
     * @return the length
     * @return the length
     */
    double getLength();

    void setMap(Map* map_i);
    Map* getMap();

    Position start_point;
    Position end_point;
    double maxCurvature;
    std::vector<Line> lines;
    double length;
    Map* map;
    
    template <class T>
    void setFinder(PathCoordinates pc);
    
private:
    PathFinder* finder;
};
}
#endif /* Path_hpp */
