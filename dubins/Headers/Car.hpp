#ifndef Car_hpp
#define Car_hpp

#include "Position.hpp"

class Car {

public:

    Car(Position position_in, Position position_fin, double curvature_p);

    ~Car();

    void setPositions(Position position_in, Position position_fin);

    Position getInitialPosition();

    Position getFinalPosition();

    double getMaxCurvature();

    void setMaxCurvature(double curvature_p);


private:

    Position initial_position = Position(cv::Point2d(), 0);
    Position final_position = Position(cv::Point2d(), 0);

    double Kmax;
};

#endif /* Car_hpp */
