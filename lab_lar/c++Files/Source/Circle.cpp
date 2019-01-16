#include "../Headers/Circle.hpp"

using namespace Geometry2D;

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
