#ifndef Hexagon_hpp
#define Hexagon_hpp
#include <vector>

#include "Polygon.hpp"

using namespace cv;
/**
 \brief Class for handling hexagon obstacles
 */
class Hexagon : public Polygon
{

  public:
    /*!
     * constructor of the Hexagon class
     */
    Hexagon();
    
    Hexagon(std::vector<cv::Point> points);
    
    /*!
     * destructor of the Hexagon class
     */
    ~Hexagon();

    /*!
     * return a list of corners of the hexagon
     * @return list of corners of the hexagon
     */
    std::vector<cv::Point> getCorners();
    /*!
     * set the list of corners
     * @param corners list of corners
     */
    void setCorners(std::vector<cv::Point> corners);
    
    void assign_points();


  private:

    cv::Point getFirstCorner();
    void setFirstCorner(cv::Point firstC);

    cv::Point getSecondCorner();
    void setSecondCorner(cv::Point secondC);

    cv::Point getThirdCorner();
    void setThirdCorner(cv::Point thirdC);

    cv::Point getFourthCorner();
    void setFourthCorner(cv::Point fourthC);

    cv::Point getFifthCorner();
    void setFifthCorner(cv::Point fifthC);

    cv::Point getSixthCorner();
    void setSixthCorner(cv::Point sixthC);

    void findHalf(int &half_h, int &half_w,
                  const std::vector<cv::Point> &corners);

    cv::Point first, second, third, fourth, fifth, sixth;
};

#endif /* Hexagon_hpp */
