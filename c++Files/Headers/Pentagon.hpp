#ifndef Pentagon_hpp
#define Pentagon_hpp

#include <vector>

#include "Polygon.hpp"

using namespace cv;

/**
 \brief Class for hadling pentagon obstacles in the map
 */
class Pentagon : public Polygon {

public:
    /*!
     * constructor of Pentagon class
     */
    Pentagon();
    
    Pentagon(std::vector<cv::Point> points);
    
    /*!
     * destructor of Pentagon class
     */
    ~Pentagon();
    
    void assign_points();

    /*!
     * return the list of corners of the pentagons
     * @return the list of corners of the amaz
     */
    std::vector<cv::Point> getCorners();
    /*!
     * set the corners of the pentagon
     * @param corners list of corners to be setted
     */
    void setCorners(std::vector<cv::Point> corners);



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

    void findHalf(int &half_h, int &half_w,
                  const std::vector<cv::Point> &corners);

    cv::Point first, second, third, fourth, fifth; // corners of the pentagon
};

#endif /* Pentagon_hpp */
