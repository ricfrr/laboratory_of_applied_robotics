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
    
    static Settings performCalibration(const string cali_config);
    
    static bool runCalibrationAndSave(Settings& s, Size imageSize, Mat&  cameraMatrix, Mat& distCoeffs,
                               vector<vector<Point2f> > imagePoints );
    static bool runCalibration( Settings& s, Size& imageSize, Mat& cameraMatrix, Mat& distCoeffs,
                               vector<vector<Point2f> > imagePoints, vector<Mat>& rvecs, vector<Mat>& tvecs,
                               vector<float>& reprojErrs,  double& totalAvgErr);
    
private:
    
    static void saveCameraParams( Settings& s, Size& imageSize, Mat& cameraMatrix, Mat& distCoeffs,
                                 const vector<Mat>& rvecs, const vector<Mat>& tvecs,
                                 const vector<float>& reprojErrs, const vector<vector<Point2f> >& imagePoints,
                                 double totalAvgErr );
    static double computeReprojectionErrors( const vector<vector<Point3f> >& objectPoints,
                                            const vector<vector<Point2f> >& imagePoints,
                                            const vector<Mat>& rvecs, const vector<Mat>& tvecs,
                                            const Mat& cameraMatrix , const Mat& distCoeffs,
                                            vector<float>& perViewErrors, bool fisheye);
    static void calcBoardCornerPositions(Size boardSize, float squareSize, vector<Point3f>& corners,
                                         Settings::Pattern patternType /*= Settings::CHESSBOARD*/);
    
};

#endif /* Calibration_Intrinsic_hpp */
