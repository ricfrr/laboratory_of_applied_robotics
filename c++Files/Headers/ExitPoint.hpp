#ifndef ExitPoint_hpp
#define ExitPoint_hpp
#include "Shape.hpp"
#include <vector>

using namespace cv;
/*!
  * ExitPoint class is able to detect and save the exit point given a photo
*/
class ExitPoint : public Shape
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

    /*!
     * return a list of corners in a clockwise order
     * @return list of corners in a clockwise order
     */
    std::vector<cv::Point> getCorners();
    /*!
     * set the list of corners
     * @param corners list of corners that have to be setted
     */
    void setCorners(std::vector<cv::Point> corners);

    /*!
     *  return the top left corner of the exit point
     * @return return the top left corner of the exit point
     */
    cv::Point getTopLeft();
    /*!
     * set the top left corner
     * @param topLeft top left corner that has to be setted
     */
    void setTopLeft(cv::Point topLeft);
    /*!
     * return the top right corner of the exit point
     * @return return the top right corner of the exit point
     */
    cv::Point getTopRight();
    /*!
     * set the top right corner
     * @param topRight top right corner tha has to be setted
     */
    void setTopRight(cv::Point topRight);
    /*!
     * return the bottom left corner
     * @return the bottom  left corner
     */
    cv::Point getBottomLeft();
    /*!
     * set the bottom left corner
     * @param bottomLeft bottom left corner of the exit point
     */
    void setBottomLeft(cv::Point bottomLeft);
    /*!
     * return the bottom right corner of the exit point
     * @return bottom right corner of the exit point
     */
    cv::Point getBottomRight();
    /*!
     * set the bottom right corner
     * @param bottomRight bottom right corner of the exit point
     */
    void setBottomRight(cv::Point bottomRight);

private:
  void findHalf(int &half_h, int &half_w, const std::vector<cv::Point> &corners);
  const int epsilon_approx = 20; // value for detect poly
  cv::Point top_left, top_right, bottom_left, bottom_right;
};

#endif /* ExitPoint_hpp */
