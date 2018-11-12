//
// Created by riccardo on 11/11/2018.
//


#include "../Headers/DubinPath.hpp"


DubinPath::DubinPath(Car car_i) {
    car = car_i;
};

DubinPath::~DubinPath() {

};

void DubinPath::dubinShortestPath() {
    scaleToStandard(car);
    LSL();
    RSR();
    LSR();
    RSL();
    RLR();
    LRL();
    double L = std::numeric_limits<double>::infinity();
    double Lcur = 0;
    PossiblePath path;
    int pidx = -1;
    for (int i = 0; i < possiblePaths.size(); i++) {
        Lcur = possiblePaths[i].sc_s1 + possiblePaths[i].sc_s2 + possiblePaths[i].sc_s3;
        if (possiblePaths[i].ok && Lcur < L) {
            L = Lcur;
            path.sc_s1 = possiblePaths[i].sc_s1;
            path.sc_s2 = possiblePaths[i].sc_s2;
            path.sc_s3 = possiblePaths[i].sc_s3;
            path.signs = possiblePaths[i].signs;
            pidx = i + 1;
        }
    }

    if (pidx > 0) {
        std::vector<double> rescaled = scaleFromStandard(std_conf.lambda, path.sc_s1, path.sc_s2, path.sc_s3);
        dubinsCurve(car.getInitialPosition().getCoordinates().x, car.getInitialPosition().getCoordinates().y,
                    car.getInitialPosition().getOrientation(), rescaled[0], rescaled[1], rescaled[2],
                    path.signs[0] * car.getMaxCurvature(), path.signs[1] * car.getMaxCurvature(),
                    path.signs[2] * car.getMaxCurvature());
        // check the correctness of the algorithm
        //TODO
    }

};

void DubinPath::dubinsCurve(double x0, double y0, double th0, double s1, double s2, double s3, double k0, double k1,
                            double k2) {
    // set first arc
    cv::Point2d coord1 = cv::Point2d(x0, y0);
    Position pos1 = Position(coord1, th0);
    arc1 = DubinArc(pos1,k0,s1);
    //set second arc
    cv::Point2d coord2 = cv::Point2d(arc1.getFinalPosition().getCoordinates().x, arc1.getFinalPosition().getCoordinates().y);
    Position pos2 = Position(coord2, arc1.getFinalPosition().getOrientation());
    arc2 = DubinArc(pos2, k1, s2);
    // set third arc
    cv::Point2d coord3 = cv::Point2d(arc2.getFinalPosition().getCoordinates().x, arc2.getFinalPosition().getCoordinates().y);
    Position pos3 = Position(coord3, arc2.getFinalPosition().getOrientation());
    arc3 = DubinArc(pos3, k2, s3);
    // set length
    length = arc1.getLength() + arc2.getLength() + arc3.getLength();
}


void DubinPath::scaleToStandard(Car car) {
    double dx = car.getFinalPosition().getCoordinates().x - car.getInitialPosition().getCoordinates().x;
    double dy = car.getFinalPosition().getCoordinates().y - car.getInitialPosition().getCoordinates().y;
    double phi = atan2(dy, dx);
    double lambda = hypot(dx, dy);

    double C = dx / lambda;
    double S = dy / lambda;

    // arc1 only for the function mod2pi
    lambda = lambda / 2;
    std_conf.sc_th0 = mod2pi(car.getInitialPosition().getOrientation() - phi);
    std_conf.sc_thf = mod2pi(car.getFinalPosition().getOrientation() - phi);
    std_conf.sc_Kmax = car.getMaxCurvature() * lambda;

    std_conf.lambda = lambda;
}

void DubinPath::LSL() {
    PossiblePath possiblePath;
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
    PossiblePath possiblePath;
    double invK = 1.0 / std_conf.sc_Kmax;
    double C = cos(std_conf.sc_th0) - cos(std_conf.sc_thf);
    double S = 2.0 * std_conf.sc_Kmax + sin(std_conf.sc_th0) - sin(std_conf.sc_thf);
    double temp1 = atan2(C, S);
    possiblePath.sc_s1 = invK * mod2pi(std_conf.sc_th0 - temp1);
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
        possiblePath.sc_s3 = invK * mod2pi(temp1 - std_conf.sc_thf);
        possiblePath.ok = true;
        possiblePath.signs = {-1, 0, -1};
        possiblePaths.push_back(possiblePath);
    }
}

void DubinPath::LSR() {
    PossiblePath possiblePath;
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
    PossiblePath possiblePath;
    double invK = 1 / std_conf.sc_Kmax;
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
        double temp2 = -atan2(2, possiblePath.sc_s2 * std_conf.sc_Kmax);
        possiblePath.sc_s1 = invK * mod2pi(std_conf.sc_th0 - temp1 + temp2);
        possiblePath.sc_s3 = invK * mod2pi(std_conf.sc_thf - temp1 + temp2);
        possiblePath.ok = true;
        possiblePath.signs = {-1, 0, 1};
        possiblePaths.push_back(possiblePath);
    }
}

void DubinPath::RLR() {
    PossiblePath possiblePath;
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
        possiblePath.sc_s2 = invK * mod2pi(2 * PI - acos(temp2));
        possiblePath.sc_s1 = invK * mod2pi(std_conf.sc_th0 - temp1 + 0.5 * possiblePath.sc_s2 * std_conf.sc_Kmax);
        possiblePath.sc_s3 = invK * mod2pi(std_conf.sc_th0 - std_conf.sc_thf +
                                           std_conf.sc_Kmax * (possiblePath.sc_s2 - possiblePath.sc_s1));
        possiblePath.ok = true;
        possiblePath.signs = {-1, 1, -1};
        possiblePaths.push_back(possiblePath);
    }
}

void DubinPath::LRL() {
    PossiblePath possiblePath;
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
        possiblePath.sc_s2 = invK * mod2pi(2 * PI - acos(temp2));
        possiblePath.sc_s1 = invK * mod2pi(temp1 - std_conf.sc_th0 + 0.5 * possiblePath.sc_s2 * std_conf.sc_Kmax);
        possiblePath.sc_s3 = invK * mod2pi(std_conf.sc_thf - std_conf.sc_th0 +
                                           std_conf.sc_Kmax * (possiblePath.sc_s2 - possiblePath.sc_s1));
        possiblePath.ok = true;
        possiblePath.signs = {1, -1, 1};
        possiblePaths.push_back(possiblePath);
    }
}


double DubinPath::mod2pi(double ang) {
    double out=ang;
    while (out < 0.0) {
        out = out + 2.0 * PI;
    }
    while (out >= 2.0 * PI) {
        out = out - 2.0 * PI;
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

