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

/*int main(int argc, const char *argv[]) {

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
    cv::imwrite("persp_img.png", persp_img);

    for(int i =0 ; i<frames.size(); i++){
        std::cout<<"index "<<i<<std::endl;
        robot_plane = ipm.detectRobotPlane(frames[i]);
        robot.findRobot(robot_plane);
        std::cout<<robot.center<<std::endl;
        circle(robot_plane, robot.center, 3, Scalar(0, 255, 0), -1, 8, 0);
        cv::imshow("plane",robot_plane);
        cv::waitKey(1);
    }

    return 0;
}*/

int main(int argc, char * argv[])
{


    RobotProject rp = RobotProject(argc,argv);
    //RobotProject rp(argc, argv);
    std::vector<cv::Mat> frames;
    cv:Mat img = imread(argv[1]);
    //extract_frames(argv[1],frames);
    if (!rp.preprocessMap(img))
    {
        std::cerr << "(Critical) Failed to preprocess map" << std::endl;
        return false;
    }

    ApiPath path;
    if (!rp.planPath(frames[0], path))
    {
        std::cerr << "(Critical) Failed to plan path" << std::endl;
        return false;
    }

    int i =0;
    while (i<frames.size())
    {

        std::vector<double> state;
        if (!rp.localize(frames[i], state))
        {
            std::cerr << "(Warning) Failed localization" << std::endl;
            continue;
        }

        i++;
    }

    return 0;

}
