#ifndef Cell_hpp
#define Cell_hpp
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>


using namespace cv;

///state of the cell
enum CellState {
    EMPTY,
    MIXED,
    FULL
};

/**
 \brief Cell class for handling each cell of the map
 */
class Cell
{

  public:
    /*!
     * constructor of Cell class
     */
    Cell();
    /*!
     * destructor of Cell class
     */
    ~Cell();

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
     *  return the top left corner of the cell
     * @return return the top left corner of the cell
     */
    cv::Point getTopLeft();
    /*!
     * set the top left corner
     * @param topLeft top left corner that has to be setted
     */
    void setTopLeft(cv::Point topLeft);
    /*!
     * return the top right corner of the cell
     * @return return the top right corner of the cell
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
     * @param bottomLeft bottom left corner of the cell
     */
    void setBottomLeft(cv::Point bottomLeft);
    /*!
     * return the bottom right corner of the cell
     * @return bottom right corner of the cell
     */
    cv::Point getBottomRight();
    /*!
     * set the bottom right corner
     * @param bottomRight bottom right corner of the cell
     */
    void setBottomRight(cv::Point bottomRight);

    /*!
     *
     * @return true if is empty
     */
    bool isEmpty();
    /*!
     *
     * @return true if is an exit
     */
    bool isExit();
    /*!
     *
     * @return true if is a border
     */
    bool isBorder();
    /*!
     *
     * @return true if is an obstacle
     */
    bool isObstacle();
    /*!
     *
     * @return true if is a person to be rescue
     */
    bool isRescue();
    /*!
     *
     * @return if is a person return the digit
     */
    int getDigit();

    /*!
     * set cell to empty
     */
    void set_Empty();
    /*!
     * set cell to exit
     */
    void set_Exit();
    /*!
     * set cell to border
     */
    void set_Border();
    /*!
     * set cell to an obstacle
     */
    void set_Obstacle();
    /*!
     * set the digit of the person
     * @param digit_i digit of the person
     */
    void setRescue(int digit_i);
    
    void findState(std::vector<cv::Point> contour);
    
    void refine_if_neccessary(std::vector<cv::Point> forShape);

    void split(std::vector<cv::Point> forShape = {});
    
    const std::vector<Cell*> getSubcells();
    const std::vector<Cell*> getAllSubcells();
    
    bool contains_object();
    
    
   private: 
    
    void findHalf(double &half_h, double &half_w, const std::vector<cv::Point> &corners);
    bool empty, exit_p, border, obstacle, rescue;
    int digit =0;
    cv::Point top_left, top_right, bottom_left, bottom_right;
    
    std::vector<Cell> subcells;
    CellState state;
    
    void collectSubcells(std::vector<Cell*> &cells);
    
    void state_for_allIn(std::vector<double> results);
    void state_for_allOut(std::vector<cv::Point> contour);
    
    void setEmpty();

    void setExit();

    void setBorder();

    void setObstacle();
};

#endif /* Cell_hpp */
