//
//  Character_Recognition_Algorithm.hpp
//  LAR_1_0
//
//  Created by Air Marvin on 10.10.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#ifndef Character_Recognition_Algorithm_hpp
#define Character_Recognition_Algorithm_hpp

#include <stdio.h>
#include <string>

///pure abstract class for character recognition algorithms
class Character_Recognition_Algorithm {

public:
    Character_Recognition_Algorithm(){}
    ~Character_Recognition_Algorithm(){}
    
    //pure virtual function, to run the demo like the public code gave us
    ///a demo function displaying the performance of a given algorithm
    virtual void processImage(const std::string& filename) = 0;
};

#endif /* Character_Recognition_Algorithm_hpp */
