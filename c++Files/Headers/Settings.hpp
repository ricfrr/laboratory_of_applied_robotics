//
//  Settings.hpp
//  LAR_1_0
//
//  Created by Air Marvin on 04.10.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#ifndef Settings_hpp
#define Settings_hpp

#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <cstdio>

#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

/**
 \brief A data structure that stores important information for calibrating a camera.
 
 \discussion It decides which input stream and calibration method will be used. It can be used to construct the Calibration_Intrinsic object.
 \see Calibration_Instrinsic
 */
class Settings
{
public:
    Settings() : goodInput(false) {}
    enum Pattern { NOT_EXISTING, CHESSBOARD, CIRCLES_GRID, ASYMMETRIC_CIRCLES_GRID };
    enum InputType { INVALID, CAMERA, VIDEO_FILE, IMAGE_LIST };
    
    void write(FileStorage& fs) const;                        //Write serialization for this class

    void read(const FileNode& node);
    
    void validate();
    
    Mat nextImage();
    
    static bool readStringList( const std::string& filename, std::vector<std::string>& l );
    
    static bool isListOfImages( const std::string& filename);

public:
    Size boardSize;              /// The size of the board -> Number of items by width and height
    Pattern calibrationPattern;  /// One of the Chessboard, circles, or asymmetric circle pattern
    float squareSize;            /// The size of a square in your defined unit (point, millimeter,etc).
    int nrFrames;                /// The number of frames to use from the input for calibration
    float aspectRatio;           /// The aspect ratio
    int delay;                   /// In case of a video input
    bool writePoints;            /// Write detected feature points
    bool writeExtrinsics;        /// Write extrinsic parameters
    bool calibZeroTangentDist;   /// Assume zero tangential distortion
    bool calibFixPrincipalPoint; /// Fix the principal point at the center
    bool flipVertical;           /// Flip the captured images around the horizontal axis
    std::string outputFileName;       /// The name of the file where to write
    bool showUndistorsed;        /// Show undistorted images after calibration
    std::string input;                /// The input ->
    bool useFisheye;             /// use fisheye camera model for calibration
    bool fixK1;                  /// fix K1 distortion coefficient
    bool fixK2;                  /// fix K2 distortion coefficient
    bool fixK3;                  /// fix K3 distortion coefficient
    bool fixK4;                  /// fix K4 distortion coefficient
    bool fixK5;                  /// fix K5 distortion coefficient
    
    int cameraID;
    std::vector<std::string> imageList;
    size_t atImageList;
    VideoCapture inputCapture;
    InputType inputType;
    bool goodInput;
    int flag;

    int IMG_WIDTH=400;
    int IMG_LENGHT=600;
    int N_ROW_MAP=300;
    int N_COL_MAP=200;
    int GAP_PERSP=15;
    
private:
    std::string patternToUse;
    
    
};

#endif /* Settings_hpp */
