#ifndef Shape_hpp
#define Shape_hpp

#include "Cell.hpp"
#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <atomic>
#include <unistd.h>

using namespace cv;
/**
 \brief class for handling all the shapes in the map
 */
class Shape
{
  public:
    /*!
     * constructor of the shape class
     */
    Shape();
    /*!
     * destructor of the shape class
     */
    ~Shape();
    /*!
     * connect a cell to the shape
     * @param cell_i cell that has to be connected to the shape
     */
    void setCell(Cell & cell_i);
    /*!
     * return the cell which is connected
     * @return the cell which is connected
     */
    std::vector<Cell *> getCell();

  private:
    std::vector<Cell *> cell_l;
};

#endif /* Shape_hpp */
