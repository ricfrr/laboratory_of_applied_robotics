#include <iostream>
#include "../c++Files/Visualizing/Visualizer.hpp"

using namespace std;


PathCoordinates initialize();

const string intrinsic_calibration = "../config/intrinsic_calibration.xml";

int main(int argc, const char *argv[]){
    
//    std::cout << "hello world" << std::endl;
    
    //cv::Mat persp_img;
    //Inverse_Perspective_Mapping ipm = Inverse_Perspective_Mapping();
    //persp_img = ipm.run(intrinsic_calibration, argv[1], "../config/fullCalibration.yml");

    cv::Mat persp_img = cv::imread(argv[1]);
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

    //get last line's endpoint rather than the actual endpoint to make sure the orientation is correct in some cases
    /// !!!
    
    
    Path *path0_1 =     new Path(Position(cv::Point2d(60,60),or1),
                                 Position(circle[3]->getCenter(),or2),0.05,&map);
    Path *path1_2 =     new Path(path0_1->end_point,
                                 Position(circle[0]->getCenter(),or2),0.05,&map);
    Path *path2_3 =     new Path(path1_2->end_point,
                                 Position(circle[1]->getCenter(),or2),0.05,&map);
    Path *path3_4 =     new Path(path2_3->end_point,
                                 Position(circle[2]->getCenter(),or1),0.05,&map);
    Path* path4_x =     new Path(path3_4->end_point,
                                finalPos,0.05,&map);

    Path* path0_2 =     new Path(*path0_1,*path1_2);
    Path* path0_3 =     new Path(*path0_2,*path2_3);
    Path* path0_4 =     new Path(*path0_3,*path3_4);
    Path* path0_exit =  new Path(*path0_4, *path4_x);
    
    Path* path4_x_test =     new Path(Position(path3_4->end_point.getCoordinates(),or2),
                                 finalPos,0.05,&map);
    
    Visualizer v(map,path0_exit);
    v.visualize();
    v.simulate();

    return 0;
}
