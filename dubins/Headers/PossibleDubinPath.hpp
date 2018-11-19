#ifndef PossiblePath_hpp
#define PossiblePath_hpp

#include <opencv2/core/core.hpp>
#include "PathCoordinates.hpp"
#include "DubinArc.hpp"

using namespace cv;

class PossibleDubinPath {

public:

    PossibleDubinPath();

    ~PossibleDubinPath();

    bool ok;
    double sc_s1;
    double sc_s2;
    double sc_s3;
    std::vector<int> signs;

private:

};

#endif /* PossiblePath_hpp */
