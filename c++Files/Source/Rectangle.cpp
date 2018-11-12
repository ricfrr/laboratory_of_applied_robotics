#include "../Headers/Rectangle.hpp"

Rectangle::Rectangle()
{
    // creator
}
Rectangle::~Rectangle()
{
    // nada
}

std::vector<cv::Point> Rectangle::getCorners()
{
    std::vector<cv::Point> corners;
    corners.push_back(Rectangle::getTopLeft());
    corners.push_back(Rectangle::getTopRight());
    corners.push_back(Rectangle::getBottomRight());
    corners.push_back(Rectangle::getBottomLeft());
    return corners;
};
//
void Rectangle::findHalf(int &half_h, int &half_w, const std::vector<cv::Point> &corners)
{
    for (int i = 0; i < corners.size(); i++)
    {
        half_h = half_h + corners[i].y;
        half_w = half_w + corners[i].x;
    }
    half_h = half_h / corners.size();
    half_w = half_w / corners.size();
}

void Rectangle::setCorners(std::vector<cv::Point> corners)
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

cv::Point Rectangle::getTopLeft() { return top_left; };
void Rectangle::setTopLeft(cv::Point topLeft) { top_left = topLeft; };

cv::Point Rectangle::getTopRight() { return top_right; };
void Rectangle::setTopRight(cv::Point topRight) { top_right = topRight; };

cv::Point Rectangle::getBottomLeft() { return bottom_left; };
void Rectangle::setBottomLeft(cv::Point bottomLeft) { bottom_left = bottomLeft; };

cv::Point Rectangle::getBottomRight() { return bottom_right; };
void Rectangle::setBottomRight(cv::Point bottomRight) { bottom_right = bottomRight; };
