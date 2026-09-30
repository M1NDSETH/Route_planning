#include "Route.h"
#include "scenario.h"
#include <iostream>
#include <chrono>


int main() {
    auto start = std::chrono::high_resolution_clock::now();
    Scenario s = run_scenario();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << duration.count() << std::endl;

    if (s.path.empty()) {
        std::cout << "Path Not Found" << std::endl;
        return 1;
    }

    angle_velocity_output(s.path, s.max_vel, s.min_vel, s.max_angle_vel, s.min_angle_vel, s.k_units);

    return 0;
}