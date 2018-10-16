#include "../Headers/Circle.hpp"

Circle::Circle()
{
    // creator
}
Circle::~Circle()
{
    // nada
}

int Circle::getRadius()
{
    return radius;
};
void Circle::setRadius(int radius_c)
{
    radius = radius_c;
};
cv::Point Circle::getCenter()
{
    return center;
};
void Circle::setCenter(cv::Point center_c)
{
    center = center_c;
};

int Circle::getDigit()
{
    return digit;
};
void Circle::setDigit(int digit_i)
{
    digit = digit_i;
};
