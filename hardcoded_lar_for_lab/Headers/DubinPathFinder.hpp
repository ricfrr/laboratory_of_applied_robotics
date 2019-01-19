#ifndef DubinPathEFinder_hpp
#define DubinPathEFinder_hpp

#include "PathEFinder.hpp"

namespace PathE2D
{

using namespace cv;
/**
 \brief Class that given the path coordinates allow to find the path using the dubins path algorithm
 */
class DubinPathEFinder: public PathEFinder {

public:
    struct standardConf {
        double lambda;
        double sc_th0;
        double sc_thf;
        double sc_Kmax;
    };
    
public:
    /**
      \brief constructor of the DubinPathEFinder class
     * @param path_coordinates_i coordinates of the path that we want to find
     */
    DubinPathEFinder(PathECoordinates path_coordinates_i,Map* map_i);
    DubinPathEFinder(Map* map_i);
    /**
     * \brief destructor of the DubinPathEFinder class
     */
    ~DubinPathEFinder();
    /**
     * \brief function that find the dubins shortest path and return a vector of line
     * @arg alternative_Points a point set for every obsticle that was found along the way
     * @return vector of lines which describes the path
     */
    std::vector<Line> dubinShortestPathE(std::vector<cv::Point> &alternative_Points);
    
    std::vector<Line> shortestPathE(std::vector<cv::Point> &alternative_Points);
    
    /**
     * \brief class that describes a possible dubin path (useful during the calculation)
     */
    class PossibleDubinPathE {
        
    public:
        /**
         * \brief constructor of PossibleDubinPathE class
         */
        PossibleDubinPathE();
        /**
         * \brief destructor of PossibleDubinPathE class
         */
        ~PossibleDubinPathE();
        
        bool ok;
        double sc_s1;
        double sc_s2;
        double sc_s3;
        std::vector<int> signs;
        
    };

private:
    PathECoordinates path_coordinates = PathECoordinates(Position(), Position(), 0);
    // scale the problem in the standard form
    void scaleToStandard(PathECoordinates path_coordinates);

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

    std::vector<PossibleDubinPathE> possiblePathEs;

    double length;
    standardConf std_conf;
    Map* map;


    PathEFinder::CollisionDetector collisionDetector;
};
}
#endif /* DubinPathEFinder_hpp */
