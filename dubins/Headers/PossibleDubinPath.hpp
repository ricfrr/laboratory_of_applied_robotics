#ifndef PossiblePath_hpp
#define PossiblePath_hpp

#include <opencv2/core/core.hpp>
#include "PathCoordinates.hpp"

using namespace cv;
/**
 * \brief class that describes a possible dubin path (useful during the calculation)
 */
class PossibleDubinPath {

public:
    /**
     * \brief constructor of PossibleDubinPath class
     */
    PossibleDubinPath();
    /**
     * \brief destructor of PossibleDubinPath class
     */
    ~PossibleDubinPath();

    bool ok;
    double sc_s1;
    double sc_s2;
    double sc_s3;
    std::vector<int> signs;

private:

};

#endif /* PossiblePath_hpp */
