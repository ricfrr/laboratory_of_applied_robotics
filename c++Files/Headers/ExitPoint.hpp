#ifndef ExitPoint_hpp
#define ExitPoint_hpp
#include "Rectangle.hpp"
#include <vector>
#include "Settings.hpp"

using namespace cv;
using namespace Geometry2D;
namespace LAR {
/**
  \brief ExitPoint class is able to detect and save the exit point given a photo
*/
class ExitPoint : public Rectangle
{

public:
  /*!
   * constructor of the ExitPoint class
   */
  ExitPoint();
  /*!
   * destructor of the ExitPoint class
   */
  ~ExitPoint();

  /*!
   * retrieve and set the corners in the ExitPoint object
   * @param img image of the arena
   */
  void findExitPoint(const Mat &img);
    
    std::pair<cv::Point,double> getEntryPoint();


private:
  const int epsilon_approx = 20; // value for detect poly
};
}
#endif /* ExitPoint_hpp */
