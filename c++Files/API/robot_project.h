#ifndef ROBOT_PROJECT
#define ROBOT_PROJECT

#include <opencv2/opencv.hpp>
#include <vector>
#include "path.h"

#include "../Headers/RoboticMapping.hpp"

// Define your own class RobotProject, that implements and exposes the following methods.
// NB: The input images are already undistorted.
class RobotProject
{
public:
  // Constructor taking as argument the command line parameters
  RobotProject(int argc, char * argv[]);

  // Method invoked to preprocess the map (extrinsic calibration + reconstruction of layout)
  bool preprocessMap(cv::Mat const & img);

  // Method invoked when a new path must be planned (detect initial robot position from img)
  bool planPath(cv::Mat const & img, Path & path);

  // Method invoked periodically to determine the position of the robot within the map.
  // The output state is a vector of three elements, x, y and theta.
  bool localize(cv::Mat const & img, 
                std::vector<double> & state);
    
private:
    std::string source_img_path;
    std::string calibration_filepath;
    std::string intrinsic_calibration;
    
    Map* map;

};

#endif
