#ifndef Cell_hpp
#define Cell_hpp
#include <vector>

#include "Shape.hpp"

using namespace cv;

class Cell
{

  public:
    Cell();
    ~Cell();

    // setter and getter of the points
    std::vector<cv::Point> getCorners();
    void setCorners(std::vector<cv::Point> corners);

    cv::Point getTopLeft();
    void setTopLeft(cv::Point topLeft);

    cv::Point getTopRight();
    void setTopRight(cv::Point topRight);

    cv::Point getBottomLeft();
    void setBottomLeft(cv::Point bottomLeft);

    cv::Point getBottomRight();
    void setBottomRight(cv::Point bottomRight);

    // set content of cell 
    bool isEmpty();
    bool isExit();
    bool isBorder();
    bool isObstacle();
    bool isRescue();

    void setEmpty();
    void setExit();
    void setBorder();
    void setObstacle();
    void setRescue();

   private: 
    
    void findHalf(int &half_h, int &half_w, const std::vector<cv::Point> &corners);
    bool empty, exit_p, border, obstacle, rescue;
    cv::Point top_left, top_right, bottom_left, bottom_right;
};

#endif /* Cell_hpp */
