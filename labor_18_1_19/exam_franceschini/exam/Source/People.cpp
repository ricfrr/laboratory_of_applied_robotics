#include "../Headers/People.hpp"

using namespace LAR;

People::People()
{
    // creator
}
People::~People()
{
    // nada
}

//People::People(People &p){
//    this->name = p.name;
//    this->radius = p.radius;
//    this->center = p.center;
//    this->confidence = p.confidence;
//    this->circles = p.getCircles();
//}

People::People(std::pair<int,int> digit, cv::Rect rect){
    this->name = digit.first;
    this->radius = std::min(rect.height,rect.width)/2;
    this->center = cv::Point(rect.x + rect.width/2, rect.y + rect.height/2);
    this->confidence = digit.second;
}

std::vector<Circle*> People::getCircles() { return circles; };

void People::addCircle(Circle* &circle){
    this->circles.push_back(circle);
}

void People::clearCircles(){
    this->circles.clear();
}
//>>>>>>> pathplanning
