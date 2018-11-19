#ifndef Car_hpp
#define Car_hpp

#include "Position.hpp"
/**
 * \brief class for storing data that describes the path
 */
class PathCoordinates {

public:
    /**
     * \brief constructor of the PathCoordinates class
     * @param position_in initial position of the path
     * @param position_fin final position if the path
     * @param curvature_p curvature of the path
     */
    PathCoordinates(Position position_in, Position position_fin, double curvature_p);
    /**
     * \brief destructor of the PathCoordinates class
     */
    ~PathCoordinates();
    /**
     * set the initial and the final position of the path
     * @param position_in initial position of the path
     * @param position_fin final position of the path
     */
    void setPositions(Position position_in, Position position_fin);
    /**
     * \brief return the initial position of the path
     * @return the initial position of the path
     */
    Position getInitialPosition();
    /**
     * \brief return the final position of the path
     * @return the final position of the path
     */
    Position getFinalPosition();
    /**
     * \brief return the curvature of the path
     * @return the curvature of the path
     */
    double getMaxCurvature();
    /**
     * \brief set the max curvature of the path
     * @param curvature_p the max curvature of the path 
     */
    void setMaxCurvature(double curvature_p);


private:

    Position initial_position = Position(cv::Point2d(), 0);
    Position final_position = Position(cv::Point2d(), 0);

    double Kmax;
};

#endif /* Car_hpp */
