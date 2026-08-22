#include "Route.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
const double K_units = 20.0;

int main() {

    std::random_device rd; 
    std::mt19937 gen(rd());
    int min_x = 1, max_x = 999;
    std::uniform_int_distribution<> distr_x(min_x, max_x);
    int min_y = 1, max_y = 499;
    std::uniform_int_distribution<> distr_y(min_y, max_y);


    std::vector<Point> targets;
    targets.push_back({999, 499});
    targets.push_back({500, 250});
    targets.push_back({20, 400});

    double AUV_length = 1.0;
    double AUV_width = 0.4;
    double pool_length = 50.0;
    double pool_width = 25.0;
    double max_vel = 1.0;
    double min_vel = 0.1;
    
    Point start_pos = {1, 1};
    
    std::vector<Point> obstacles;
    for (int i = 0; i < 40; i++){
        int x = distr_x(gen);
        int y = distr_y(gen);
        Point point = {x, y};
        obstacles.push_back(point);
    }
    
    GRID grid(metres_to_grid_units(pool_length, K_units), metres_to_grid_units(pool_width, K_units), targets, obstacles);
    AUV VELT(start_pos, metres_to_grid_units(AUV_length, K_units), metres_to_grid_units(AUV_width, K_units), metres_to_grid_units(max_vel, K_units), metres_to_grid_units(min_vel, K_units));
    
    for (int i = 0; i < 40; i++){
        obstacles_inflation(grid.field, grid, obstacles[i], VELT.radius);
    }
    

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<Point> full_route = VELT.build_full_route(targets, grid);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << duration.count() << std::endl;

    
    if (full_route.empty()) {
        std::cout << "Path Not Found" << std::endl;
    } else {
        std::cout << "X  Y  Angle  Velocity" << std::endl;
        angle_velocity_output(full_route, VELT.max_velocity, VELT.min_velocity);
    }


    return 0;
}