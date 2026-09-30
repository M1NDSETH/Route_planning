#include "scenario.h"
#include <cmath>
#include <random>



Scenario run_scenario(){
    const double K_units       = 20.0;
    const double AUV_length    = 1.0;
    const double AUV_width     = 0.4;
    const double pool_length   = 50.0;
    const double pool_width    = 25.0;    
    const double max_vel       = 1.0;
    const double min_vel       = 0.1;
    const double max_angle_vel = 60.0;
    const double min_angle_vel = 5.0;


    Point start_pos = {1, 1};
    std::vector<Point> targets = {
        {999, 499},
        {500, 250},
        {20, 400}
    };
    std::vector<Point> obstacles = {{999, 999}};
    // std::random_device rd;
    // std::mt19937 gen(rd());
    // std::uniform_int_distribution<> dx(1, 999);
    // std::uniform_int_distribution<> dy(1, 499);
    // for (int i = 0; i < 40; ++i) {
    //     obstacles.push_back({dx(gen), dy(gen)});
    // }
    GRID grid(
        static_cast<int>(metres_to_grid_units(pool_length, K_units)),
        static_cast<int>(metres_to_grid_units(pool_width,  K_units)),
        targets,
        obstacles);
    AUV VELT(start_pos,
            metres_to_grid_units(AUV_length, K_units),
            metres_to_grid_units(AUV_width,  K_units),
            metres_to_grid_units(max_vel,    K_units),
            metres_to_grid_units(min_vel,    K_units),
            max_angle_vel,
            min_angle_vel);


    for (const auto & o : obstacles) {
        if (grid.inside(o.x, o.y)) {
            obstacles_inflation(grid.field, grid, o, VELT.radius);
        }
    }


    std::vector<Point> path = VELT.build_full_route(targets, grid);


    return Scenario{
        std::move(grid),
        start_pos,
        std::move(targets),
        std::move(path),
        K_units,
        metres_to_grid_units(max_vel, K_units),
        metres_to_grid_units(min_vel, K_units),
        max_angle_vel,
        min_angle_vel
    };


}