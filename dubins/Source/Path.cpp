

#include "../Headers/Path.hpp"

Path::Path() {};

Path::Path(Position start_point, Position end_point, double curvature) {
    setStartPoint(start_point);
    setEndPoint(end_point);
    setMaxCurvature(curvature);
    findPath();
};

Path::~Path() {};

void Path::setEndPoint(Position end_point) {
    this->end_point = end_point;
}

Position Path::getEndPoint() {
    return end_point;
}

Position Path::getStartPoint() {
    return start_point;
}

void Path::setMaxCurvature(double curvature) {
    maxCurvature = curvature;
}

double Path::getMaxCurvature() {
    return maxCurvature;
}

void Path::setStartPoint(Position start_point) {
    this->start_point = start_point;
}

void Path::setLines(std::vector<Line> lines) {
    this->lines = lines;
}

std::vector<Line> Path::getLines() {
    return lines;
}


void Path::setLength(double length){
    this->length = length;
};
double Path::getLength(){
    return length;
};

void Path::findPath() {
    PathCoordinates path_coordinates = PathCoordinates(start_point, end_point, maxCurvature);
    DubinPath dubin_finder = DubinPath(path_coordinates);
    std::vector<Line> dubin_lines;
    dubin_lines = dubin_finder.dubinShortestPath();
    setLines(dubin_lines);
    double tmp_length;
    for (int i =0; i<dubin_lines.size(); i++){
        tmp_length += dubin_lines[i].getLength();
    }
    setLength(tmp_length);
}



