//
//  Path.hpp
//  
//
//  Created by Air Marvin on 13.11.18.
//

#ifndef Path_hpp
#define Path_hpp

#include <stdio.h>
#include "Geometrie.hpp"

class Path {
    
public:
    Path();
    Path(std::vector<std::pair<double,double>> points);
    ~Path();
    
    /**
     \brief points that are used to construct the path
     */
    std::vector<std::pair<double,double>> points;
    
    /**
     \brief function that computes lines based on the path type and the givent point set
     */
    void fillLines(std::vector<Lines> lines = {}) = 0;
    
    std::vector<Line *> lines;
};

#endif /* Path_hpp */
