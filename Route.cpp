#include <cmath>
#include <vector>
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


//класс сетки точек пространства
class GRID {
    private:
        int x_size, y_size;
        std::vector <Point> targets, obstacles;
        std::vector <uint8_t> field;

    public:

        //преобразование координат точки в индекс в сетке
        inline int index(Point p) const{
            return p.y * x_size + p.x;
        }
        inline int index(int x, int y) const{
            return y * x_size + x;
        }
        
        GRID(int grid_x_size, int grid_y_size, std::vector<Point> grid_targets, std::vector<Point> grid_obstacles, int inflation_size){
            x_size = grid_x_size;
            y_size = grid_y_size;
            targets = grid_targets;
            obstacles = grid_obstacles;
            field.resize(grid_x_size * grid_y_size, 0);
        }

        void obstacles_creation(int inflation_size){
            inflation_size = inflation_size / 2;
            for (size_t i = 0; i < obstacles.size() - 1; i++){
                int x_point = max(0, obstacles[i].x - inflation_size);
                while (x_point <= min(obstacles[i].x + inflation_size, x_size - 1)){
                    int y_point = max(0, obstacles[i].y - inflation_size);
                    while (y_point <= min(obstacles[i].y + inflation_size, y_size - 1)){
                        if (heuristic(obstacles[i].x, obstacles[i].y, x_point, y_point) <= inflation_size * inflation_size){
                            field[index(obstacles[i])] = 1;
                        }
                        y_point += 1;
                    }
                    x_point += 1;
                }
            }

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
        bool line_of_sight(Point parent, Point neighbor){
            std::vector <Point> cells = bresenham(parent,neighbor);
            for (size_t i = 0; i < cells.size() - 1; i++){
                if ((0 <= cells[i].x < x_size) and (0 <= cells[i].y < y_size)){
                    if (field[index(cells[i])] == 1) {return false;}
                }
                else {return false;}
            }
            return true;
        }

};

class AUV{
    private:
    Point start_point;
    int size;

    public:

    std::vector <Point> build_full_route(std::vector <Point> targets, GRID grid){
        std:: vector <Point> full_path;
        Point current = start_point;

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
void angle_velocity_outpiy(std::vector <Point> path, int velocity){
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



