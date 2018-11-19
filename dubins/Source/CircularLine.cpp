#include "../Headers/CircularLine.hpp"


CircularLine::~CircularLine() {
}

CircularLine::CircularLine(Position start_point, double curvature, double length) {
    setStartPoint(start_point);
    setCurvature(curvature);
    setLength(length);
    Position end_point = findEndPoint(curvature, start_point, length);
    setEndPoint(end_point);
};


void CircularLine::setCurvature(double curvature_i) {
    curvature = curvature_i;
};

double CircularLine::getCurvature() {
    return curvature;
};





