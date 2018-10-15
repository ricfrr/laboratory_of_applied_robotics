#ifndef Circle_hpp
#define Circle_hpp
#include "Shape.hpp"
#include <vector>

using namespace cv;

class Circle : public Shape
{

  public:
    Circle();
    ~Circle();
    
    //
    int getRadius();
    void setRadius(int radius_c);
    cv::Point getCenter();
    void setCenter(cv::Point center_c);
    
   


  private:
    cv::Point center;
    int radius;
};

#endif /* Circle_hpp */
