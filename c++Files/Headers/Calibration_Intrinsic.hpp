//
//  Calibration_Intrinsic.hpp
//  LAR_1_0
//
//  Created by Air Marvin on 04.10.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#ifndef Calibration_Intrinsic_hpp
#define Calibration_Intrinsic_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include "Settings.hpp"

enum { DETECTION = 0, CAPTURING = 1, CALIBRATED = 2 };

class Calibration_Instrinsic {
public:
    Calibration_Instrinsic();
    ~Calibration_Instrinsic();
    
    static void performCalibration(const std::string cali_config);
    
    static bool runCalibrationAndSave(Settings& s, Size imageSize, Mat&  cameraMatrix, Mat& distCoeffs,
                               std::vector<std::vector<Point2f> > imagePoints );
    static bool runCalibration( Settings& s, Size& imageSize, Mat& cameraMatrix, Mat& distCoeffs,
                               std::vector<std::vector<Point2f> > imagePoints, std::vector<Mat>& rvecs, std::vector<Mat>& tvecs,
                               std::vector<float>& reprojErrs,  double& totalAvgErr);
    
private:
    
    static void saveCameraParams( Settings& s, Size& imageSize, Mat& cameraMatrix, Mat& distCoeffs,
                                 const std::vector<Mat>& rvecs, const std::vector<Mat>& tvecs,
                                 const std::vector<float>& reprojErrs, const std::vector<std::vector<Point2f> >& imagePoints,
                                 double totalAvgErr );
    static double computeReprojectionErrors( const std::vector<std::vector<Point3f> >& objectPoints,
                                            const std::vector<std::vector<Point2f> >& imagePoints,
                                            const std::vector<Mat>& rvecs, const std::vector<Mat>& tvecs,
                                            const Mat& cameraMatrix , const Mat& distCoeffs,
                                            std::vector<float>& perViewErrors, bool fisheye);
    static void calcBoardCornerPositions(Size boardSize, float squareSize, std::vector<Point3f>& corners,
                                         Settings::Pattern patternType /*= Settings::CHESSBOARD*/);
    
};

#endif /* Calibration_Intrinsic_hpp */
