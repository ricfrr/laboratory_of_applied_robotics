#include <iostream>
#include "../c++Files/Visualizing/Visualizer.hpp"

using namespace std;


PathCoordinates initialize();

const string intrinsic_calibration = "../config/intrinsic_calibration.xml";

int main(int argc, const char *argv[]){
    
//    std::cout << "hello world" << std::endl;
    
    cv::Mat persp_img;
    Inverse_Perspective_Mapping ipm = Inverse_Perspective_Mapping();
    persp_img = ipm.run(intrinsic_calibration, argv[1], "../config/fullCalibration.yml");

    //cv::Mat persp_img = cv::imread(argv[1]);
    // MAP
    Map map = Map();
    map.createMap(persp_img);

    
    PeopleStorage peops = map.getPeople();
    std::vector<Circle*> circle = peops.getCircles();
    
//    cv::Point2d(100,400)
//    cv::Point2d(100,550)

    double or1 = 0*M_PI;
    double or2 = 1.5*M_PI;
    std::pair<cv::Point,double> exit = map.getExitPoint().getEntryPoint();
    Position finalPos =  Position(exit.first,exit.second);finalPos.orientation_locked = true;

    Path *path1_2 =     new Path(Position(circle[3]->getCenter(),or1),
                                 Position(circle[0]->getCenter(),or2),0.05,&map);
    Path *path2_3 =     new Path(path1_2->end_point,
                                 Position(circle[1]->getCenter(),or2),0.05,&map);
    Path *path3_4 =     new Path(path2_3->end_point,
                                 Position(circle[2]->getCenter(),or2),0.05,&map);
    Path* path4_x =     new Path(path3_4->end_point,
                                finalPos,0.05,&map);

    Path* path1_3 =     new Path(*path1_2,*path2_3);
    Path* path1_4 =     new Path(*path1_3,*path3_4);
    Path* path1_exit =  new Path(*path1_4, *path4_x);
    
    Visualizer v(map,path1_exit);
    v.visualize();
    v.simulate();

    return 0;
}
