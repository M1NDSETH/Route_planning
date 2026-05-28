#include "Route.h"
#include <cmath>
#include <vector>
#include <queue>
#include <algorithm>
#include <iostream>


//структура точки пространства
struct Point
{
    int x;
    int y;
};

//вспомогательные функции
int max(int a, int b){
    if (a < b) {return b;} else {return a;}
}
int min(int a, int b){
    if (a < b) {return a;} else {return b;}
}
int heuristic(int x0, int y0, int x1, int y1){
    return (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0);
}

//надувание препятствий
void obstacles_inflation(std::vector <uint8_t> field, GRID grid, Point center, int radius){
    for (int x = center.x - radius; x <= center.x + radius; x++){
        for (int y = center.y - radius; y <= center.y + radius; y++){
            if (!grid.inside(x, y)){
                continue;
            }
            if (heuristic(center.x, center.y, x, y) <= radius){
                field[grid.index(x, y)] = 1;
            }
        }
    }
}

std::vector <Point> theta_star(Point start, Point target, GRID grid, std::vector <uint8_t> temp_field){
    int size = grid.x_size * grid.y_size;
    std::vector <double> g_score(size, INFINITY);
    std::vector <uint8_t> closed_set(size, 0);
    std::vector <Point> parents(size);

    using QueueNode = std::pair<double, Point>;
    auto compare =[](const QueueNode& a, const QueueNode& b){
        return a.first > b.first;
    };
    std::priority_queue < QueueNode, std::vector<QueueNode>, decltype(compare) > open(compare);
    int start_index = grid.index(start);
    g_score[start_index] = 0;
    parents[start_index] = start;
    open.push({heuristic(start.x, start.y, target.x, target.y), start});
    const int directions[8][2] =
    {
        {-1,  0},
        { 1,  0},
        { 0, -1},
        { 0,  1},

        {-1, -1},
        {-1,  1},
        { 1, -1},
        { 1,  1}
    };

    while (!open.empty()){
        Point current = open.top().second;
        open.pop();
        int current_index = grid.index(current);
        if (closed_set[current_index]){
            continue;
        }
        closed_set[current_index] = 1;
        if (current.x == target.x && current.y == target.y){
            std::vector <Point> path;
            Point p = target;
            while (!(p.x == start.x && p.y == start.y)){
                path.push_back(start);
                p = parents[grid.index(p)];
            }
            path.push_back(start);
            std::reverse(path.begin(),path.end());
            return(path);
        }
        for (int i = 0; i < 8; i++){
            Point neighbor{current.x + directions[i][0], current.y + directions[i][1]};
            if (!grid.inside(neighbor.x, neighbor.y)){
                continue;
            }
            int neighbor_index = grid.index(neighbor);
            if (grid.field[neighbor_index] == 1 || temp_field[neighbor_index] == 1){
                continue;
            }
            if (!grid.valid_move(current, neighbor)){
                continue;
            }

            Point current_parent = parents[current_index];
            double new_g;
            if (grid.line_of_sight(current_parent, neighbor, temp_field)){
                new_g = g_score[grid.index(current_parent)] + heuristic(current_parent.x, current_parent.y, neighbor.x, neighbor.y);
                if (new_g < g_score[neighbor_index]){
                    g_score[neighbor_index] = new_g;
                    parents[neighbor_index] =
                    current_parent;
                    double f = new_g + heuristic(neighbor.x, neighbor.y, target.x, target.y);
                    open.push({f, neighbor});
                }
            }
            else{
                new_g = g_score[current_index] + heuristic(current.x, current.y, neighbor.x, neighbor.y);
                if (new_g < g_score[neighbor_index]){
                    g_score[neighbor_index] = new_g;
                    parents[neighbor_index] = current;
                    double f = new_g + heuristic(neighbor.x, neighbor.y, target.x, target.y);
                    open.push({f, neighbor});
                }
            }
        }

    }
    return {};
}


//класс сетки точек пространства
class GRID {
    public:
         
        int x_size, y_size;
        std::vector <Point> targets, obstacles;
        std::vector <uint8_t> field;

        //преобразование координат точки в индекс в сетке
        inline int index(Point p) const{
            return p.y * x_size + p.x;
        }
        inline int index(int x, int y) const{
            return y * x_size + x;
        }
        inline bool inside(int x, int y) const{
            return (x >= 0 && x < x_size && y >= 0 && y < y_size);
        }
        
        GRID(int grid_x_size, int grid_y_size, std::vector<Point> grid_targets, std::vector<Point> grid_obstacles, int inflation_size){
            x_size = grid_x_size;
            y_size = grid_y_size;
            targets = grid_targets;
            obstacles = grid_obstacles;
            field.resize(grid_x_size * grid_y_size, 0);
        }

        
        //проверка corner cutting
        bool valid_move(Point current, Point neighbor){
            int x0 = current.x;
            int y0 = current.y;
            int x1 = neighbor.x;
            int y1 = neighbor.y;

            if ((x1 - x0 != 0) and (y1 - y0 != 0)){
                if ((field[index(x1, y0)] == 1) or (field[index(x0, y1)]) == 1) {return false;}
            }
            return true;
        }
        //проверка линии взгляда LOS
        bool line_of_sight(Point parent, Point neighbor, std::vector <uint8_t> clean_field){
            std::vector <Point> cells = bresenham(parent,neighbor);
            for (size_t i = 0; i < cells.size() - 1; i++){
                if (inside(cells[i].x, cells[i].y)){
                    if (clean_field[index(cells[i])] == 1) {return false;}
                }
                else {return false;}
            }
            return true;
        }

};

//класс аппарата
class AUV{
    private:
    Point start_point;
    int size;

    public:

    std::vector <Point> build_full_route(std::vector <Point> targets, GRID grid){
        std::vector <Point> full_path;
        Point current = start_point;
        for (size_t i = 0; i < targets.size(); i++){
            Point current_target = targets[i];
            std::vector <uint8_t> temp_field(grid.x_size * grid.y_size, 0);
            for (size_t j = i + 1; j < targets.size(); j++){
                obstacles_inflation(temp_field, grid, targets[j], size / 2);
            }
            std::vector <Point> path_segment = theta_star(current, current_target, grid, temp_field);
            if (path_segment.empty()){
                return {};
            }
            if (!path_segment.empty()){
                full_path.insert(full_path.end(), path_segment.begin() + 1, path_segment.end());
            }
            else{
                full_path.insert(full_path.end(), path_segment.begin(), path_segment.end());
            }
            current = current_target;
        }
        return full_path;
    }
};



//вычисление размера аппарата
double size_calculation(double length, double weight){
    double size = sqrt(length * length + weight * weight);
    return size;
}

//список точек, которые приблизительно образуют прямую между 2 точками
std::vector <Point> bresenham(Point start, Point finish){
    std::vector <Point> cells;
    int dx = abs(finish.x - start.x);
    int dy = abs(finish.y - start.y);
    int Sx = 0;
    int Sy = 0;
    if (start.x < finish.x) {Sx = 1;} else { Sx = -1;}
    if (start.y < finish.y) {Sy = 1;} else {Sy = -1;}
    int err = dx - dy;

    while (true){
        Point new_point = {start.x, start.y};
        cells.push_back(new_point);
        if ((start.x == finish.x) and (start.y == finish.y)){
            break;
        }
        int local_err = err * 2;
        if (local_err > -dy){
            err -= dy;
            start.x += Sx;
        }
        if (local_err < dx){
            err += dx;
            start.y += Sy;
        }
    }

    return cells;
}

//вывод углов поворота по маршруту
void angle_velocity_output(std::vector <Point> path, int velocity){
    size_t i = 0;
    double current;
    while (i < path.size() - 2){
        int dx = path[i+1].x - path[i].x;
        int dy = path[i+1].y - path[i].y;
        if (i == 0){
            current = std::atan2(dy, dx) * (-1) * 180.0 / M_PI;
            std::cout << path[i].x << path[i].y << current << velocity << std::endl;
        }
        else{
            double prev = current;
            
            if (dx == 0){
                current = 90 * (dy / abs(dy));
            }
            else{
                current = std::atan2(dy, dx) * (-1) * 180.0 / M_PI;
            }
            double turn_angle = current - prev;
            if (turn_angle == 0){
                i++;
                continue;
            }
            if (360 - abs(turn_angle) < abs(turn_angle)){
                turn_angle = (360 - abs(turn_angle)) * (abs(turn_angle) / turn_angle) * (-1);
            }
        }
        std::cout << path[i].x << path[i].y << current << velocity << std::endl;
    }
}



