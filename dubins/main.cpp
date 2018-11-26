#include <iostream>
#include "../c++Files/Visualizing/Visualizer.hpp"

using namespace std;


PathCoordinates initialize();

const string intrinsic_calibration = "config/intrinsic_calibration.xml";

int main(int argc, const char *argv[]){
    
//    std::cout << "hello world" << std::endl;
    
    cv::Mat persp_img;
    Inverse_Perspective_Mapping ipm = Inverse_Perspective_Mapping();
    persp_img = ipm.run(intrinsic_calibration, argv[1], "config/fullCalibration.yml");

    //cv::Mat persp_img = cv::imread(argv[1]);
    // MAP
    Map map = Map();
    map.createMap(persp_img);

    
    PeopleStorage peops = map.getPeople();
    std::vector<Circle*> circle = peops.getCircles();
    
    Path *path = new Path(Position(Point2d(circle[0]->getCenter().x,circle[0]->getCenter().y),1),
                          Position(Point2d(circle[1]->getCenter().x,circle[1]->getCenter().y),1),1);
    
    Visualizer v(map,path);
    v.visualize();
    //v.simulate();

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
