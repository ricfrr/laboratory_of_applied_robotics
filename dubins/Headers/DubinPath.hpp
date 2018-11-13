#ifndef DubinPath_hpp
#define DubinPath_hpp

#include <opencv2/core/core.hpp>
#include <limits>
#include "Car.hpp"
#include "DubinArc.hpp"
#include "PossiblePath.hpp"

using namespace cv;

struct standardConf {
    double lambda;
    double sc_th0;
    double sc_thf;
    double sc_Kmax;
};

class DubinPath {

public:

    DubinPath(Car car_i);

    ~DubinPath();

    void dubinShortestPath();

private:
    Car car = Car(Position(), Position(), 0);

    void scaleToStandard(Car car);

    std::vector<double> scaleFromStandard(double lambda, double sc_s1, double sc_s2, double sc_s3);

    void dubinsCurve(double x0, double y0, double th0, double s1, double s2, double s3, double k0, double k1, double k2);

    //
    double mod2pi(double ang);

    double PI = M_PI;

    // compute all the possibilities
    void LSL();

    void RSR();

    void LSR();

    void RSL();

    void RLR();

    void LRL();

    std::vector<PossiblePath> possiblePaths;

    DubinArc arc1 = DubinArc(Position(), 0, 0);
    DubinArc arc2 = DubinArc(Position(), 0, 0);
    DubinArc arc3 = DubinArc(Position(), 0, 0);
    double length;
    standardConf std_conf;
};

#endif /* DubinPath_hpp */
