//
//  Undistortion.cpp
//  LAR_1_0
//
//  Created by Air Marvin on 05.10.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#include "Undistortion.hpp"

Undistorsion::Undistorsion(string calibration_filename){
    this->calibration_filename = calibration_filename;
}

Undistorsion::~Undistorsion(){}

void Undistorsion::undistort_image(cv::Mat frame, cv::Mat frameUndist,
                                   InputArray cameraMatrix,
                                   InputArray distCoeffs,
                                   InputArray newCameraMatrix){
    

        undistort(frame, frameUndist, cameraMatrix, distCoeffs);
        
        cv::imshow( "Original", frame);
        cv::imshow( "Undistorted", frameUndist);
}

void Undistorsion::loadCoefficients(const std::string& filename,
                      cv::Mat& camera_matrix,
                      cv::Mat& distCoeffs)
{
    cv::FileStorage fs( filename, cv::FileStorage::READ );
    if (!fs.isOpened())
    {
        throw std::runtime_error("Could not open file " + filename);
    }
    fs["camera_matrix"] >> camera_matrix;
    fs["distortion_coefficients"] >> distCoeffs;
    fs.release();
}

// Capture the video stream from a camera, and undistort it using the
// calibration parameters
void Undistorsion::processVideo()
{
    cv::VideoCapture vc;
    if(vc.open(0)) {
        // Set up the capture device properties
        if (!(vc.set(cv::CAP_PROP_FRAME_WIDTH, 1280)
              && vc.set(cv::CAP_PROP_FRAME_HEIGHT, 1024)
              && vc.set(cv::CAP_PROP_FPS, 30)))
        {
            throw std::runtime_error("Failed to set parameters");
        }
    }
    else throw std::runtime_error("Failed to open the camera");
    
    cv::Mat frame, frameUndist;
    cv::namedWindow( "Original", cv::WINDOW_AUTOSIZE );
    cv::namedWindow( "Undistorted", cv::WINDOW_AUTOSIZE );
    
    cv::Mat cameraMatrix, distCoeffs;
    loadCoefficients(calibration_filename, cameraMatrix, distCoeffs);
    
    bool terminating = false;
    while (!terminating)
    {
        if(!vc.read(frame))
        {
            throw std::runtime_error("Failed to grab frame");
        }
        
        undistort_image(frame, frameUndist, cameraMatrix, distCoeffs);
        
        char c;
        c = cv::waitKey(30);
        switch (c)
        {
            case 'q':
                std::cout << "Terminating!" << std::endl;
                terminating = true;
                break;
            default:
                break;
        }
    }
    cv::destroyAllWindows();
}
