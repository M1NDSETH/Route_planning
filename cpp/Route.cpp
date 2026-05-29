#include "Route.h"
#include <cmath>
#include <vector>
#include <queue>
#include <algorithm>
#include <iostream>

int max(int a, int b) {
    return (a < b) ? b : a;
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

int heuristic(int x0, int y0, int x1, int y1) {
    return (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0);
}

void obstacles_inflation(std::vector<uint8_t>& field, GRID grid, Point center, int radius) {
    for (int x = center.x - radius; x <= center.x + radius; x++) {
        for (int y = center.y - radius; y <= center.y + radius; y++) {
            if (!grid.inside(x, y)) {
                continue;
            }
            if (heuristic(center.x, center.y, x, y) <= radius * radius) {
                field[grid.index(x, y)] = 1;
            }
        }
    }
}

std::vector<Point> theta_star(Point start, Point target, GRID grid, std::vector<uint8_t> temp_field) {
    int size = grid.x_size * grid.y_size;
    std::vector<double> g_score(size, INFINITY);
    std::vector<uint8_t> closed_set(size, 0);
    std::vector<Point> parents(size);

    using QueueNode = std::pair<double, Point>;
    auto compare = [](const QueueNode& a, const QueueNode& b) {
        return a.first > b.first;
    };
    std::priority_queue<QueueNode, std::vector<QueueNode>, decltype(compare)> open(compare);
    
    int start_index = grid.index(start);
    g_score[start_index] = 0;
    parents[start_index] = start;
    open.push({sqrt(heuristic(start.x, start.y, target.x, target.y)), start});

    const int directions[8][2] = {
        {-1,  0}, { 1,  0}, { 0, -1}, { 0,  1},
        {-1, -1}, {-1,  1}, { 1, -1}, { 1,  1}
    };

    while (!open.empty()) {
        Point current = open.top().second;
        open.pop();
        int current_index = grid.index(current);
        if (closed_set[current_index]) {
            continue;
        }
        closed_set[current_index] = 1;

        if (current.x == target.x && current.y == target.y) {
            std::vector<Point> path;
            Point p = target;
            while (!(p.x == start.x && p.y == start.y)) {
                path.push_back(p);
                p = parents[grid.index(p)];
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (int i = 0; i < 8; i++) {
            Point neighbor{current.x + directions[i][0], current.y + directions[i][1]};
            if (!grid.inside(neighbor.x, neighbor.y)) {
                continue;
            }
            int neighbor_index = grid.index(neighbor);
            if (grid.field[neighbor_index] == 1 || temp_field[neighbor_index] == 1) {
                continue;
            }
            if (!grid.valid_move(current, neighbor)) {
                continue;
            }

            Point current_parent = parents[current_index];
            double new_g;
            if (grid.line_of_sight(current_parent, neighbor, temp_field)) {
                new_g = g_score[grid.index(current_parent)] + sqrt(heuristic(current_parent.x, current_parent.y, neighbor.x, neighbor.y));
                if (new_g < g_score[neighbor_index]) {
                    g_score[neighbor_index] = new_g;
                    parents[neighbor_index] = current_parent;
                    double f = new_g + sqrt(heuristic(neighbor.x, neighbor.y, target.x, target.y));
                    open.push({f, neighbor});
                }
            } else {
                new_g = g_score[current_index] + sqrt(heuristic(current.x, current.y, neighbor.x, neighbor.y));
                if (new_g < g_score[neighbor_index]) {
                    g_score[neighbor_index] = new_g;
                    parents[neighbor_index] = current;
                    double f = new_g + sqrt(heuristic(neighbor.x, neighbor.y, target.x, target.y));
                    open.push({f, neighbor});
                }
            }
        }
    }
    return {};
}

GRID::GRID(int grid_x_size, int grid_y_size, std::vector<Point> grid_targets, std::vector<Point> grid_obstacles) {
    x_size = grid_x_size;
    y_size = grid_y_size;
    targets = grid_targets;
    obstacles = grid_obstacles;
    field.resize(grid_x_size * grid_y_size, 0);
}

bool GRID::valid_move(Point current, Point neighbor) {
    int x0 = current.x;
    int y0 = current.y;
    int x1 = neighbor.x;
    int y1 = neighbor.y;

    if ((x1 - x0 != 0) && (y1 - y0 != 0)) {
        if ((field[index(x1, y0)] == 1) || (field[index(x0, y1)] == 1)) {
            return false;
        }
    }
    return true;
}

bool GRID::line_of_sight(Point parent, Point neighbor, std::vector<uint8_t> clean_field) {
    std::vector<Point> cells = bresenham(parent, neighbor);
    for (size_t i = 0; i < cells.size(); i++) {
        if (inside(cells[i].x, cells[i].y)) {
            if (clean_field[index(cells[i])] == 1 || field[index(cells[i])] == 1) {
                return false;
            }
        } else {
            return false;
        }
    }
    return true;
}

std::vector<Point> AUV::build_full_route(std::vector<Point> targets, GRID grid) {
    std::vector<Point> full_path;
    Point current = start_point;
    for (size_t i = 0; i < targets.size(); i++) {
        Point current_target = targets[i];
        std::vector<uint8_t> temp_field(grid.x_size * grid.y_size, 0);
        for (size_t j = i + 1; j < targets.size(); j++) {
            obstacles_inflation(temp_field, grid, targets[j], radius);
        }
        std::vector<Point> path_segment = theta_star(current, current_target, grid, temp_field);
        if (path_segment.empty()) {
            return {};
        }
        if (!full_path.empty()) {
            full_path.insert(full_path.end(), path_segment.begin() + 1, path_segment.end());
        } else {
            full_path.insert(full_path.end(), path_segment.begin(), path_segment.end());
        }
        current = current_target;
    }
    return full_path;
}

// Конструктор с приведением типов и округлением вверх (ceil)
AUV::AUV(Point start, double length, double weight) {
    start_point = start;
    // std::ceil округлит, например, 10.1 до 11.0, чтобы радиус робота брался с безопасным запасом
    radius = static_cast<int>(std::ceil(0.5 * sqrt(length * length + weight * weight)));
}

std::vector<Point> bresenham(Point start, Point finish) {
    std::vector<Point> cells;
    int dx = abs(finish.x - start.x);
    int dy = abs(finish.y - start.y);
    int Sx = (start.x < finish.x) ? 1 : -1;
    int Sy = (start.y < finish.y) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        cells.push_back({start.x, start.y});
        if (start.x == finish.x && start.y == finish.y) {
            break;
        }
        int local_err = err * 2;
        if (local_err > -dy) {
            err -= dy;
            start.x += Sx;
        }
        if (local_err < dx) {
            err += dx;
            start.y += Sy;
        }
    }
    return cells;
}

void angle_velocity_output(std::vector<Point> path, int velocity){
    size_t i = 0;
    double current = 0;
    std::cout << "Point               " << "       Angle" << "    Velocity" << std::endl;

    while (i < path.size() - 1) { 
        double dx = static_cast<double>(path[i + 1].x - path[i].x);
        double dy = static_cast<double>(path[i + 1].y - path[i].y);
        
        if (i == 0) {
            if (dx == 0) {
                current = 90.0 * (dy / std::abs(dy));
            } else {
                current = std::atan(dy / dx) * (-1.0) * 180.0 / M_PI;
            }
            
            std::cout << path[i + 1].x << ", " << path[i + 1].y << "         " << current << "           " << velocity << std::endl;
            i++;
            continue;
        } 
        else {
            double prev = current;
            
            if (dx == 0) {
                current = 90.0 * (dy / std::abs(dy));
            } 
            if (dx > 0){
                current = std::atan(dy / dx) * (-1.0) * 180.0 / M_PI;
            }
            if (dx < 0){
                current = - (std::atan(dy / dx) * 180.0 / M_PI + 180.0);
            }
            
            double turn_angle = current - prev;
            if (turn_angle == 0) {
                i++;
                continue;
            }
            
            if (360.0 - std::abs(turn_angle) < std::abs(turn_angle)) {
                turn_angle = (360.0 - std::abs(turn_angle)) * (std::abs(turn_angle) / turn_angle) * (-1.0);
            }
            
            std::cout << path[i + 1].x << ", " << path[i + 1].y << "         " << turn_angle << "           " << velocity << std::endl;
            i++; 
        }
    }
}    


