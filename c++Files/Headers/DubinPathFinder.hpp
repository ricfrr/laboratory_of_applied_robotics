#ifndef DubinPathFinder_hpp
#define DubinPathFinder_hpp

#include "PathFinder.hpp"

namespace Path2D
{
struct standardConf {
    double lambda;
    double sc_th0;
    double sc_thf;
    double sc_Kmax;
};

using namespace cv;
/**
 \brief Class that given the path coordinates allow to find the path using the dubins path algorithm
 */
class DubinPathFinder: public PathFinder {

public:
    /**
      \brief constructor of the DubinPathFinder class
     * @param path_coordinates_i coordinates of the path that we want to find
     */
    DubinPathFinder(PathCoordinates path_coordinates_i,Map* map_i);
    DubinPathFinder(Map* map_i);
    /**
     * \brief destructor of the DubinPathFinder class
     */
    ~DubinPathFinder();
    /**
     * \brief function that find the dubins shortest path and return a vector of line
     * @arg alternative_Points a point set for every obsticle that was found along the way
     * @return vector of lines which describes the path
     */
    std::vector<Line> dubinShortestPath(std::vector<cv::Point> &alternative_Points);
    
    std::vector<Line> shortestPath(std::vector<cv::Point> &alternative_Points);
    
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
        
    };

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


    PathFinder::CollisionDetector collisionDetector;
};
}
#endif /* DubinPathFinder_hpp */
