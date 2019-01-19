//
//  Settings.cpp
//  LAR_1_0
//
//  Created by Air Marvin on 04.10.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#include "../Headers/Settings.hpp"

using namespace ImageProcessing;

void Settings::write(FileStorage& fs) const                        //Write serialization for this class
{
    fs << "{"
    << "BoardSize_Width"  << boardSize.width
    << "BoardSize_Height" << boardSize.height
    << "Square_Size"         << squareSize
    << "Calibrate_Pattern" << patternToUse
    << "Calibrate_NrOfFrameToUse" << nrFrames
    << "Calibrate_FixAspectRatio" << aspectRatio
    << "Calibrate_AssumeZeroTangentialDistortion" << calibZeroTangentDist
    << "Calibrate_FixPrincipalPointAtTheCenter" << calibFixPrincipalPoint
    
    << "Write_DetectedFeaturePoints" << writePoints
    << "Write_extrinsicParameters"   << writeExtrinsics
    << "Write_outputFileName"  << outputFileName
    
    << "Show_UndistortedImage" << showUndistorsed
    
    << "Input_FlipAroundHorizontalAxis" << flipVertical
    << "Input_Delay" << delay
    << "Input" << input
    << "}";
}

void Settings::readFile(const FileNode& node)                          //Read serialization for this class
{
    node["BoardSize_Width" ] >> boardSize.width;
    node["BoardSize_Height"] >> boardSize.height;
    node["Calibrate_Pattern"] >> patternToUse;
    node["Square_Size"]  >> squareSize;
    node["Calibrate_NrOfFrameToUse"] >> nrFrames;
    node["Calibrate_FixAspectRatio"] >> aspectRatio;
    node["Write_DetectedFeaturePoints"] >> writePoints;
    node["Write_extrinsicParameters"] >> writeExtrinsics;
    node["Write_outputFileName"] >> outputFileName;
    node["Calibrate_AssumeZeroTangentialDistortion"] >> calibZeroTangentDist;
    node["Calibrate_FixPrincipalPointAtTheCenter"] >> calibFixPrincipalPoint;
    node["Calibrate_UseFisheyeModel"] >> useFisheye;
    node["Input_FlipAroundHorizontalAxis"] >> flipVertical;
    node["Show_UndistortedImage"] >> showUndistorsed;
    node["Input"] >> input;
    node["Input_Delay"] >> delay;
    node["Fix_K1"] >> fixK1;
    node["Fix_K2"] >> fixK2;
    node["Fix_K3"] >> fixK3;
    node["Fix_K4"] >> fixK4;
    node["Fix_K5"] >> fixK5;
    
    validate();
}

///validates the parameters of the chessboard
void Settings::validate()
{
    //parameters of chessboard
    goodInput = true;
    if (boardSize.width <= 0 || boardSize.height <= 0)
    {
        std::cerr << "Invalid Board size: " << boardSize.width << " " << boardSize.height << std::endl;
        goodInput = false;
    }
    if (squareSize <= 10e-6)
    {
        std::cerr << "Invalid square size " << squareSize << std::endl;
        goodInput = false;
    }
    if (nrFrames <= 0)
    {
        std::cerr << "Invalid number of frames " << nrFrames << std::endl;
        goodInput = false;
    }
    
    if (input.empty())      // Check for valid input
    inputType = INVALID;
    else
    {
        //if the input is given as an integer of 0 to 9 the inputtype is set to camera
        if (input[0] >= '0' && input[0] <= '9')
        {
            std::stringstream ss(input);
            ss >> cameraID;
            inputType = CAMERA;
        }
        //else the inputtype is set to a list of images
        else
        {
            //check if the input is an xml file and store content in imagelist vector
            //***no check whether the input really contains image files
            if (isListOfImages(input) && readStringList(input, imageList))
            {
                inputType = IMAGE_LIST;
                nrFrames = (nrFrames < (int)imageList.size()) ? nrFrames : (int)imageList.size();
            }
            else // if no list was given a video file is expected
            inputType = VIDEO_FILE;
        }
        
        //proceed with opening the right object according to input type
        if (inputType == CAMERA)
        inputCapture.open(cameraID);
        if (inputType == VIDEO_FILE)
        inputCapture.open(input);
        if (inputType != IMAGE_LIST && !inputCapture.isOpened())
        inputType = INVALID;
    }
    if (inputType == INVALID)
    {
        std::cerr << " Input does not exist: " << input;
        goodInput = false;
    }
    
    //bitwise or operation with the flags to set flag properties
    flag = 0;
    if(calibFixPrincipalPoint) flag |= CALIB_FIX_PRINCIPAL_POINT;
    if(calibZeroTangentDist)   flag |= CALIB_ZERO_TANGENT_DIST;
    if(aspectRatio)            flag |= CALIB_FIX_ASPECT_RATIO;
    if(fixK1)                  flag |= CALIB_FIX_K1;
    if(fixK2)                  flag |= CALIB_FIX_K2;
    if(fixK3)                  flag |= CALIB_FIX_K3;
    if(fixK4)                  flag |= CALIB_FIX_K4;
    if(fixK5)                  flag |= CALIB_FIX_K5;
    
    if (useFisheye) {
        // the fisheye model has its own enum, so overwrite the flags
        flag = fisheye::CALIB_FIX_SKEW | fisheye::CALIB_RECOMPUTE_EXTRINSIC;
        if(fixK1)                   flag |= fisheye::CALIB_FIX_K1;
        if(fixK2)                   flag |= fisheye::CALIB_FIX_K2;
        if(fixK3)                   flag |= fisheye::CALIB_FIX_K3;
        if(fixK4)                   flag |= fisheye::CALIB_FIX_K4;
        if (calibFixPrincipalPoint) flag |= fisheye::CALIB_FIX_PRINCIPAL_POINT;
    }
    
    calibrationPattern = NOT_EXISTING;
    if (!patternToUse.compare("CHESSBOARD")) calibrationPattern = CHESSBOARD;
    if (!patternToUse.compare("CIRCLES_GRID")) calibrationPattern = CIRCLES_GRID;
    if (!patternToUse.compare("ASYMMETRIC_CIRCLES_GRID")) calibrationPattern = ASYMMETRIC_CIRCLES_GRID;
    if (calibrationPattern == NOT_EXISTING)
    {
        std::cerr << " Camera calibration mode does not exist: " << patternToUse << std::endl;
        goodInput = false;
    }
    atImageList = 0;
    
}

Mat Settings::nextImage()
{
    Mat result;
    if( inputCapture.isOpened() )
    {
        Mat view0;
        inputCapture >> view0;
        view0.copyTo(result);
    }
    else if( atImageList < imageList.size() )
    result = imread(imageList[atImageList++], IMREAD_COLOR);
    
    return result;
}

///extract the images out of the first expected xml file list node
bool Settings::readStringList( const std::string& filename, std::vector<std::string>& l )
{
    l.clear();
    FileStorage fs(filename, FileStorage::READ);
    if( !fs.isOpened() )
    return false;
    FileNode n = fs.getFirstTopLevelNode();
    if( n.type() != FileNode::SEQ )
    return false;
    FileNodeIterator it = n.begin(), it_end = n.end();
    for( ; it != it_end; ++it )
    l.push_back((std::string)*it);
    return true;
}

///checks if the giving filename is an xml, yaml or yml file
bool Settings::isListOfImages( const std::string& filename)
{
    std::string s(filename);
    // Look for file extension
    //if file is not of type xml yaml or yml it's no list of images
    //***not checking if list really contains images !!!
    if( s.find(".xml") == std::string::npos && s.find(".yaml") == std::string::npos && s.find(".yml") == std::string::npos )
    return false;
    else
    return true;
}
