#include <iostream>
#include "../c++Files/Visualizing/Visualizer.hpp"

#include "API/robot_project.h"

using namespace std;


PathCoordinates initialize();

const std::string intrinsic_calibration = "../config/intrinsic_calibration.xml";//"../data/exam_dataset/files/intrinsic_calibration.xml";//// ;

void extract_frames(const string &videoFilePath, std::vector<cv::Mat> &frames) {
    try {
        //open the video file
        VideoCapture cap(videoFilePath); // open the video file
        if (!cap.isOpened())  // check if we succeeded
            CV_Error(CV_StsError, "Can not open Video file");
        //cap.get(CV_CAP_PROP_FRAME_COUNT) contains the number of frames in the video;
        for (int frameNum = 0; frameNum < cap.get(CV_CAP_PROP_FRAME_COUNT); frameNum++) {
            Mat frame;
            cap >> frame; // get the next frame from video
            frames.push_back(frame);
        }
    }
    catch (cv::Exception &e) {
        cerr << e.msg << endl;
        exit(1);
    }
}

int main(int argc, const char *argv[]) {

//    std::cout << "hello world" << std::endl;
    //Calibration_Instrinsic::performCalibration(argv[1]);
    Settings::IMG_WIDTH = 400;
    Settings::IMG_LENGHT = 600;
    
//    cv::Mat persp_img;
//    Inverse_Perspective_Mapping ipm = Inverse_Perspective_Mapping();
//    persp_img = ipm.run(intrinsic_calibration, argv[1], "../config/fullCalibration.yml");
//
//    //cv::imwrite("result_bad.png", persp_img);
//    //cv::Mat persp_img = cv::imread(argv[1]);
//    // MAP
//    Map map = Map(DigitRecognitionAlgo::tesseractOCP, 80, 10, 5, 1);
//    map.createMap(persp_img);
//
//    map.save("savedMap.json");
//
//    map.quickCalculation = true;
//    MissionPlanning m = MissionPlanning(&map);
//    m.plan_mission_two();
    
    RobotProject project = RobotProject(argc, argv);
    project.start();

    cv::Mat persp_img;
    cv::Mat robot_plane;
    Robot robot = Robot();
    Inverse_Perspective_Mapping ipm = Inverse_Perspective_Mapping();
    // =======================TEST VIDEO =================================
    std::vector<cv::Mat> frames;
    extract_frames(argv[1],frames);
    std::cout<<"size frames"<<frames.size()<<std::endl;
    persp_img = ipm.run(intrinsic_calibration, frames[0], "../config/fullCalibration.yml");
    //int amount = frames.size()/10;
    //std::cout<<"amount :"<<amount<<std::endl;
    for(int i =0 ; i<frames.size(); i++){
        std::cout<<"index "<<i<<std::endl;
        if(i>=0){
       //     cv::imshow("frame",frames[i]);
        }
        robot_plane = ipm.detectRobotPlane(frames[i]);
        robot.findRobot(robot_plane);
        std::cout<<robot.center<<std::endl;
        circle(robot_plane, robot.center, 3, Scalar(0, 255, 0), -1, 8, 0);
        cv::imshow("plane",robot_plane);
        cv::waitKey(1);

    }

    return 0;
}
