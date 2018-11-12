#ifndef People_hpp
#define People_hpp

#include "Circle.hpp"
#include <vector>

using namespace cv;

/**
 \brief class for detecting and storing people
 */
class People : public Circle {

public:
    /*!
     * constructor of people class
     */
    People();
    People(std::pair<int,int> digit, cv::Rect rect);
    /*!
     * destructor of people class
     */
    ~People();
    
    /// digit
    int name;
    /// confidence that name is correct
    int confidence;
    

private:
    const int epsilon_approx = 7;

};

#endif /* People_hpp */
