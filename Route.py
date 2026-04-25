import heapq
import matplotlib.pyplot as plt
import numpy as np
import math



def heuristic(a, b):
    return math.sqrt((a[0] - b[0])**2 + (a[1] - b[1])**2)


def bresenham(x0,y0,x1,y1):
    cells =[]
    dx = abs(x1-x0)
    dy = abs(y1-y0)
    Sx = 1 if x0<x1 else -1
    Sy = 1 if y0<y1 else -1
    err = dx - dy
    while True:
        cells.append((x0,y0))
        if x0 == x1 and y0 == y1:
            break
        local_err = 2 * err
        if local_err > -dy:
            err -= dy
            x0 += Sx
        if local_err < dx:
            err += dx
            y0 += Sy
    return cells


def line_of_sight(grid, x0, y0, x1, y1):
    for (x,y) in bresenham(x0, y0, x1, y1):
        if grid[x][y] == 1:
            return False
    return True


class GRID:
    def __init__(self, x_size, y_size, targets, obstacles):
        self.x_size = x_size
        self.y_size = y_size
        self.targets = targets
        self.obstacles = obstacles
        self.field = np.zeros((x_size, y_size))
    def grid_creation(self,auv_size):
        for point in self.obstacles:
            for i in range(self.x_size):
                for j in range(self.y_size):
                    if heuristic((i,j),point) <= auv_size:
                        self.field[i][j] = 1
            self.field[point] = 1
       
        
class AUV:
    def __init__(self,start_point,size):
        self.start_point = start_point
        self.size = size
    
    def build_full_route(self,targets, grid):
        full_path = []
        current = self.start_point
        for i, target in enumerate(targets):
            temp_grid = grid.copy()
            for j,p in enumerate(targets):
                if p != target and j>i:
                    temp_grid[p] = 1   
            path_segment = theta_star(current, target, temp_grid)
            if path_segment is None:
                print("Path not found")
                return None
            if full_path:
                full_path.extend(path_segment[1:])
            else:
                full_path.extend(path_segment)
            current = target
        return full_path 


def theta_star(start, goal, grid):
    rows, cols = grid.shape
    open_set = []
    heapq.heappush(open_set, (0, start))
    came_from = {start: start} 
    g_score = {start: 0}
    directions = [(-1,0),(1,0),(0,-1),(0,1),
                  (-1,-1),(-1,1),(1,-1),(1,1)]
    while open_set:
        _, current = heapq.heappop(open_set)
        if current == goal:
            path = []
            while current != came_from[current]:
                path.append(current)
                current = came_from[current]
            path.append(start)
            return path[::-1]

        for dx, dy in directions:
            neighbor = (current[0] + dx, current[1] + dy)
            if not (0 <= neighbor[0] < rows and 0 <= neighbor[1] < cols):
                continue
            if grid[neighbor] == 1:
                continue
            parent = came_from[current]
            if line_of_sight(grid, parent[0], parent[1], neighbor[0], neighbor[1]):
                new_g = g_score[parent] + heuristic(parent, neighbor)

                if neighbor not in g_score or new_g < g_score[neighbor]:
                    g_score[neighbor] = new_g
                    came_from[neighbor] = parent
                    f_score = new_g + heuristic(neighbor, goal)
                    heapq.heappush(open_set, (f_score, neighbor))
            else:
                new_g = g_score[current] + heuristic(current, neighbor)

                if neighbor not in g_score or new_g < g_score[neighbor]:
                    g_score[neighbor] = new_g
                    came_from[neighbor] = current
                    f_score = new_g + heuristic(neighbor, goal)
                    heapq.heappush(open_set, (f_score, neighbor))
    return None


def visualize(grid, path, targets):
    plt.figure()
    plt.imshow(grid.T, origin='lower', cmap='gray_r')
    x = [p[0] for p in path]
    y = [p[1] for p in path]
    plt.plot(x, y, color='red', linewidth=2)
    for i, p in enumerate(targets):
        plt.scatter(p[0], p[1])
        plt.text(p[0], p[1], f' Target {i+1}')
    plt.title("Route A*")
    plt.grid(True)
    plt.show()

    