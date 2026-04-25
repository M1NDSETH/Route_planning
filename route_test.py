from Route import AUV,GRID,visualize
import random
import time

def main():
    grid_size_x = 1000
    grid_size_y = 500
    pillars=[]
    for i in range(3):
        pillars.append((random.randint(1,999),random.randint(1,499)))

    obstacles = []
    for i in range(50):
        obstacles.append((random.randint(1,999),random.randint(1,499)))
    AUV_size = 30
    VELT=AUV((1,1), AUV_size)
    grid = GRID(grid_size_x, grid_size_y, pillars, obstacles)
    start_time = time.perf_counter()
    grid.obstacles_creation(AUV_size)
    end_time = time.perf_counter()
    print(end_time - start_time)    
    path = VELT.build_full_route(grid.targets, grid.field)

    if path:
        print("Path length:", len(path))
        visualize(grid.field, path, grid.targets)
main()