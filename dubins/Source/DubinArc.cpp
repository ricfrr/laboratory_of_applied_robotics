//
// Created by riccardo on 11/11/2018.
//

#include "../Headers/DubinArc.hpp"

DubinArc::DubinArc(Position start_pos_i, double curvature_i, double length_i) {
    start_pos = start_pos_i;
    length = length_i;
    curvature = curvature_i;
    circline(curvature, start_pos, length_i);
};

DubinArc::~DubinArc() {
    //nada
};

Position DubinArc::getInitialPosition() {
    return start_pos;
}

Position DubinArc::getFinalPosition() {
    return fin_pos;
}

double DubinArc::getLength() {
    return length;
};

//compute the end point given a initial position lenght and curvature
void DubinArc::circline(double k, Position start, double length) {
    cv::Point2d coordinates;
    double orientation;
    double tmp_sin_val = k * length / 2.0;
    double tmp_cos_val = start.getOrientation() + k * length / 2.0;
    coordinates.x = start.getCoordinates().x + length * sinc(tmp_sin_val) * cos(tmp_cos_val);
    coordinates.y = start.getCoordinates().y + length * sinc(tmp_sin_val) * sin(tmp_cos_val);
    double tmp_orientation = start.getOrientation() + k * length;
    orientation = mod2pi(tmp_orientation);
    //setting final position
    fin_pos.setPosition(coordinates, orientation);
};


// implementation of sinc function
double DubinArc::sinc(double inp) {
    double s;
    if (abs(inp) < 0.002) {
        s = 1 - pow(inp, 2.0) * (1 / 6 - pow(inp, 2.0) / 120);
    } else {
        s = sin(inp) / inp;
    }
    return s;
};

double DubinArc::mod2pi(double ang) {
    double out = ang;
    while (out < 0) {
        out = out + 2 * M_PI;
    }
    while (out >= 2 * M_PI) {
        out = out - 2 * M_PI;
    }
    return out;
};