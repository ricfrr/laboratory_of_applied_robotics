#include "../Headers/Hexagon.hpp"

Hexagon::Hexagon()
{
    // creator
}

Hexagon::Hexagon(std::vector<cv::Point> points): Polygon(points){
    assign_points();
}

Hexagon::~Hexagon()
{
    // nada
}

void Hexagon::assign_points(){
    
    if(this->points.size() != 6)
        std::runtime_error("not a hexagon");
    
    setCorners(this->points);
    
}

void Hexagon::setCorners(std::vector<cv::Point> corners)
{
    Hexagon::setFirstCorner(corners[0]);
    Hexagon::setSecondCorner(corners[1]);
    Hexagon::setThirdCorner(corners[2]);
    Hexagon::setFourthCorner(corners[3]);
    Hexagon::setFifthCorner(corners[4]);
    Hexagon::setSixthCorner(corners[5]);
};

std::vector<cv::Point> Hexagon::getCorners()
{
    std::vector<cv::Point> corners;
    corners.push_back(Hexagon::getFirstCorner());
    corners.push_back(Hexagon::getSecondCorner());
    corners.push_back(Hexagon::getThirdCorner());
    corners.push_back(Hexagon::getFourthCorner());
    corners.push_back(Hexagon::getFifthCorner());
    corners.push_back(Hexagon::getSixthCorner());

    return corners;
};

cv::Point Hexagon::getFirstCorner() { return first; };
void Hexagon::setFirstCorner(cv::Point firstC) { first = firstC; };

cv::Point Hexagon::getSecondCorner() { return second; };
void Hexagon::setSecondCorner(cv::Point secondC) { second = secondC; };

cv::Point Hexagon::getThirdCorner() { return third; };
void Hexagon::setThirdCorner(cv::Point thirdC) { third = thirdC; };

cv::Point Hexagon::getFourthCorner() { return fourth; };
void Hexagon::setFourthCorner(cv::Point fourthC) { fourth = fourthC; };

cv::Point Hexagon::getFifthCorner() { return fifth; };
void Hexagon::setFifthCorner(cv::Point fifthC) { fifth = fifthC; };

cv::Point Hexagon::getSixthCorner() { return sixth; };
void Hexagon::setSixthCorner(cv::Point sixthC) { sixth = sixthC; };
