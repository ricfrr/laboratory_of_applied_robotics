//
//  MissionPlanning.cpp
//  LAR_1_0
//
//  Created by Air Marvin on 14.12.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#include "../Headers/MissionPlanning.hpp"
#include <chrono>

struct {
    bool operator()(People *a, People *b) const {
        return *a < *b;
    }
} customLess;




double dinstance(cv::Point2d p, cv::Point2d o) {
    cv::Point point = p - o;
    return std::sqrt(std::pow(point.x, 2) + std::pow(point.y, 2));
}


MissionPlanning::MissionPlanning() {
    this->map_p = new Map;
    initializePeopleDimensionDinstance();
}

MissionPlanning::MissionPlanning(Map *map) {
    this->map_p = map;
    initializePeopleDimensionDinstance();
}

MissionPlanning::~MissionPlanning() {}

void MissionPlanning::initializePeopleDimensionDinstance() {
    std::vector<Circle *> tmp_circles = map_p->getPeople().getCircles();
    for (int i = 0; i < tmp_circles.size(); i++) {
        people_dimension += tmp_circles[i]->radius;
    }
    people_dimension = people_dimension / tmp_circles.size();
    people_dimension *= 1.2;
};

bool comparePeople(People p1, People p2) {
    return p1.name < p2.name;
}

void MissionPlanning::plan_mission_one() {
    using fsec = std::chrono::duration<float>;
    auto t0 = std::chrono::steady_clock::now();

    std::cout << "\nplanning mission one \n" << std::endl;

    Digit_Recognition::PeopleStorage peops = map_p->getPeople();
    std::vector<People> people_v = peops.people;
    std::vector<People *> people_s;

    for (std::vector<People>::iterator it = people_v.begin(); it != people_v.end(); it++)
        people_s.push_back(&*it);

    std::sort(people_s.begin(), people_s.end(), customLess);

    if (people_s.empty()) {
        std::cout << "no people detected.. cannot plan mission" << std::endl;
        return;
    }

    double or2;
    std::pair<cv::Point, double> exit = map_p->getExitPoint().getEntryPoint();
    PathE2D::Position finalPos = PathE2D::Position(exit.first, exit.second);
    finalPos.orientation_locked = true;

    if (people_s.size() > 1)
        or2 = Geometry::angle_rad(people_s[0]->center, people_s[1]->center);
    else
        or2 = Geometry::angle_rad(people_s[0]->center, finalPos.getCoordinates());

    PathE2D::PathE *path = new PathE2D::PathE(PathE2D::Position(map_p->robo->center, map_p->robo->angle),
                                              PathE2D::Position(people_s[0]->center, or2), this->curvature, map_p);

    for (int i = 1; i < people_s.size(); i++) {

        if (people_s.size() > i + 1)
            or2 = Geometry::angle_rad(people_s[i]->center, people_s[i + 1]->center);

        PathE2D::PathE *p_path = new PathE2D::PathE(path->end_point,
                                                    PathE2D::Position(people_s[i]->center, or2), this->curvature,
                                                    map_p); //fix the orientation wi
        path = new PathE2D::PathE(*path, *p_path);
    }

    PathE2D::PathE *p_path = new PathE2D::PathE(path->end_point,
                                                finalPos, this->curvature, map_p);
    path = new PathE2D::PathE(*path, *p_path);

    std::cout << "---- DONE ----" << std::endl;
    auto t1 = std::chrono::steady_clock::now();
    fsec delta = t1 - t0;
    std::cout << delta.count() << "s\n";

    this->path_p = path;

    Visualizer v(*map_p, path);
    v.visualize();
    //v.simulate();

}

void MissionPlanning::plan_mission_two_fast() {

    using fsec = std::chrono::duration<float>;
    auto t0 = std::chrono::steady_clock::now();

    std::cout << "planning mission two" << std::endl;

    Digit_Recognition::PeopleStorage peops = map_p->getPeople();
    std::vector<People> people_v = peops.people;
    std::vector<PathE2D::Position *> point_of_interest;

    //cv::Point2d start(60, 60);
    //double or1 = 0 * M_PI;

    // add people and end point to the point of interest vector
    for (std::vector<People>::iterator it = people_v.begin(); it != people_v.end(); it++) {
        PathE2D::Position *tmp_position = new Position(it->getCenter());
        //tmp_position->setWeight(5*10*Settings::PIXEL_SCALE); // 5: bonus in second , 10 is the velocity of the robot 10mm/s so 5*10*pixel_scale give me the rewards
        tmp_position->setWeight(5 * 10 * map_p->robo->map_pixelscale);
        point_of_interest.push_back(tmp_position);
    }

    std::pair<cv::Point, double> exit = map_p->getExitPoint().getEntryPoint();
    PathE2D::Position finalPos = PathE2D::Position(exit.first, exit.second);
    finalPos.orientation_locked = true;
    finalPos.setWeight(0 * 10 * map_p->robo->map_pixelscale);
    point_of_interest.push_back(&finalPos);

    PathE2D::PathE *path = new PathE();
    Position *start_point = new Position(map_p->robo->center, map_p->robo->angle);
    int i = 0;
    PathE2D::PathE *path_tmp;
    while (!isExitReached(point_of_interest)) {
        if (i > 0) {
            start_point = &path_tmp->end_point;
        }
        path_tmp = findOptimalFastPathE(start_point, point_of_interest);
        if (i == 0) {
            path = path_tmp;
        } else {
            path = new PathE2D::PathE(*path, *path_tmp);
        }
        std::cout << "iteration " << i << std::endl;
        i++;
    }

    auto t1 = std::chrono::steady_clock::now();
    fsec delta = t1 - t0;
    std::cout << delta.count() << "s\n";
    this->path_p = path;
    std::cout << "-----DONE----" << std::endl;
    Visualizer v(*map_p, path);
    v.visualize();
    //v.simulate();
}


void MissionPlanning::plan_mission_two() {

    using fsec = std::chrono::duration<float>;
    auto t0 = std::chrono::steady_clock::now();

    std::cout << "planning mission two" << std::endl;

    Digit_Recognition::PeopleStorage peops = map_p->getPeople();
    std::vector<People> people_v = peops.people;
    std::vector<PathE2D::Position *> point_of_interest;

    //cv::Point2d start(60, 60);
    //double or1 = 0 * M_PI;

    // add people and end point to the point of interest vector
    for (std::vector<People>::iterator it = people_v.begin(); it != people_v.end(); it++) {
        PathE2D::Position *tmp_position = new Position(it->getCenter());
        //tmp_position->setWeight(5*10*Settings::PIXEL_SCALE); // 5: bonus in second , 10 is the velocity of the robot 10mm/s so 5*10*pixel_scale give me the rewards
        tmp_position->setWeight(5 * 10 * map_p->robo->map_pixelscale);
        point_of_interest.push_back(tmp_position);
    }

    std::pair<cv::Point, double> exit = map_p->getExitPoint().getEntryPoint();
    PathE2D::Position finalPos = PathE2D::Position(exit.first, exit.second);
    finalPos.orientation_locked = true;
    finalPos.setWeight(0 * 10 * map_p->robo->map_pixelscale);
    point_of_interest.push_back(&finalPos);

    PathE2D::PathE *path = new PathE();
    Position *start_point = new Position(map_p->robo->center, map_p->robo->angle);
    int i = 0;
    PathE2D::PathE *path_tmp;
    while (!isExitReached(point_of_interest)) {
        if (i > 0) {
            start_point = &path_tmp->end_point;
        }
        path_tmp = findOptimalPathE(start_point, point_of_interest);
        if (i == 0) {
            path = path_tmp;
        } else {
            path = new PathE2D::PathE(*path, *path_tmp);
        }
        std::cout << "iteration " << i << std::endl;
        i++;
    }

    auto t1 = std::chrono::steady_clock::now();
    fsec delta = t1 - t0;
    std::cout << delta.count() << "s\n";
    this->path_p = path;
    std::cout << "-----DONE----" << std::endl;
    Visualizer v(*map_p, path);
    v.visualize();
    //v.simulate();
}

PathE2D::PathE *
MissionPlanning::findOptimalFastPathE(Position *start_point, std::vector<PathE2D::Position *> &point_of_interests) {
    PathE2D::PathE *path;
    // sort the point of interest vector to make the algorithm faster
    std::sort(point_of_interests.begin(), point_of_interests.end(), [start_point](Position *a, Position *b) {
        return dinstance(a->getCoordinates(), start_point->getCoordinates()) <
               dinstance(b->getCoordinates(), start_point->getCoordinates());
    });
    // loop through all the point of interest
    double orientation;
    if (point_of_interests.size() > 1) {
        orientation = Geometry::angle_rad(point_of_interests[0]->getCoordinates(),
                                          point_of_interests[1]->getCoordinates());
        std::cout << "angle : " << orientation << std::endl;
        point_of_interests[0]->setOrientation(orientation);
        path = new PathE2D::PathE(*start_point, *point_of_interests[0], this->curvature, map_p);
    } else {
        path = new PathE2D::PathE(*start_point, *point_of_interests[0], this->curvature, map_p);
    }
    start_point->setOrientation(orientation);
    // remove all the point of interest picked from the vector
    point_of_interests.erase(point_of_interests.begin());

    return path;
}


PathE2D::PathE *
MissionPlanning::findOptimalPathE(Position *start_point, std::vector<PathE2D::Position *> &point_of_interests) {
    PathE2D::PathE *path;
    PathE2D::PathE *final_path;
    double value = std::numeric_limits<double>::infinity();
    double net_dinstance = std::numeric_limits<double>::infinity();

    double path_value = 0;

    double final_orientation = 0;
    double final_index = 0;

    std::vector<int> intermediate_person_index;

    // sort the point of interest vector to make the algorithm faster
    std::sort(point_of_interests.begin(), point_of_interests.end(), [start_point](Position *a, Position *b) {
        return dinstance(a->getCoordinates(), start_point->getCoordinates()) <
               dinstance(b->getCoordinates(), start_point->getCoordinates());
    });
    // loop through all the point of interest
    for (int i = 0; i < point_of_interests.size(); i++) {
        //check if the path there are real possibilities to obtain a better path
        if (i == 0 ||
            dinstance(start_point->getCoordinates(), point_of_interests[i]->getCoordinates()) <
            net_dinstance + point_of_interests[i]->getWeight()) {
            //loop through all the orientation
            //TODO pick the orientation in the correct way and not in a random way
            for (int j = 0; j < point_of_interests.size(); j++) {
                double temp_orientation;
                // check if the point of interest is the exit point
                if (i != j) {
                    if (!point_of_interests[i]->orientation_locked) {
                        temp_orientation = Geometry::angle_rad(point_of_interests[i]->getCoordinates(),
                                                               point_of_interests[j]->getCoordinates());
                        std::cout << "angle : " << temp_orientation << std::endl;
                        point_of_interests[i]->setOrientation(temp_orientation);
                    } else {
                        j = point_of_interests.size();//to finish the loop immediately when the algorithm check the end point
                    }
                } else if (point_of_interests[i]->orientation_locked) {
                    j = point_of_interests.size();
                } else {
                    continue; // because we don't want to check the orientatio for us
                }

                path = new PathE2D::PathE(*start_point, *point_of_interests[i], this->curvature, map_p);
                path_value = path->length - point_of_interests[i]->getWeight();

                if (value > path_value) {

                    final_path = path;
                    value = path_value;

                    net_dinstance = final_path->length;
                    final_orientation = temp_orientation;
                    final_index = i;
                }

            }
        }
    }

    start_point->setOrientation(final_orientation);
    // remove all the point of interest picked from the vector
    point_of_interests.erase(point_of_interests.begin() + final_index);
    return final_path;
};

// function that find the angle between two points
double angleBetweenPoints(cv::Point2d p1, cv::Point2d p2) {
    double dinst = dinstance(p1, p2);
    double x_dist = abs(p1.x - p2.x);
    return acos(x_dist / dinst);
}

//check if a value is inside the vector
bool isInside(std::vector<int> &intermediate_person_index, int &j) {
    for (int i = 0; i < intermediate_person_index.size(); i++) {
        if (j == intermediate_person_index[i]) {
            return true;
        }
    }
    return false;
}

// check if the path pass near another person and refine it
PathE2D::PathE *
MissionPlanning::passNearAnotherPerson(PathE2D::PathE *path, std::vector<Position *> &point_of_interest, int index,
                                       std::vector<int> &intermediate_person_index) {
    for (int i = 0; i < path->lines.size(); i++) {
        std::vector<cv::Point2d> inter_points = path->lines[i].getIntermediatePoints();
        // loop  through all the intermediate points of the path checking if there is a point that pass near another person
        for (int k = 0; k < inter_points.size(); k++) {
            cv::Point2d tmp_point = inter_points[k];
            for (int j = 0; j < point_of_interest.size(); j++) {

                if (j != index && !point_of_interest[j]->orientation_locked &&
                    !isInside(intermediate_person_index, j) &&
                    dinstance(tmp_point, point_of_interest[j]->getCoordinates()) <
                    people_dimension) {

                    point_of_interest[j]->setOrientation(angleBetweenPoints(point_of_interest[j]->getCoordinates(),
                                                                            path->end_point.getCoordinates()));
                    PathE2D::PathE *tmp_path_1 = new PathE2D::PathE(path->start_point, *point_of_interest[j],
                                                                    this->curvature, map_p);
                    PathE2D::PathE *tmp_path_2 = new PathE2D::PathE(*point_of_interest[j], path->end_point,
                                                                    this->curvature,
                                                                    map_p);
                    PathE2D::PathE *ref = nullptr;
                    intermediate_person_index.push_back(j);
                    ref = passNearAnotherPerson(tmp_path_2, point_of_interest, index, intermediate_person_index);
                    if (ref != nullptr) {
                        tmp_path_2 = ref;
                    }
                    PathE2D::PathE *refined_path = new PathE2D::PathE(*tmp_path_1, *tmp_path_2);
                    return refined_path;
                }
            }
        }
    }
    return nullptr;
}

bool MissionPlanning::isExitReached(std::vector<PathE2D::Position *> &point_of_interests) {
    for (int i = 0; i < point_of_interests.size(); i++) {
        PathE2D::Position *tm_pos = point_of_interests[i];
        if (tm_pos->orientation_locked) {
            return false;
        }
    }
    return true;
}

