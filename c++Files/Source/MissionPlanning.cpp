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


double dinstance(cv::Point &p, cv::Point &o) {
    cv::Point point = p - o;
    return std::sqrt(std::pow(point.x, 2) + std::pow(point.y, 2));
}

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
    
    this->path_p = path;

    Visualizer v(*map_p, path);
    v.visualize();
    v.simulate();

}


void MissionPlanning::plan_mission_two() {

    using fsec = std::chrono::duration<float>;
    auto t0 = std::chrono::steady_clock::now();

    std::cout << "planning mission two" << std::endl;

    Digit_Recognition::PeopleStorage peops = map_p->getPeople();
    std::vector<People> people_v = peops.people;
    std::vector<Path2D::Position *> point_of_interest;

    cv::Point2d start(60, 60);
    double or1 = 0 * M_PI;

    // add people and end point to the point of interest vector
    for (std::vector<People>::iterator it = people_v.begin(); it != people_v.end(); it++) {
        Path2D::Position *tmp_position = new Position(it->getCenter());
        if (it->name == 4) {
            tmp_position->setWeight(300);
        }
        if (it->name == 2) {
            tmp_position->setWeight(200);
        }
        if (it->name == 1) {
            tmp_position->setWeight(100);
        }
        if (it->name == 3) {
            tmp_position->setWeight(0);
        }
        if (it->name == 5) {
            tmp_position->setWeight(300);
        }
        point_of_interest.push_back(tmp_position);
    }

    std::pair<cv::Point, double> exit = map_p->getExitPoint().getEntryPoint();
    Path2D::Position finalPos = Path2D::Position(exit.first, exit.second);
    finalPos.orientation_locked = true;
    finalPos.setWeight(100);
    point_of_interest.push_back(&finalPos);

    Path2D::Path *path = new Path();
    Position *start_point = new Position(start, or1);
    int i = 0;
    Path2D::Path *path_tmp;
    while (!isExitReached(point_of_interest)) {
        if (i > 0) {
            start_point = &path_tmp->end_point;
        }
        path_tmp = findOptimalPath(start_point, point_of_interest);
        if (i == 0) {
            path = path_tmp;
        } else {
            path = new Path2D::Path(*path, *path_tmp);
        }
        std::cout << "iteration " << i << std::endl;
        i++;
    }

    auto t1 = std::chrono::steady_clock::now();
    fsec delta = t1 - t0;
    std::cout << delta.count() << "s\n";

    std::cout << "-----DONE----" << std::endl;
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
            for (double j = 0.0; j < orientation_number; j += 1.0) {

                double temp_orientation = (M_PI * j) / (orientation_number);
                std::cout << "angle : " << temp_orientation << std::endl;
                // check if the point of interest is the exit point
                if (!point_of_interests[i]->orientation_locked)
                    point_of_interests[i]->setOrientation(temp_orientation);
                else
                    j = orientation_number;//to finish the loop immediately when the algorithm check the end point

                path = new Path2D::Path(*start_point, *point_of_interests[i], 0.05, map_p);
                path_value = path->length - point_of_interests[i]->getWeight();
                // if the path that we have found is better than the one that we already have, update the parameters
                if (value > path_value) {
                    //refine the path checking if the path pass near another person
                    Path2D::Path *refined_path = nullptr;
                    intermediate_person_index.erase(intermediate_person_index.begin(), intermediate_person_index.end());
                    refined_path = passNearAnotherPerson(path, point_of_interests, i, intermediate_person_index);
                    if (refined_path != nullptr) {
                        path = refined_path;
                        final_path = path;
                        int intermediate_weight = 0;
                        for (int int_dim = 0; int_dim < intermediate_person_index.size(); int_dim++)
                            intermediate_weight += intermediate_person_index[int_dim];
                        value = final_path->length - point_of_interests[i]->getWeight() - intermediate_weight;
                    } else {
                        final_path = path;
                        value = path_value;
                    }

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
    for (int k = 0; k < intermediate_person_index.size(); k++) {
        if (final_index < intermediate_person_index[k])
            intermediate_person_index[k]--;
        point_of_interests.erase(point_of_interests.begin() + intermediate_person_index[k]);
    }

    return final_path;
};

// function that found the angle between two points
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
Path2D::Path *
MissionPlanning::passNearAnotherPerson(Path2D::Path *path, std::vector<Position *> &point_of_interest, int index,
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
                    Path2D::Path *tmp_path_1 = new Path2D::Path(path->start_point, *point_of_interest[j], 0.05, map_p);
                    Path2D::Path *tmp_path_2 = new Path2D::Path(*point_of_interest[j], path->end_point, 0.05, map_p);
                    Path2D::Path *ref = nullptr;
                    intermediate_person_index.push_back(j);
                    ref = passNearAnotherPerson(tmp_path_2, point_of_interest, index, intermediate_person_index);
                    if (ref != nullptr) {
                        tmp_path_2 = ref;
                    }
                    Path2D::Path *refined_path = new Path2D::Path(*tmp_path_1, *tmp_path_2);
                    return refined_path;
                }
            }
        }
    }
    return nullptr;
}

bool MissionPlanning::isExitReached(std::vector<Path2D::Position *> &point_of_interests) {
    for (int i = 0; i < point_of_interests.size(); i++) {
        Path2D::Position *tm_pos = point_of_interests[i];
        if (tm_pos->orientation_locked) {
            return false;
        }
    }
    return true;
}

