#ifndef Arena_hpp
#define Arena_hpp
#include "Rectangle.hpp"
#include "Settings.hpp"
#include <vector>

using namespace cv;
using namespace Geometry2D;


/**
  \brief Arena class is able to detect and save the arena given a photo
*/
class Arena : public Rectangle
{

  public:
    /*!
     * Constructor of Arena
     */
    Arena();
    /*!
     * Destructor of Arene
     */
    ~Arena();
    
    /*!
     * Retrieve and set the cornerns in the Arena Object
     * @param img image of the arena
     */
    void findArena(const Mat &img);
    
//    /*!
//     * return a list of corners in a clockwise order
//     * @return list of corners in a clockwise order
//     */
//    std::vector<cv::Point> getCorners();
//    /*!
//     * set the list of corners
//     * @param corners list of corners that have to be setted
//     */
//    void setCorners(std::vector<cv::Point> corners);
//
//    /*!
//     *  return the top left corner of the arena
//     * @return return the top left corner of the arena
//     */
//    cv::Point getTopLeft();
//    /*!
//     * set the top left corner
//     * @param topLeft top left corner that has to be setted
//     */
//    void setTopLeft(cv::Point topLeft);
//    /*!
//     * return the top right corner of the arena
//     * @return return the top right corner of the arena
//     */
//    cv::Point getTopRight();
//    /*!
//     * set the top right corner
//     * @param topRight top right corner tha has to be setted
//     */
//    void setTopRight(cv::Point topRight);
//    /*!
//     * return the bottom left corner
//     * @return the bottom  left corner
//     */
//    cv::Point getBottomLeft();
//    /*!
//     * set the bottom left corner
//     * @param bottomLeft bottom left corner of the arena
//     */
//    void setBottomLeft(cv::Point bottomLeft);
//    /*!
//     * return the bottom right corner of the arena
//     * @return bottom right corner of the arena
//     */
//    cv::Point getBottomRight();
//    /*!
//     * set the bottom right corner
//     * @param bottomRight bottom right corner of the arena
//     */
//    void setBottomRight(cv::Point bottomRight);


  private:

    Settings settings; 
    const int epsilon_approx = 110;
    //cv::Point top_left,top_right,bottom_left,bottom_right;
};

#endif /* Arena_hpp */
