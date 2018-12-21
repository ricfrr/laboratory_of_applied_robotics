//
//  MissionPlanning.cpp
//  LAR_1_0
//
//  Created by Air Marvin on 14.12.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#include "../Headers/MissionPlanning.hpp"

struct {
    bool operator()(People *a, People *b) const {
        return *a < *b;
    }
} customLess;


double dinstance(cv::Point &p, const cv::Point &o) {
    cv::Point point = p - o;
    return std::sqrt(std::pow(point.x, 2) + std::pow(point.y, 2));
}

MissionPlanning::MissionPlanning() {
    this->map_p = new Map;
}

MissionPlanning::MissionPlanning(Map *map) {
    this->map_p = map;
}

MissionPlanning::~MissionPlanning() {}

bool comparePeople(People p1, People p2) {
    return p1.name < p2.name;
}

void MissionPlanning::plan_mission_one() {

    std::cout << "\nplanning mission one \n" << std::endl;

    Digit_Recognition::PeopleStorage peops = map_p->getPeople();
    std::vector<People> people_v = peops.circles;
    std::vector<People *> people_s;

    for (std::vector<People>::iterator it = people_v.begin(); it != people_v.end(); it++)
        people_s.push_back(&*it);

    std::sort(people_s.begin(), people_s.end(), customLess);

    if (people_s.empty()) {
        std::cout << "no people detected.. cannot plan mission" << std::endl;
        return;
    }

    double or1 = 0 * M_PI;
    double or2 = 1.5 * M_PI;
    std::pair<cv::Point, double> exit = map_p->getExitPoint().getEntryPoint();
    Path2D::Position finalPos = Path2D::Position(exit.first, exit.second);
    finalPos.orientation_locked = true;

    Path2D::Path *path = new Path2D::Path(Path2D::Position(map_p->robo->center, map_p->robo->angle),
                                          Path2D::Position(people_s[0]->center, or2), 0.05, map_p);

    for (int i = 1; i < people_s.size(); i++) {
        Path2D::Path *p_path = new Path2D::Path(path->end_point,
                                                Path2D::Position(people_s[i]->center, or2), 0.05, map_p);
        path = new Path2D::Path(*path, *p_path);
    }

    Path2D::Path *p_path = new Path2D::Path(path->end_point,
                                            finalPos, 0.05, map_p);
    path = new Path2D::Path(*path, *p_path);

    std::cout << "---- DONE ----" << std::endl;

    Visualizer v(*map_p, path);
    v.visualize();
    v.simulate();

}


void MissionPlanning::plan_mission_two() {

    std::cout << "planning mission two" << std::endl;

    Digit_Recognition::PeopleStorage peops = map_p->getPeople();
    std::vector<People> people_v = peops.circles;
    std::vector<Path2D::Position *> point_of_interest;

    cv::Point2d start(60, 60);
    double or1 = 0 * M_PI;

    // add people and end point to the point of interest vector
    for (std::vector<People>::iterator it = people_v.begin(); it != people_v.end(); it++) {
        Path2D::Position *tmp_position = new Position(it->getCenter());
        if (it->name == 4) {
            //tmp_position->setWeight(300);
        }
        if (it->name == 2) {
            tmp_position->setWeight(300);
        }
        if (it->name == 1) {
            tmp_position->setWeight(100);
        }
        point_of_interest.push_back(tmp_position);
    }

    std::pair<cv::Point, double> exit = map_p->getExitPoint().getEntryPoint();
    Path2D::Position finalPos = Path2D::Position(exit.first, exit.second);
    finalPos.orientation_locked = true;
    point_of_interest.push_back(&finalPos);

    Path2D::Path *path = new Path();
    Position *start_point = new Position(start, or1);
    int i = 0;
    Path2D::Path *path_tmp;
    while (!isExitReached(point_of_interest)) {
        if (i>0){
            start_point = &path_tmp->end_point;
        }
        path_tmp = findOptimalPath(start_point, point_of_interest);
        if (i == 0) {
            path = path_tmp;
        } else {
            path = new Path2D::Path(*path, *path_tmp);
        }
        std::cout << "iteration "<<i << std::endl;
        i++;
    }
    if (point_of_interest.empty()) {
        std::cout << "cannot plan mission..." << std::endl;
        return;
    }

    Visualizer v(*map_p, path);
    v.visualize();
    v.simulate();
}

Path2D::Path *
MissionPlanning::findOptimalPath(Position *start_point, std::vector<Path2D::Position *> &point_of_interests) {
    Path2D::Path *path;
    Path2D::Path *final_path;
    double orientation_number = 16.0;
    double value = std::numeric_limits<double>::infinity();
    double path_value = 0;

    double final_orientation = 0;
    double final_index = 0;

    for (int i = 0; i < point_of_interests.size(); i++) {
        for (double j = 0.0; j < orientation_number; j += 1.0) {
            double temp_orientation = (M_PI * j)/ (orientation_number) ;
            if (!point_of_interests[i]->orientation_locked)
                point_of_interests[i]->setOrientation(temp_orientation);
            else
                j = orientation_number;//to finish the loop immediately
            path = new Path2D::Path(*start_point, *point_of_interests[i], 0.05, map_p);
            path_value = path->length - point_of_interests[i]->getWeight();
            if (value > path_value) {
                final_path = path;
                value = path_value;
                final_orientation = temp_orientation;
                final_index = i;
            }
        }
    }
    start_point = point_of_interests[final_index];
    start_point->setOrientation(final_orientation);
    point_of_interests.erase(point_of_interests.begin() + final_index);
    return final_path;
};

bool MissionPlanning::isExitReached(std::vector<Path2D::Position *> &point_of_interests) {

    for (int i = 0; i < point_of_interests.size(); i++) {
        Path2D::Position *tm_pos = point_of_interests[i];
        if (tm_pos->orientation_locked) {
            return false;
        }
    }
    return true;
}

