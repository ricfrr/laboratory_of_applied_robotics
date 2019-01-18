//
// Created by riccardo on 02/12/2018.
//
#include "../Headers/Clipper.hpp"
using namespace ImageProcessing;

Clipper::Clipper() {

};

Clipper::~Clipper() {

};

double distanceEuclidean(cv::Point initial_point, cv::Point final_point) {
    double res;
    res = sqrt(pow(initial_point.x - final_point.x, 2.0) +
               pow(initial_point.y - final_point.y, 2.0));
    return res;
}

std::vector<cv::Point> Clipper::clip(std::vector<cv::Point> &points, int offset) {
    int lenght = points.size() * 2;
    std::vector<cv::Point> clipped_points;
    for (int i = 0; i < lenght; i++) {
        clipped_points.push_back(cv::Point(0, 0));
    }
    for (int i = 0; i < points.size(); i++) {

        cv::Point point_clip;
        cv::Point p1 = points[i];
        cv::Point p2 = points[((i + 1) % points.size())];
        double l = distanceEuclidean(p1, p2);
        double x_distance = abs(p1.x - p2.x);
        double theta = acos(x_distance / l);
        double y_clip;
        double x_clip;

        //se il punto di cui prendo la partenza ha la x piu grande devo mettere il + e non il meno
        //first point
        if (p1.x >= p2.x) {
            x_clip = points[i].x + offset * cos(theta);
        } else {
            x_clip = points[i].x - offset * cos(theta);
        }
        if (p1.y >= p2.y) {
            y_clip = points[i].y + offset * sin(theta);
        } else {
            y_clip = points[i].y - offset * sin(theta);
        }

        point_clip = cv::Point(x_clip, y_clip);

        clipped_points[i * 2] = point_clip;

        if (p1.x >= p2.x) {
            x_clip = points[i].x - (l + offset) * cos(theta);
        } else {
            x_clip = (l + offset) * cos(theta) + points[i].x;
        }
        if (p1.y >= p2.y) {
            y_clip = points[i].y - (l + offset) * sin(theta);
        } else {
            y_clip = (l + offset) * sin(theta) + points[i].y;
        }

        point_clip = cv::Point(round(x_clip), round(y_clip));
        clipped_points[(i * 2 + 3) % lenght] = point_clip;

    }

    return clipped_points;
};

std::vector<cv::Point> Clipper::clipArena(std::vector<cv::Point> &points, int offset) {

    std::vector<cv::Point> clipped_points;
    //we already know the order of the points 0:top left 1: top right 2: bottom right 3:bottom left
    cv::Point top_left = cv::Point(points[0].x+offset,points[0].y+offset);
    cv::Point top_right  = cv::Point(points[1].x-offset,points[1].y+offset);
    cv::Point bottom_right  = cv::Point(points[2].x-offset,points[2].y-offset);
    cv::Point bottom_left = cv::Point(points[3].x+offset,points[3].y-offset);

    clipped_points.push_back(top_left);
    clipped_points.push_back(top_right);
    clipped_points.push_back(bottom_right);
    clipped_points.push_back(bottom_left);

    return clipped_points;
};
