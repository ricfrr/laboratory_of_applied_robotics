#ifndef Map_hpp
#define Map_hpp

#include "Arena.hpp"
#include "Cell.hpp"
#include "ExitPoint.hpp"
#include "Hexagon.hpp"
#include "Obstacle.hpp"
#include "Pentagon.hpp"
#include "Square.hpp"
#include "Triangle.hpp"
#include "People.hpp"
#include "Circle.hpp"
#include "Settings.hpp"
#include <vector>
#include "Digit_Recognition.hpp"


using namespace cv;

/**
 \brief Class for handling the map
 */
class Map {

public:
    /*!
     * constructor of the Map class
     */
    Map();
    /*!
     * destructor of the Map class
     */
    ~Map();
    /*!
     * create a map given the a photo of the arena
     * @param img photo of the arena after perspective transformation
     */
    void createMap(const Mat &img);

private:
    // grid of the map
    // create a map with empty cells
    void initializeGrid(Arena &arena, ExitPoint &exit_point, Obstacle &obstacles);

    // detect if a pixel is in contact with an object
    void findMaxMinY(int &max_y, int &min_y, const std::vector<cv::Point> poly);

    bool contact(std::vector<cv::Point> cell, const std::vector<cv::Point> poly);

    bool isOutofArena(std::vector<cv::Point> corners, Arena arena);

    void checkObstacles(Cell &cell, Obstacle obstacles);

    void checkPeople(Cell &cell, PeopleStorage &people);


    ExitPoint exit_point;
    Obstacle obstacles;
    Arena arena;
    PeopleStorage people;
    Settings setting;

    std::vector<std::vector<Cell>> grid;

    int map_pixel_w = setting.IMG_WIDTH;
    int map_pixel_h = setting.IMG_LENGHT;

    int n_row = setting.N_ROW_MAP;
    int n_col = setting.N_COL_MAP;
};

#endif /* Map_hpp */
