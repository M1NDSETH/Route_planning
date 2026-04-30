from Route import AUV,GRID,visualize,obstacles_inflation
import random
import time

def main():
    grid_size_x = 1000
    grid_size_y = 500
    pillars=[]
    for i in range(3):
        pillars.append((random.randint(1,999),random.randint(1,499)))

    obstacles = []
    for i in range(10):
        obstacles.append((random.randint(1,999),random.randint(1,499)))
    AUV_size = 30
    VELT=AUV((1,1), AUV_size)
    grid = GRID(grid_size_x, grid_size_y, pillars, obstacles)
    
    obstacles_inflation(grid, obstacles, AUV_size)
    
    start_time = time.perf_counter()    
    path = VELT.build_full_route(grid.targets, grid)
    end_time = time.perf_counter()
    print(end_time - start_time)
    if path:
        print("Path length:", len(path))
        visualize(grid.field, path, grid.targets)
main()