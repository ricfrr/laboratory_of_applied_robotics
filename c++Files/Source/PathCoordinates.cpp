//
// Created by riccardo on 11/11/2018.
//


#include "../Headers/PathCoordinates.hpp"

using namespace Path2D;

PathCoordinates::PathCoordinates(Position position_in, Position position_fin, double curvature_p) {
    setPositions(position_in, position_fin);
    setMaxCurvature(curvature_p);
};

PathCoordinates::~PathCoordinates() {
    //nada
};

void PathCoordinates::setPositions(Position position_in, Position position_fin) {
    initial_position = position_in;
    final_position = position_fin;
};

Position PathCoordinates::getInitialPosition() {
    return initial_position;
};

Position PathCoordinates::getFinalPosition() {
    return final_position;
};


double PathCoordinates::getMaxCurvature() {
    return Kmax;
};

void PathCoordinates::setMaxCurvature(double curvature_p) {
    Kmax = curvature_p;
};
