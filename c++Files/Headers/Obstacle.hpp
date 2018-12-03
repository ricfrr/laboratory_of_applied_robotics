#ifndef Obstacle_hpp
#define Obstacle_hpp

#include "Rectangle.hpp"
#include "Square.hpp"
#include "Triangle.hpp"
#include "Pentagon.hpp"
#include "Hexagon.hpp"
#include "Shape.hpp"
#include <vector>

using namespace cv;
/**
 \brief class for handling obstacles in the map
 */
class Obstacle : public Shape {

public:
    /*!
     * contstructor of the Obstacle class
     */
    Obstacle();
    /*!
     * destructor of the Obstacle class
     */
    ~Obstacle();
    /*!
     * retrieve the obstacles given a photo of the map
     * @param img
     */
    void findObstacles(const Mat &img); // return a list of obstacles
    /*!
     * return the list of the triangles obstacles
     * @return list of the triangles obstacles
     */
    std::vector<Triangle *> getTriangles();
    /*!
     * return the list of the squares obstacles
     * @return list of the squares obstacles
     */
    std::vector<Square *> getSquares();
    /*!
     * return the list of the pentagons obstacles
     * @return list of the pentagons obstacles
     */
    std::vector<Pentagon *> getPentagons();
    /*!
     * return the list of the hexagons obstacles
     * @return list of the hexagons obstacles
     */
    std::vector<Hexagon *> getHexagons();

private:
    const int epsilon_approx = 7;
    std::vector<Square> squares;
    std::vector<Triangle> triangles;
    std::vector<Pentagon> pentagons;
    std::vector<Hexagon> hexagons;
};

#endif /* Obstacle_hpp */
