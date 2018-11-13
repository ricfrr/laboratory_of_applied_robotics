//
//  Path.cpp
//  
//
//  Created by Air Marvin on 13.11.18.
//

#include "Path.hpp"

Path::Path(){}

Path::Path(std::vector<std::pair<double,double>> points){
    this->points = points;
    
    //not sure if I can call that method here because its abstract
    fillLines();
}

Path::~Path(){}
