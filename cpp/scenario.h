#ifndef SCENARIO_H
#define SCENARIO_H

#include <vector>
#include "Route.h"



struct Scenario{
    GRID grid;
    Point start;
    std::vector<Point> targets;
    std::vector<Point> path;

    double k_units;
    double max_vel;
    double min_vel;
    double max_angle_vel;
    double min_angle_vel;
};

Scenario run_scenario();

#endif