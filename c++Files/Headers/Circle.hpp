#ifndef Circle_hpp
#define Circle_hpp
#include "Shape.hpp"
#include <vector>

using namespace cv;
/**
  \brief Circle class is able to detect and save the arena given a photo
*/
class Circle : public Shape
{

  public:
    /*!
     * constructor of Circle class
     */
    Circle();
    /*!
     * destructor of Circle class
     */
    ~Circle();
    
    /*!
     * return the radius of the circle
     * @return the radius of the circle
     */
    int getRadius();
    /*!
     * set the radius of the circle
     * @param radius_c the radius of the circle
     */
    void setRadius(int radius_c);
    /*!
     * return the center of the circle
     * @return  the center of the circle
     */
    cv::Point getCenter();
    /*!
     * set the center of the circle
     * @param center_c center of the circle
     */
    void setCenter(cv::Point center_c);
    /*!
     * return the digit of the circle
     * @return the digit
     */
    int getDigit();
    /*!
     * set the digit of the circle 
     * @param digit_i digit of the circle
     */
    void setDigit(int digit_i);
    
   


  private:
    cv::Point center;
    int radius, digit;
};

#endif /* Circle_hpp */
