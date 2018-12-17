#include <iostream>
#include "../c++Files/Visualizing/Visualizer.hpp"
#include "Headers/MissionPlanning.hpp"

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

    
    Digit_Recognition::PeopleStorage peops = map.getPeople();
    std::vector<Circle*> circle = peops.getCircles();
    
    //will get undistorted image
    //will have to do intrinsic calibration
    //will have to do projection etc..
    //function bool proprocessMap map
    
    map.quickCalculation = true;
    MissionPlanning m = MissionPlanning(&map);
    m.plan_mission_two();

    return 0;
}
