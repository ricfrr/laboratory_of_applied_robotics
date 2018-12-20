//
//  robot_project.cpp
//  LAR_1_0
//
//  Created by Air Marvin on 20.12.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#include "robot_project.h"


RobotProject::RobotProject(int argc, char * argv[]){
    
    this->source_img_path = argv[1];
    this->calibration_filepath = argv[2];
    this->intrinsic_calibration = argv[3];
    
    this->map = new Map;
}

bool RobotProject::preprocessMap(cv::Mat const & img){
    
    //take the image and preprocess
    cv::Mat persp_img;
    Inverse_Perspective_Mapping ipm = Inverse_Perspective_Mapping();
    persp_img = ipm.run(this->intrinsic_calibration, img, calibration_filepath);
    
    //important settings
    // - img width and height in pixels
    map->setting.IMG_WIDTH = persp_img.cols;
    map->setting.IMG_LENGHT = persp_img.rows;
    
    //important Parameters (in constructor of map)
    //  - DigitRecognition min confidence
    //  - consecutive delusions
    //  - main CRA
    //  - min rotation angle
    map->createMap(persp_img);
    
    return map->wasSuccess();
}

bool RobotProject::planPath(cv::Mat const & img, Path & path){
    
    return false;
}

bool RobotProject::localize(cv::Mat const & img,
                            std::vector<double> & state){
    
    state.clear();

    //do not reconstruct map because too expensive
    //find robot shape
    //calculate COM
    //calculate orientation
    
    //put info to state
    
    return false;
}
