#include "../Headers/CustomPolygon.hpp"

using namespace Geometry2D;

CustomPolygon::CustomPolygon()
{
    // creator
}

CustomPolygon::CustomPolygon(std::vector<cv::Point> points): Polygon(points){
    assign_points();
}

CustomPolygon::~CustomPolygon()
{
    // nada
}

void CustomPolygon::assign_points(){
    setCorners(this->points);
}

void CustomPolygon::setCorners(std::vector<cv::Point> corners)
{
    for(int i =0; i<corners.size(); i++){
        this->corners.push_back(corners[i]);
    }
};

std::vector<cv::Point> CustomPolygon::getCorners()
{
    std::vector<cv::Point> corners;
    for(int i =0; i<this->corners.size(); i++){
        corners.push_back(this->corners[i]);
    }
    return corners;
};


