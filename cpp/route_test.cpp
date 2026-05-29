#include "Route.h"
#include <iostream>
#include <vector>
#include <chrono>
const double K_units = 20.0;

int main() {
    std::vector<Point> targets;
    targets.push_back({999, 499});
    targets.push_back({500, 250});
    targets.push_back({20, 400});

    double AUV_length = 1.0;
    double AUV_width = 0.4;
    double pool_length = 50.0;
    double pool_width = 25.0;
    
    Point start_pos = {1, 1};
    
    std::vector<Point> obstacles;
    obstacles.push_back({999, 1});

    GRID grid(metres_to_grid_units(pool_length, K_units), metres_to_grid_units(pool_width, K_units), targets, obstacles);
    AUV VELT(start_pos, metres_to_grid_units(AUV_length, K_units),metres_to_grid_units(AUV_width, K_units));
    
    obstacles_inflation(grid.field, grid, obstacles[0], VELT.radius);

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<Point> full_route = VELT.build_full_route(targets, grid);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << duration.count() << std::endl;

    
    if (full_route.empty()) {
        std::cout << "Path Not Found" << std::endl;
    } else {
        std::cout << "X  Y  Angle  Velocity" << std::endl;
        angle_velocity_output(full_route, 20);
    }


    return 0;
}