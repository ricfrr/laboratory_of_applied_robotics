//
// Created by riccardo on 11/11/2018.
//


#include "../Headers/PathECoordinates.hpp"

using namespace PathE2D;
using namespace Element;

PathECoordinates::PathECoordinates(Position position_in, Position position_fin, double curvature_p) {
    setPositions(position_in, position_fin);
    setMaxCurvature(curvature_p);
};

PathECoordinates::~PathECoordinates() {
    //nada
};

void PathECoordinates::setPositions(Position position_in, Position position_fin) {
    initial_position = position_in;
    final_position = position_fin;
};

Position PathECoordinates::getInitialPosition() {
    return initial_position;
};

Position PathECoordinates::getFinalPosition() {
    return final_position;
};


double PathECoordinates::getMaxCurvature() {
    return Kmax;
};

void PathECoordinates::setMaxCurvature(double curvature_p) {
    Kmax = curvature_p;
};
