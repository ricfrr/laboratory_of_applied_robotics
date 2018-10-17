#include "../Headers/Cell.hpp"

Cell::Cell()
{
    // creator
}
Cell::~Cell()
{
    // nada
}

std::vector<cv::Point> Cell::getCorners()
{
    std::vector<cv::Point> corners;
    corners.push_back(Cell::getTopLeft());
    corners.push_back(Cell::getTopRight());
    corners.push_back(Cell::getBottomRight());
    corners.push_back(Cell::getBottomLeft());
    return corners;
};
//
void Cell::findHalf(int &half_h, int &half_w, const std::vector<cv::Point> &corners)
{
    for (int i = 0; i < corners.size(); i++)
    {
        half_h = half_h + corners[i].y;
        half_w = half_w + corners[i].x;
    }
    half_h = half_h / corners.size();
    half_w = half_w / corners.size();
}

void Cell::setCorners(std::vector<cv::Point> corners)
{
    int half_h = 0;
    int half_w = 0;
    findHalf(half_h, half_w, corners);

    for (int i = 0; i < corners.size(); i++)
    {
        if (corners[i].x < half_w)
        {
            if (corners[i].y < half_h)
            {
                setTopLeft(corners[i]);
            }
            else
            {
                setBottomLeft(corners[i]);
            }
        }
        else
        {
            if (corners[i].y < half_h)
            {
                setTopRight(corners[i]);
            }
            else
            {
                setBottomRight(corners[i]);
            }
        }
    }
};

cv::Point Cell::getTopLeft() { return top_left; };
void Cell::setTopLeft(cv::Point topLeft) { top_left = topLeft; };

cv::Point Cell::getTopRight() { return top_right; };
void Cell::setTopRight(cv::Point topRight) { top_right = topRight; };

cv::Point Cell::getBottomLeft() { return bottom_left; };
void Cell::setBottomLeft(cv::Point bottomLeft) { bottom_left = bottomLeft; };

cv::Point Cell::getBottomRight() { return bottom_right; };
void Cell::setBottomRight(cv::Point bottomRight) { bottom_right = bottomRight; };


// set content of cells 
void Cell::setEmpty()
{
    empty = true;
    exit_p = false;
    border = false;
    obstacle = false;
    rescue = false;
};
void Cell::setExit()
{
    empty = false;
    exit_p = true;
    border = false;
    obstacle = false;
    rescue = false;
};
void Cell::setBorder()
{
    empty = false;
    exit_p = false;
    border = true;
    obstacle = false;
    rescue = false;
};
void Cell::setObstacle()
{
    empty = false;
    exit_p = false;
    border = false;
    obstacle = true;
    rescue = false;
};

int Cell::getDigit(){
    return digit;
};
void Cell::setRescue(int digit_i)
{
    empty = false;
    exit_p = false;
    border = false;
    obstacle = false;
    rescue = true;
    digit = digit_i;
};

bool Cell::isEmpty()
{
    return empty;
};
bool Cell::isExit()
{
    return exit_p;
};
bool Cell::isBorder()
{
    return border;
};
bool Cell::isObstacle()
{
    return obstacle;
};
bool Cell::isRescue()
{
    return rescue;
}
