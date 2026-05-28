#include "Route.h"
#include <iostream>
#include <vector>
#include <chrono>

int main() {
    std::vector<Point> targets;
    targets.push_back({100, 400});
    targets.push_back({500, 50});
    targets.push_back({900, 450});
    
    Point start_pos = {1, 1};
    
    std::vector<Point> obstacles;
    obstacles.push_back({999, 499});

    GRID grid(1000, 500, targets, obstacles);
    AUV VELT(start_pos, 20.0, 8.0);
    
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