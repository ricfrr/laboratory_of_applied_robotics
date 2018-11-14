#ifndef People_hpp
#define People_hpp

#include "Circle.hpp"
#include <vector>

using namespace cv;

/**
 \brief class for representing people in the map that have to be collected
 
 \discussion People are round objects with green background that need to be collected before exiting the map.
 People have a unique identifier given by a digit. This identifier is their name. People inherit all shape characteristics of Circles, because their have circular shapes. In addition to their name and form people have a confidence level assigned to them indicating how confident the character recognition algorithm was when detecting their name.
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
    
    /// People have names - represented by a digit
    int name;
    /// confidence that name is the correct digit
    int confidence;
    

private:
    const int epsilon_approx = 7;

};

#endif /* People_hpp */
