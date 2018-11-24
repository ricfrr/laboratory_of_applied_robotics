#ifndef DubinPath_hpp
#define DubinPath_hpp

#include <opencv2/core/core.hpp>
#include <limits>
#include "PathCoordinates.hpp"
#include "PossibleDubinPath.hpp"
#include "Line.hpp"
#include "CircularLine.hpp"
#include "StraightLine.hpp"
#include "Map.hpp"
#include "CollisionDetector.hpp"

using namespace cv;

struct standardConf {
    double lambda;
    double sc_th0;
    double sc_thf;
    double sc_Kmax;
};
/**
 \brief Class that given the path coordinates allow to find the path using the dubins path algorithm
 */
class DubinPath {

public:
    /**
      \brief constructor of the DubinPath class
     * @param path_coordinates_i coordinates of the path that we want to find
     */
    DubinPath(PathCoordinates path_coordinates_i,Map* map_i);
    /**
     * \brief destructor of the DubinPath class
     */
    ~DubinPath();
    /**
     * \brief function that find the dubins shortest path and return a vector of line
     * @return vector of lines which describes the path
     */
    std::vector<Line> dubinShortestPath();

private:
    PathCoordinates path_coordinates = PathCoordinates(Position(), Position(), 0);
    // scale the problem in the standard form
    void scaleToStandard(PathCoordinates path_coordinates);

    std::vector<double> scaleFromStandard(double lambda, double sc_s1, double sc_s2, double sc_s3);
    // create the three lines of the dubins path
    std::vector<Line> dubinsCurve(double x0, double y0, double th0, double s1, double s2, double s3, double k0, double k1, double k2);

    //Normalize an angle (in range [0,2*pi))
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
    Map* map;


    CollisionDetector collisionDetector;
};

#endif /* DubinPath_hpp */
