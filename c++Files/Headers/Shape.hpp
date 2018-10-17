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

class Shape
{
  public:
    Shape();
    ~Shape();

    void setCell(Cell cell_i);
    std::vector<Cell> getCell();

  private:
    std::vector<Cell> cell_l;
};

#endif /* Shape_hpp */
