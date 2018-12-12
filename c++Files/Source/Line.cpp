

#include "../Headers/Line.hpp"

using namespace Path2D;
using namespace Element;

Line::Line() {}

Line::Line(Position start_point, Position end_point) {
    setStartPoint(start_point);
    setEndPoint(end_point);
}

Line::~Line() {}


void Line::setStartPoint(Position start_point_i) {
    start_point = start_point_i;
};

Position Line::getStartPoint() {
    return start_point;
};

void Line::setEndPoint(Position end_point_i) {
    end_point = end_point_i;
};

Position Line::getEndPoint() {
    return end_point;
};

void Line::setLength(double length_i) {
    length = length_i;
};

double Line::getLength() {
    return length;
};


//compute the end point given a initial position lenght and curvature curvature = 0 straight line
Position Line::findPointDistance(double k, Position start, double length) {
    cv::Point2d coordinates;
    double fin_orientation;
    double tmp_sin_val = k * length / 2.0;
    double tmp_cos_val = start.getOrientation() + k * length / 2.0;
    coordinates.x = start.getCoordinates().x + length * sinc(tmp_sin_val) * cos(tmp_cos_val);
    coordinates.y = start.getCoordinates().y + length * sinc(tmp_sin_val) * sin(tmp_cos_val);
    double tmp_orientation = start.getOrientation() + k * length;
    fin_orientation = mod2pi(tmp_orientation);
    //setting final position
    Position tmp_fin_pos;
    tmp_fin_pos.setPosition(coordinates, fin_orientation);
    return tmp_fin_pos;
};

void Line::setCurvature(double curvature_i) {
    curvature = curvature_i;
};

double Line::getCurvature() {
    return curvature;
};



// implementation of sinc function
double Line::sinc(double inp) {
    double s;
    if (abs(inp) < 0.002) {
        s = 1 - pow(inp, 2.0) * (1 / 6 - pow(inp, 2.0) / 120);
    } else {
        s = sin(inp) / inp;
    }
    return s;
};



double Line::mod2pi(double ang) {
    double out = ang;
    while (out < 0) {
        out = out + 2 * M_PI;
    }
    while (out >= 2 * M_PI) {
        out = out - 2 * M_PI;
    }
    return out;
}

void Line::setIntermediatePoints(std::vector<Point2d> &intermediate_points) {
    this->intermediate_points = intermediate_points;
}

std::vector<Point2d> Line::getIntermediatePoints() {
    return this->intermediate_points;
};
