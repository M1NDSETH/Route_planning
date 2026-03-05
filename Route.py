import heapq
import matplotlib.pyplot as plt
import numpy as np
import math


def heuristic(a, b):
    return math.sqrt((a[0] - b[0])**2 + (a[1] - b[1])**2)

class GRID:
    def __init__(self, x_size, y_size, pillars, gates):
        self.x_size=x_size
        self.y_size=y_size
        self.pillars=pillars
        self.gates=gates
        self.field=np.zeros((x_size, y_size))
        self.gate_centre=((gates[0][0]+gates[1][0])//2,
                          (gates[0][1]+gates[1][1])//2)
    def init_gate(self):
        for i in self.gates:
            self.field[i]=1
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




def build_full_route(start, pillars, gate, grid):
    full_path = []
    current = start
    targets = pillars + [gate]
    for i, target in enumerate(targets):
        temp_grid = grid.copy()
        if target in pillars:
            for j,p in enumerate(pillars):
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




def visualize(grid, path, pillars, gate_centre, gates):
    plt.figure()
    plt.imshow(grid.T, origin='lower', cmap='gray_r')
    x = [p[0] for p in path]
    y = [p[1] for p in path]
    plt.plot(x, y, color='red', linewidth=2)
    for i, p in enumerate(pillars):
        plt.scatter(p[0], p[1])
        plt.text(p[0], p[1], f' Pillar {i+1}')
    plt.scatter(gate_centre[0], gate_centre[1])
    x1_gate=gates[0][0]
    y1_gate=gates[1][0]
    x2_gate=gates[0][1]
    y2_gate=gates[1][1]
    gate1=[x1_gate,y1_gate]
    gate2=[x2_gate,y2_gate]
    plt.plot(gate1,gate2, color='green', linewidth=2)
    plt.title("Route A*")
    plt.grid(True)
    plt.show()



def main():
    grid_size_x = 30
    grid_size_y = 30
    #grid = np.zeros((grid_size_x, grid_size_y))

    start_point = (0, 10)

    pillars = [
        (25,10),
        (0,25),
        (0, 0)
    ]

    gates=[
        (11,25),
        (25,15)
    ]

    #xgate=0
    #ygate=0
    #for i in gates:
     #   xgate=xgate + i[0]
      #  ygate=ygate+i[1]
       # grid[i]=1
    #gate_centre = (xgate//2, ygate//2)

    #grid[5:15, 12] = 1
    grid = GRID(grid_size_x, grid_size_y, pillars, gates)
    grid.init_gate()
    path = build_full_route(start_point, grid.pillars, grid.gate_centre, grid.field)
    if path:
        print("Path length:", len(path))
        visualize(grid.field, path, grid.pillars, grid.gate_centre,grid.gates)
        return 0
    else:
        return 1

main()
    