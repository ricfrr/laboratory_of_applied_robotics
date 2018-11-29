#include <iostream>
#include "../c++Files/Visualizing/Visualizer.hpp"

using namespace std;


PathCoordinates initialize();

const string intrinsic_calibration = "config/intrinsic_calibration.xml";

int main(int argc, const char *argv[]){
    
//    std::cout << "hello world" << std::endl;
    
//    cv::Mat persp_img;
//    Inverse_Perspective_Mapping ipm = Inverse_Perspective_Mapping();
//    persp_img = ipm.run(intrinsic_calibration, argv[1], "config/fullCalibration.yml");

    cv::Mat persp_img = cv::imread(argv[1]);
    // MAP
    Map map = Map();
    map.createMap(persp_img);

    
    PeopleStorage peops = map.getPeople();
    std::vector<Circle*> circle = peops.getCircles();
    
    Path *path = new Path(Position(circle[4]->getCenter(),1/4*M_PI),
                          Position(circle[1]->getCenter(),3/4*M_PI),0.05,&map);
    
    Visualizer v(map,path);
    //v.visualize();
    v.simulate();

    return 0;
}
