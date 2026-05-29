#ifndef ROUTE_H
#define ROUTE_H

#include <vector>
#include <cstdint>

// Структура точки пространства
struct Point {
    int x;
    int y;
};

// Прототип функции Bresenham
std::vector<Point> bresenham(Point start, Point finish);

// Класс сетки точек пространства
class GRID {
public:
    int x_size, y_size;
    std::vector<Point> targets, obstacles;
    std::vector<uint8_t> field;

    GRID(int grid_x_size, int grid_y_size, std::vector<Point> grid_targets, std::vector<Point> grid_obstacles);

    inline int index(Point p) const {
        return p.y * x_size + p.x;
    }
    inline int index(int x, int y) const {
        return y * x_size + x;
    }
    inline bool inside(int x, int y) const {
        return (x >= 0 && x < x_size && y >= 0 && y < y_size); 
    }

    bool valid_move(Point current, Point neighbor);
    bool line_of_sight(Point parent, Point neighbor, std::vector<uint8_t> clean_field);
};

inline double metres_to_grid_units(double dist, double K){
    return dist * K;
}
int max(int a, int b);
int min(int a, int b);
int heuristic(int x0, int y0, int x1, int y1);

// Надувание препятствий (передача field строго по ссылке &)
void obstacles_inflation(std::vector<uint8_t>& field, GRID grid, Point center, int radius);

// Поиск пути Theta*
std::vector<Point> theta_star(Point start, Point target, GRID grid, std::vector<uint8_t> temp_field);

// Класс аппарата
class AUV {
public:
    Point start_point;
    int radius;

    AUV(Point start, double length, double weight);
    std::vector<Point> build_full_route(std::vector<Point> targets, GRID grid);
};

void angle_velocity_output(std::vector<Point> path, int velocity);

#endif // ROUTE_H