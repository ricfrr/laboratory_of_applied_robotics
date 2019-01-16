//
//  robot_project.cpp
//  LAR_1_0
//
//  Created by Air Marvin on 20.12.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#include "robot_project.h"


RobotProject::RobotProject(int argc,  char * argv[]){
    
    //this->source_img_path       = argv[1];
    this->calibration_filepath  = argv[2];
    this->intrinsic_calibration = argv[3];
    
    this->mission               = std::atol(argv[4]);
    this->ipm = Inverse_Perspective_Mapping();
    
    this->map = new Map(DigitRecognitionAlgo::tesseractOCP, 80, 15, 3.0,0);
    
    if(argv[5] == "quick")
        this->map->quickCalculation = true;
}

void RobotProject::start(){
    cv::Mat img = cv::imread(this->source_img_path);
    preprocessMap(img);
    planPath(img, path);
}

bool RobotProject::preprocessMap(cv::Mat const & img){
    
    //take the image and preprocess
    cv::Mat persp_img,robot_plane;

    persp_img = ipm.run(this->intrinsic_calibration, img, calibration_filepath);
    //detect the robot plane
//    cv::imshow("persp",persp_img);
//    cv::waitKey(0);
    robot_plane = ipm.detectRobotPlane(img);
    cv::imshow("robot_plane",robot_plane);
    cv::waitKey(0);
    //important settings
    // - img width and height in pixels
//    Settings::IMG_WIDTH = persp_img.cols;
//    Settings::IMG_LENGHT = persp_img.rows;
    
    //important Parameters (in constructor of map)
    //  - DigitRecognition min confidence
    //  - consecutive delusions
    //  - main CRA
    //  - min rotation angle
    map->createMap(persp_img,robot_plane);
    map->save("savedMap.json");
    std::cout<<map->wasSuccess()<<std::endl;
    return map->wasSuccess();
}

bool RobotProject::planPath(cv::Mat const & img, ApiPath & path){
    
    MissionPlanning m = MissionPlanning(map);
    switch(mission){
        case 1:
            m.plan_mission_one();
            break;
        case 2:
            m.plan_mission_two();
            break;
    }
    std::vector<Pose> pose;
    //initialization of pose vector
    double int_point_counter =0;
    double  points_number =0;
    for (int i =0; m.path_p->lines.size(); i++){
        points_number+= m.path_p->lines[i].getIntermediatePoints().size();
    }
    for (int i =0; m.path_p->lines.size(); i++){
        std::vector<cv::Point2d> intermediate_points = m.path_p->lines[i].getIntermediatePoints();
        for (int j =0; j<intermediate_points.size(); j++){
            //int_point_counter*5*Setting::PIXEL_SCALE i the distance from the starting point
            std::pair<double,double> mm_point = Geometry::convertPixelToMillimeterInMapPlane(intermediate_points[j],map->getStartPoint());
            pose.push_back(Pose(int_point_counter/points_number,mm_point.first,mm_point.second,Geometry::angle_rad(intermediate_points[j],intermediate_points[j+1]),m.path_p->lines[i].getCurvature())); //TODO check theta and kappa values and trasform the x and y in meters
            int_point_counter++;
        }
    }
    path.setPoints(pose);
    return true;
}

bool RobotProject::localize(cv::Mat const & img,
                            std::vector<double> & state){
    
    state.clear();
    cv::Mat robot_plane = ipm.detectRobotPlane(img);
    //do not reconstruct map because too expensive
    //find robot shape
    //calculate COM
    //calculate orientation
    Robot robo;
    bool result = robo.findRobot(robot_plane);
    
    cv:Point start = map->getStartPoint();
    
    std::pair<double,double> coordinates =
    Geometry::convertPixelToMillimeterInRoboPlane(robo.center, start);
    
    double x = coordinates.first;
    double y = coordinates.second;
    double theta = robo.angle;
    
    state = {x,y,theta};
    
    //put info to state
    
    return result;
}
