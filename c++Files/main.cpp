#include <iostream>
#include "../c++Files/Visualizing/Visualizer.hpp"
#include "Headers/MissionPlanning.hpp"

using namespace std;


PathCoordinates initialize();

const std::string intrinsic_calibration = "../config/intrinsic_calibration.xml";

int main(int argc, const char *argv[]) {

//    std::cout << "hello world" << std::endl;

    Settings::IMG_WIDTH = 400;
    Settings::IMG_LENGHT = 600;
    
    cv::Mat persp_img;
    Inverse_Perspective_Mapping ipm = Inverse_Perspective_Mapping();
    persp_img = ipm.run(intrinsic_calibration, argv[1], "../config/fullCalibration.yml");

    //cv::imwrite("result_bad.png", persp_img);
    //cv::Mat persp_img = cv::imread(argv[1]);
    // MAP
    Map map = Map(DigitRecognitionAlgo::tesseractOCP, 80, 10, 5, 1);
    map.createMap(persp_img);
    
    map.save("savedMap.json");

    map.quickCalculation = true;
    MissionPlanning m = MissionPlanning(&map);
    m.plan_mission_two();

    return 0;
}
