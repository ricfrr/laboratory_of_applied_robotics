//
// Created by riccardo on 11/11/2018.
//


#include "../Headers/Car.hpp"


Car::Car(Position position_in, Position position_fin, double curvature_p) {
    setPositions(position_in, position_fin);
    setMaxCurvature(curvature_p);
};

Car::~Car() {
    //nada
};

void Car::setPositions(Position position_in, Position position_fin) {
    initial_position = position_in;
    final_position = position_fin;
};

Position Car::getInitialPosition() {
    return initial_position;
};

Position Car::getFinalPosition() {
    return final_position;
};


double Car::getMaxCurvature() {
    return Kmax;
};

void Car::setMaxCurvature(double curvature_p) {
    Kmax = curvature_p;
};