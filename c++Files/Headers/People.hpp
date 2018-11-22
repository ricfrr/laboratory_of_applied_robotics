#ifndef People_hpp
#define People_hpp

#include "Circle.hpp"
#include "Shape.hpp"
#include "Digit_Recognition.hpp"
#include <vector>

using namespace cv;

/*!
 * class for detecting and saving people
 */
class People : public Shape {

public:
    /*!
     * oonstructor of people class
     */
    People();
    /*!
     * destructor of people class
     */
    ~People();
    /*!
     * detect circles in the map
     * @param img image of the map
     */
    void findCircles(const Mat &img); // return a list of Peoples
    /*!
     * return the list of circle in the map
     * @return list of circle
     */
    std::vector<Circle*> getCircles();
    
    void addCircle(Circle &circle);
    
    void clearCircles();

private:
    const int epsilon_approx = 7;
    std::vector<Circle*> circles;

};

#endif /* People_hpp */
