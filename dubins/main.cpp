#include <iostream>
#include "Headers/Position.hpp"
#include "Headers/PathCoordinates.hpp"
#include "Headers/Path.hpp"
using namespace std;


PathCoordinates initialize();

int main(){

    PathCoordinates pathCoordinates = initialize();
    Path path = Path(pathCoordinates.getInitialPosition(),pathCoordinates.getFinalPosition(),pathCoordinates.getMaxCurvature());

    return 0;
}

PathCoordinates initialize(){
    // initial position
    int in_x = 0;
    int in_y = 1;
    double in_orient = -1.0/3*M_PI;
    double Kmax = 1.0; //max curvature 1/kmax give us the radius
    cv::Point2d in_coordinates = Point2d(in_x,in_y);
    Position in_position = Position(in_coordinates,in_orient);
    // final position
    int f_x = 4;
    int f_y = 3;
    double f_orient = M_PI/3.0;
    cv::Point2d f_coordinates = Point2d(f_x,f_y);
    Position f_position = Position(f_coordinates,f_orient);


    PathCoordinates car = PathCoordinates(in_position,f_position,Kmax);
    return car;
}
