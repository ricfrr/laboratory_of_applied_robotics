#include <iostream>
#include "Headers/Position.hpp"
#include "Headers/Car.hpp"
#include "Headers/DubinPath.hpp"
using namespace std;

const double PI = 3.14159265358979323846;

Car initialize();

int main(){
    Car car = initialize();
    DubinPath dubinPath = DubinPath(car);
    dubinPath.dubinShortestPath();
    return 0;
}

Car initialize(){
    // initial position
    int in_x = 0;
    int in_y = 1;
    double in_orient = -1.0/3*PI;
    double Kmax = 1.0;
    cv::Point2d in_coordinates = Point2d(in_x,in_y);
    Position in_position = Position(in_coordinates,in_orient);
    // final position
    int f_x = 4;
    int f_y = 3;
    double f_orient = PI/3.0;
    cv::Point2d f_coordinates = Point2d(f_x,f_y);
    Position f_position = Position(f_coordinates,f_orient);


    Car car = Car(in_position,f_position,Kmax);
    return car;
}