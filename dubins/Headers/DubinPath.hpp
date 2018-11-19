#ifndef DubinPath_hpp
#define DubinPath_hpp

#include <opencv2/core/core.hpp>
#include <limits>
#include "PathCoordinates.hpp"
#include "DubinArc.hpp"
#include "PossibleDubinPath.hpp"
#include "Line.hpp"
#include "CircularLine.hpp"
#include "StraightLine.hpp"

using namespace cv;

struct standardConf {
    double lambda;
    double sc_th0;
    double sc_thf;
    double sc_Kmax;
};

class DubinPath {

public:

    DubinPath(PathCoordinates path_coordinates_i);

    ~DubinPath();

    std::vector<Line> dubinShortestPath();

private:
    PathCoordinates path_coordinates = PathCoordinates(Position(), Position(), 0);

    void scaleToStandard(PathCoordinates path_coordinates);

    std::vector<double> scaleFromStandard(double lambda, double sc_s1, double sc_s2, double sc_s3);

    std::vector<Line> dubinsCurve(double x0, double y0, double th0, double s1, double s2, double s3, double k0, double k1, double k2);

    //
    double mod2pi(double ang);
    // compute all the possibilities
    void LSL();

    void RSR();

    void LSR();

    void RSL();

    void RLR();

    void LRL();

    std::vector<PossibleDubinPath> possiblePaths;

    double length;
    standardConf std_conf;
};

#endif /* DubinPath_hpp */
