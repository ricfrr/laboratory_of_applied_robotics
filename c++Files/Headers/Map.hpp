#ifndef Map_hpp
#define Map_hpp

#include "Arena.hpp"
#include "Cell.hpp"
#include "ExitPoint.hpp"
#include "Hexagon.hpp"
#include "CustomPolygon.hpp"
#include "Obstacle.hpp"
#include "Pentagon.hpp"
#include "Square.hpp"
#include "Robot.hpp"
#include "People.hpp"
#include "Circle.hpp"
#include "Settings.hpp"
#include <vector>
#include "Digit_Recognition.hpp"
#include "Clipper.hpp"

#include <fstream>


using namespace cv;
using namespace ImageProcessing;
using namespace LAR;

/**
 \brief Class for handling the map
 */
class Map {

public:
    /*!
     * constructor of the Map class
     \param algorithm a DigitRecognitionAlgo enum type that specifies the type of Character_Recognition_Algorithm used to perform the recognition of the digit
     \param suff_confidence confidence level [0-100] the recognized digit is accepted
     \param search_angle region +/- detected angle the algorithm will try to find the digit
     \param d_angle delta_angle step when turning to find the digit
     \param extra_eroding number of consecutive eroding steps, importted for low quality images
     \see Optical_Recognition_Algorithm, Template_Character_Recognition
     */
    Map(const DigitRecognitionAlgo &algorithm = DigitRecognitionAlgo::tesseractOCP, const unsigned int &suff_confidence = 80, const unsigned int &search_angle = 15, const double &d_angle = 3.0, const unsigned int &extra_eroding = 0);

    /*!
     * destructor of the Map class
     */
    ~Map();

    /*!
     * create a map given the a photo of the arena
     * @param img photo of the arena after perspective transformation
     */
    void createMap(const Mat &img,const Mat &robot_plane);

    void getPixelDimensions(int &width, int &height);

    void getArenaCells(std::vector<Cell *> &cells);

    void getGrid(std::vector<std::vector<Cell *>> &grid);
    
    Cell * getCell(cv::Point forPoint);

    LAR::Obstacle getObstacles();

    Digit_Recognition::PeopleStorage getPeople();

    LAR::ExitPoint getExitPoint();


    bool circleContact(std::vector<cv::Point> corners, Circle *circle);

    double distanceBetweenTwoPoints(double x, double y, double a, double b);


    /**
     * \brief return the width of the image
     * @return the width of the image
     */
    int getImageWidth();

    /**
     * \brief return the height of the image
     * @return the height of the image
     */
    int getImageHeight();

    /**
     * \brief return the number of columns in the grid
     * @return the number of columns in the grid
     */
    int getGridColNum();

    /**
     * \brief return the number of rows in the grid
     * @return the number of rows in the grid
     */
    int getGridRowNum();

    std::vector<std::vector<Cell *>> grid;
    
    /// returns 4 vectors with top, right, bottom & left neighboring cells for a cell
    std::vector<std::vector<Cell*>> getNearestNeighbors(Cell * &cell);
    
    /// returns 4 vectors with top, right, bottom & left neighboring empty cells for a cell
    std::vector<std::vector<Cell*>> getEmptyNearestNeighbors(Cell * &cell);
    
    /// returns 4 vectors with top, right, bottom & left neighboring cells for a cell at a point
    std::vector<std::vector<Cell*>> getNearestNeighbors(cv::Point cellInPoint);
    
    /// returns 4 vectors with top, right, bottom & left neighboring center points of empty cells for a cell
    std::vector<std::vector<cv::Point>> getEmptyNearestNeighborsPoints(Cell * &cell);
    
    ///the distance a neighboring cell is given based on cell width/height and a multiplier
    double n_multiplier = 2.0;
    
    ///a flag that indicates that the path planning algorithm should skip possible solution and have a shorter calculation time
    bool quickCalculation = true;
    
    ///settings for camera to real world conversion
    Settings setting;
    
    Robot* robo;
    
    bool wasSuccess();
    
    void save(const std::string &path);
    
    ///returns the startPoint (pixel value) of the arena (upper left corner)
    cv::Point getStartPoint();
    
    ///get the position of the robot
    cv::Point2d getPosition();
    
    void scalePixelsForMap();
    
    std::string findBestFilter(const std::vector<std::string> &filter, const Mat &img);
    
    void setFilterPath(const std::string &imgPath);
    
private:

    // grid of the map
    // create a map with empty cells
    void initializeGrid(Arena &arena, ExitPoint &exit_point, Obstacle &obstacles);

    // detect if a pixel is in contact with an object
    void findMaxMinY(int &max_y, int &min_y, const std::vector<cv::Point> poly);

    bool contact(std::vector<cv::Point> cell, const std::vector<cv::Point> poly);

    bool isOutofArena(std::vector<cv::Point> corners, Arena arena);

    void clipPoints();

    void checkPeople(Cell &cell, Digit_Recognition::PeopleStorage &people);

    void checkObstacles(Cell &cell, Obstacle &obstacles);


    ExitPoint exit_point;
    Obstacle obstacles;
    Arena arena;
    
    Digit_Recognition::PeopleStorage people;

    Clipper clipper;


    int map_pixel_w = setting.IMG_WIDTH;
    int map_pixel_h = setting.IMG_LENGHT;

    int n_row = setting.N_ROW_MAP;
    int n_col = setting.N_COL_MAP;
    
    std::vector<Cell *> getTopNeighbors(Cell* &forCell);
    std::vector<Cell *> getLeftNeighbors(Cell* &forCell);
    std::vector<Cell *> getRightNeighbors(Cell* &forCell);
    std::vector<Cell *> getBottomNeighbors(Cell* &forCell);
    
    bool success = true;
    
    struct MapEncoder {
        
        static void encode(Map* map, const std::string &filepath);
        
        static cv::Point get_real_coordinates(const cv::Point &forPoint, Map* inMap);
    };
};

#endif /* Map_hpp */
