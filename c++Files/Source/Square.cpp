#include "../Headers/Square.hpp"

Square::Square()
{
    // creator
}
Square::~Square()
{
    // nada
}

std::vector<cv::Point> Square::getCorners()
{
    std::vector<cv::Point> corners;
    corners.push_back(Square::getTopLeft());
    corners.push_back(Square::getTopRight());
    corners.push_back(Square::getBottomRight());
    corners.push_back(Square::getBottomLeft());
    return corners;
};
//
void Square::findHalf(int &half_h, int &half_w, const std::vector<cv::Point> &corners)
{
    for (int i = 0; i < corners.size(); i++)
    {
        half_h = half_h + corners[i].y;
        half_w = half_w + corners[i].x;
    }
    half_h = half_h / corners.size();
    half_w = half_w / corners.size();
}

void Square::setCorners(std::vector<cv::Point> corners)
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

cv::Point Square::getTopLeft() { return top_left; };
void Square::setTopLeft(cv::Point topLeft) { top_left = topLeft; };

cv::Point Square::getTopRight() { return top_right; };
void Square::setTopRight(cv::Point topRight) { top_right = topRight; };

cv::Point Square::getBottomLeft() { return bottom_left; };
void Square::setBottomLeft(cv::Point bottomLeft) { bottom_left = bottomLeft; };

cv::Point Square::getBottomRight() { return bottom_right; };
void Square::setBottomRight(cv::Point bottomRight) { bottom_right = bottomRight; };
