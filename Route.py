import heapq
import matplotlib.pyplot as plt
import numpy as np
import math



def heuristic(a, b):
    return math.sqrt((a[0] - b[0])**2 + (a[1] - b[1])**2)



class GRID:
    def __init__(self, x_size, y_size, targets, obstacles):
        self.x_size = x_size
        self.y_size = y_size
        self.targets = targets
        self.field = np.zeros((x_size, y_size))
        for point in obstacles:
            self.field[point] = 1
       



class AUV:
    def __init__(self,start_point):
        self.start_point = start_point
    
    def build_full_route(self,targets, grid):
        full_path = []
        current = self.start_point
        for i, target in enumerate(targets):
            temp_grid = grid.copy()
            for j,p in enumerate(targets):
                if p != target and j>i:
                    temp_grid[p] = 1   
            path_segment = astar(current, target, temp_grid)
            if path_segment is None:
                print("Path not found")
                return None
            if full_path:
                full_path.extend(path_segment[1:])
            else:
                full_path.extend(path_segment)
            current = target
        return full_path 



def astar(start, goal, grid):
    rows, cols = grid.shape
    open_set = []
    heapq.heappush(open_set, (0, start))
    came_from = {}
    g_score = {start: 0}
    directions = [(-1,0),(1,0),(0,-1),(0,1),
                  (-1,-1),(-1,1),(1,-1),(1,1)]
    while open_set:
        _, current = heapq.heappop(open_set)
        if current == goal:
            path = []
            while current in came_from:
                path.append(current)
                current = came_from[current]
            path.append(start)
            return path[::-1]
        for dx, dy in directions:
            neighbor = (current[0] + dx, current[1] + dy)
            if 0 <= neighbor[0] < rows and 0 <= neighbor[1] < cols:
                if grid[neighbor] == 1:
                    continue  
                tentative_g = g_score[current] + heuristic(current, neighbor)
                if neighbor not in g_score or tentative_g < g_score[neighbor]:
                    came_from[neighbor] = current
                    g_score[neighbor] = tentative_g
                    f_score = tentative_g + heuristic(neighbor, goal)
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



def main():
    grid_size_x = 30
    grid_size_y = 30
    
    obstacles = []

    pillars = [
        (25,10),
        (0,25),
        (12, 12)
    ]
   
    VELT=AUV((0,0))
    #grid[5:15, 12] = 1
    grid = GRID(grid_size_x, grid_size_y, pillars, obstacles)
    
    path = VELT.build_full_route(grid.targets, grid.field)
    if path:
        print("Path length:", len(path))
        visualize(grid.field, path, grid.targets)
        return 0
    else:
        return 1

if __name__ == "__main__":
    main()
    