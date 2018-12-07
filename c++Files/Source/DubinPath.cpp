//
// Created by riccardo on 11/11/2018.
//


#include "../Headers/DubinPath.hpp"
#include "../Headers/CircularLine.hpp"


DubinPath::DubinPath(PathCoordinates path_coordinates_i, Map* map_i) {
    path_coordinates = path_coordinates_i;
    map = map_i;
};

DubinPath::~DubinPath() {

};

std::vector<Line> DubinPath::dubinShortestPath(std::vector<cv::Point> &alternative_Points) {
    scaleToStandard(path_coordinates);
    LSL();
    RSR();
    LSR();
    RSL();
    RLR();
    LRL();
    double L = std::numeric_limits<double>::infinity();
    double Lcur = 0;
    PossibleDubinPath path;
    std::vector<Line> lines;
    std::vector<Line> tmp_lines;

    for (int i = 0; i < possiblePaths.size(); i++) {
        Lcur = possiblePaths[i].sc_s1 + possiblePaths[i].sc_s2 + possiblePaths[i].sc_s3;
        if (possiblePaths[i].ok && Lcur < L) {
            path.sc_s1 = possiblePaths[i].sc_s1;
            path.sc_s2 = possiblePaths[i].sc_s2;
            path.sc_s3 = possiblePaths[i].sc_s3;
            path.signs = possiblePaths[i].signs;
            //qua devo checkare la fattibiltà del path facendo la collision detection e per farla
            //mi devo trovare le tre linee quindi scalo e faccio la dubins curve poi da li devo fare la
            // collision detection sui punti che passano che devono essere un punto ogni radius del robot
            std::vector<double> rescaled = scaleFromStandard(std_conf.lambda, path.sc_s1, path.sc_s2, path.sc_s3);
            tmp_lines = dubinsCurve(path_coordinates.getInitialPosition().getCoordinates().x,
                                    path_coordinates.getInitialPosition().getCoordinates().y,
                                    path_coordinates.getInitialPosition().getOrientation(), rescaled[0], rescaled[1],
                                    rescaled[2],
                                    path.signs[0] * path_coordinates.getMaxCurvature(),
                                    path.signs[1] * path_coordinates.getMaxCurvature(),
                                    path.signs[2] * path_coordinates.getMaxCurvature());
            
            std::pair<bool,Cell*> coll = collisionDetector.detectCollision(tmp_lines, map,path_coordinates.getMaxCurvature());
            
            if (!coll.first) {
                std::cout<<"path valido"<<std::endl;
                L = Lcur;
                lines = tmp_lines;
            }
            else{
                //std::cout<<"non path valido\nreturning alternative points"<<std::endl;
                
                std::vector<std::vector<cv::Point>> points = map->getEmptyNearestNeighborsPoints(coll.second);
                
                for(int j=0;j<points.size();j++){
                    alternative_Points.reserve( alternative_Points.size() + points[j].size() ); // preallocate memory
                    alternative_Points.insert( alternative_Points.end(), points[j].begin(), points[j].end() );
                }
                
            }
        }
    }
    return lines;

};

std::vector<Line>
DubinPath::dubinsCurve(double x0, double y0, double th0, double s1, double s2, double s3, double k0, double k1,
                       double k2) {
    // set first arc
    cv::Point2d coord1 = cv::Point2d(x0, y0);
    Position pos1 = Position(coord1, th0);
    std::vector<Line> lines;
    Line arc1;
    if (k0 == 0) {
        arc1 = StraightLine(pos1, s1); //straight line
        lines.push_back(arc1);
    } else {
        arc1 = CircularLine(pos1, k0, s1);
        lines.push_back(arc1);
    }
    //set second arc
    cv::Point2d coord2 = cv::Point2d(arc1.getEndPoint().getCoordinates().x, arc1.getEndPoint().getCoordinates().y);
    Position pos2 = Position(coord2, arc1.getEndPoint().getOrientation());
    Line arc2;
    if (k1 == 0) {
        arc2 = StraightLine(pos2, s2); //straight line
        lines.push_back(arc2);
    } else {
        arc2 = CircularLine(pos2, k1, s2);
        lines.push_back(arc2);
    }
    // set third arc
    cv::Point2d coord3 = cv::Point2d(arc2.getEndPoint().getCoordinates().x, arc2.getEndPoint().getCoordinates().y);
    Position pos3 = Position(coord3, arc2.getEndPoint().getOrientation());
    Line arc3;
    if (k2 == 0) {
        arc3 = StraightLine(pos3, s3); //straight line
        lines.push_back(arc3);
    } else {
        arc3 = CircularLine(pos3, k2, s3);
        lines.push_back(arc3);
    }
    return lines;
}


void DubinPath::scaleToStandard(PathCoordinates path_coordinates) {
    double dx = path_coordinates.getFinalPosition().getCoordinates().x -
                path_coordinates.getInitialPosition().getCoordinates().x;
    double dy = path_coordinates.getFinalPosition().getCoordinates().y -
                path_coordinates.getInitialPosition().getCoordinates().y;
    double phi = atan2(dy, dx);
    double lambda = hypot(dx, dy);

    double C = dx / lambda;
    double S = dy / lambda;

    // arc1 only for the function mod2pi
    lambda = lambda / 2;
    std_conf.sc_th0 = mod2pi(path_coordinates.getInitialPosition().getOrientation() - phi);
    std_conf.sc_thf = mod2pi(path_coordinates.getFinalPosition().getOrientation() - phi);
    std_conf.sc_Kmax = path_coordinates.getMaxCurvature() * lambda;

    std_conf.lambda = lambda;
}

void DubinPath::LSL() {
    PossibleDubinPath possiblePath;
    double invK = 1.0 / std_conf.sc_Kmax;
    double C = cos(std_conf.sc_thf) - cos(std_conf.sc_th0);
    double S = 2.0 * std_conf.sc_Kmax + sin(std_conf.sc_th0) - sin(std_conf.sc_thf);
    double temp1 = atan2(C, S);
    possiblePath.sc_s1 = invK * mod2pi(temp1 - std_conf.sc_th0);
    double temp2 = 2.0 + 4.0 * pow(std_conf.sc_Kmax, 2.0) - 2.0 * cos(std_conf.sc_th0 - std_conf.sc_thf) +
                   4.0 * std_conf.sc_Kmax * (sin(std_conf.sc_th0) - sin(std_conf.sc_thf));
    if (temp2 < 0) {
        possiblePath.ok = false;
        possiblePath.sc_s1 = 0;
        possiblePath.sc_s2 = 0;
        possiblePath.sc_s3 = 0;
        possiblePaths.push_back(possiblePath);
    } else {
        possiblePath.sc_s2 = invK * sqrt(temp2);
        possiblePath.sc_s3 = invK * mod2pi(std_conf.sc_thf - temp1);
        possiblePath.ok = true;
        possiblePath.signs = {1, 0, 1};
        possiblePaths.push_back(possiblePath);
    }

}

void DubinPath::RSR() {
    
    PossibleDubinPath possiblePath;
    
    double invK = 1.0 / std_conf.sc_Kmax;
    double C = cos(std_conf.sc_th0) - cos(std_conf.sc_thf);
    double S = 2.0 * std_conf.sc_Kmax - sin(std_conf.sc_th0) + sin(std_conf.sc_thf);
    double temp1 = atan2(C, S);
    
    possiblePath.sc_s1 = invK * mod2pi(std_conf.sc_th0 - temp1);
    
    double temp2 = 2.0 + 4.0 * pow(std_conf.sc_Kmax, 2.0) - 2.0 * cos(std_conf.sc_th0 - std_conf.sc_thf) -
                   4.0 * std_conf.sc_Kmax * (sin(std_conf.sc_th0) - sin(std_conf.sc_thf));
    if (temp2 < 0) {
        possiblePath.ok = false;
        possiblePath.sc_s1 = 0;
        possiblePath.sc_s2 = 0;
        possiblePath.sc_s3 = 0;
        possiblePaths.push_back(possiblePath);
    } else {
        possiblePath.sc_s2 = invK * sqrt(temp2);
        possiblePath.sc_s3 = invK * mod2pi(temp1 - std_conf.sc_thf);
        possiblePath.ok = true;
        possiblePath.signs = {-1, 0, -1};
        possiblePaths.push_back(possiblePath);
    }
}

void DubinPath::LSR() {
    PossibleDubinPath possiblePath;
    double invK = 1.0 / std_conf.sc_Kmax;
    double C = cos(std_conf.sc_th0) + cos(std_conf.sc_thf);
    double S = 2.0 * std_conf.sc_Kmax + sin(std_conf.sc_th0) + sin(std_conf.sc_thf);
    double temp1 = atan2(-C, S);
    double temp3 = 4.0 * pow(std_conf.sc_Kmax, 2.0) - 2.0 + 2.0 * cos(std_conf.sc_th0 - std_conf.sc_thf) +
                   4.0 * std_conf.sc_Kmax * (sin(std_conf.sc_th0) + sin(std_conf.sc_thf));
    if (temp3 < 0) {
        possiblePath.ok = false;
        possiblePath.sc_s1 = 0;
        possiblePath.sc_s2 = 0;
        possiblePath.sc_s3 = 0;
        possiblePaths.push_back(possiblePath);
    } else {
        possiblePath.sc_s2 = invK * sqrt(temp3);
        double temp2 = -atan2(-2, possiblePath.sc_s2 * std_conf.sc_Kmax);
        possiblePath.sc_s1 = invK * mod2pi(temp1 + temp2 - std_conf.sc_th0);
        possiblePath.sc_s3 = invK * mod2pi(temp1 + temp2 - std_conf.sc_thf);
        possiblePath.ok = true;
        possiblePath.signs = {1, 0, -1};
        possiblePaths.push_back(possiblePath);
    }
}

void DubinPath::RSL() {
    PossibleDubinPath possiblePath;
    double invK = 1.0 / std_conf.sc_Kmax;
    double C = cos(std_conf.sc_th0) + cos(std_conf.sc_thf);
    double S = 2.0 * std_conf.sc_Kmax - sin(std_conf.sc_th0) - sin(std_conf.sc_thf);
    double temp1 = atan2(C, S);
    double temp3 = 4.0 * pow(std_conf.sc_Kmax, 2.0) - 2.0 + 2.0 * cos(std_conf.sc_th0 - std_conf.sc_thf) -
                   4.0 * std_conf.sc_Kmax * (sin(std_conf.sc_th0) + sin(std_conf.sc_thf));
    if (temp3 < 0) {
        possiblePath.ok = false;
        possiblePath.sc_s1 = 0;
        possiblePath.sc_s2 = 0;
        possiblePath.sc_s3 = 0;
        possiblePaths.push_back(possiblePath);
    } else {
        possiblePath.sc_s2 = invK * sqrt(temp3);
        double temp2 = atan2(2, possiblePath.sc_s2 * std_conf.sc_Kmax);
        possiblePath.sc_s1 = invK * mod2pi(std_conf.sc_th0 - temp1 + temp2);
        possiblePath.sc_s3 = invK * mod2pi(std_conf.sc_thf - temp1 + temp2);
        possiblePath.ok = true;
        possiblePath.signs = {-1, 0, 1};
        possiblePaths.push_back(possiblePath);
    }
}

void DubinPath::RLR() {
    PossibleDubinPath possiblePath;
    double invK = 1.0 / std_conf.sc_Kmax;
    double C = cos(std_conf.sc_th0) - cos(std_conf.sc_thf);
    double S = 2.0 * std_conf.sc_Kmax - sin(std_conf.sc_th0) + sin(std_conf.sc_thf);
    double temp1 = atan2(C, S);
    double temp2 = 0.125 * (6.0 - 4.0 * pow(std_conf.sc_Kmax, 2.0) + 2.0 * cos(std_conf.sc_th0 - std_conf.sc_thf) +
                            4.0 * std_conf.sc_Kmax * (sin(std_conf.sc_th0) - sin(std_conf.sc_thf)));
    if (abs(temp2) > 1) {
        possiblePath.ok = false;
        possiblePath.sc_s1 = 0;
        possiblePath.sc_s2 = 0;
        possiblePath.sc_s3 = 0;
        possiblePaths.push_back(possiblePath);
    } else {
        possiblePath.sc_s2 = invK * mod2pi(2 * M_PI - acos(temp2));
        possiblePath.sc_s1 = invK * mod2pi(std_conf.sc_th0 - temp1 + 0.5 * possiblePath.sc_s2 * std_conf.sc_Kmax);
        possiblePath.sc_s3 = invK * mod2pi(std_conf.sc_th0 - std_conf.sc_thf +
                                           std_conf.sc_Kmax * (possiblePath.sc_s2 - possiblePath.sc_s1));
        possiblePath.ok = true;
        possiblePath.signs = {-1, 1, -1};
        possiblePaths.push_back(possiblePath);
    }
}

void DubinPath::LRL() {
    PossibleDubinPath possiblePath;
    double invK = 1. / std_conf.sc_Kmax;
    double C = cos(std_conf.sc_thf) - cos(std_conf.sc_th0);
    double S = 2 * std_conf.sc_Kmax + sin(std_conf.sc_th0) - sin(std_conf.sc_thf);
    double temp1 = atan2(C, S);
    double temp2 = 0.125 * (6 - 4 * pow(std_conf.sc_Kmax, 2.0) + 2 * cos(std_conf.sc_th0 - std_conf.sc_thf) -
                            4 * std_conf.sc_Kmax * (sin(std_conf.sc_th0) - sin(std_conf.sc_thf)));
    if (abs(temp2) > 1) {
        possiblePath.ok = false;
        possiblePath.sc_s1 = 0;
        possiblePath.sc_s2 = 0;
        possiblePath.sc_s3 = 0;
        possiblePaths.push_back(possiblePath);
    } else {
        possiblePath.sc_s2 = invK * mod2pi(2 * M_PI - acos(temp2));
        possiblePath.sc_s1 = invK * mod2pi(temp1 - std_conf.sc_th0 + 0.5 * possiblePath.sc_s2 * std_conf.sc_Kmax);
        possiblePath.sc_s3 = invK * mod2pi(std_conf.sc_thf - std_conf.sc_th0 +
                                           std_conf.sc_Kmax * (possiblePath.sc_s2 - possiblePath.sc_s1));
        possiblePath.ok = true;
        possiblePath.signs = {1, -1, 1};
        possiblePaths.push_back(possiblePath);
    }
}


double DubinPath::mod2pi(double ang) {
    double out = ang;
    while (out < 0.0) {
        out = out + 2.0 * M_PI;
    }
    while (out >= 2.0 * M_PI) {
        out = out - 2.0 * M_PI;
    }
    return out;
};


std::vector<double> DubinPath::scaleFromStandard(double lambda, double sc_s1, double sc_s2, double sc_s3) {
    std::vector<double> rescaled;
    double s1 = sc_s1 * lambda;
    double s2 = sc_s2 * lambda;
    double s3 = sc_s3 * lambda;
    rescaled.push_back(s1);
    rescaled.push_back(s2);
    rescaled.push_back(s3);
    return rescaled;

}

