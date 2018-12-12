//
//  Line.hpp
//  
//
//  Created by Air Marvin on 13.11.18.
//

#ifndef Line_hpp
#define Line_hpp

#include <stdio.h>
#include <opencv2/core/core.hpp>
#include "Position.hpp"


namespace Path2D {
    namespace Element{
/**
 * \brief Line class that describes the basic line of a path
 */
class Line {
public:

    /**
     * \brief constructor of Line class
     */
    Line();

    /**
     * \brief contrusctor of Line class
     * @param start_point start point of the path
     * @param end_point end point of the path
     */
    Line(Position start_point, Position end_point);

    /**
     * \brief destructor of the line path
     */
    ~Line();

    /**
     * \brief set the start position of the path
     * @param start_point_i start point of the path
     */
    void setStartPoint(Position start_point_i);

    /**
     * \brief return the start point of the path
     * @return the start point
     */
    Position getStartPoint();

    /**
     * \brief set the end point of the path
     * @param end_point_i the end point of the path
     */
    void setEndPoint(Position end_point_i);

    /**
     * \brief return the end point of the path
     * @return the end point of the path
     */
    Position getEndPoint();

    /**
     * \brief set the length of the path
     * @param length_i the length of the path
     */
    void setLength(double length_i);

    /**
     * return the length of the path
     * @return the length of the path
     */
    double getLength();

    /**
     * \brief function that given the curvature the start point and the lenght of the path allow to find the end point of the line
     * @param k curvature
     * @param start start point of the line
     * @param length length of the line
     * @return
     */
    Position findPointDistance(double k, Position start, double length);

    /**
     * \brief Implementation of function sinc(t), returning 1 for t==0, and sin(t)/t
     * @param inp input angle
     * @return 1 for inp==0 otherwise sin(inp)/inp
     */
    double sinc(double inp);

    /**
     * \brief Normalize an angle (in range [0,2*pi))
     * @param ang input angle
     * @return the normalize angle in range  [0,2*pi]
     */
    double mod2pi(double ang);

    /**
       \brief allow to set the curvature of the line
    */

    void setCurvature(double curvature_i);

    /**
     * return the curvature of the line
     * @return curvature of the line
     */
    double getCurvature();

    void setIntermediatePoints(std::vector<Point2d> &intermediate_points);
    std::vector<Point2d> getIntermediatePoints();

private:

    Position start_point;
    Position end_point;
    std::vector<Point2d> intermediate_points;
    double length;
    double curvature; // 1: left 0: straight -1 :right

};
}
}

#endif /* Line_hpp */
