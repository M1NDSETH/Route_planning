from Route import AUV,GRID,visualize
import random
def main():
    grid_size_x = 30
    grid_size_y = 30
    pillars=[]
    for i in range(3):
        pillars.append((random.randint(1,29),random.randint(1,29)))

    obstacles = []
    for i in range(99):
        obstacles.append((random.randint(1,29),random.randint(1,29)))

    VELT=AUV((1,1))
    grid = GRID(grid_size_x, grid_size_y, pillars, obstacles)
        
    path = VELT.build_full_route(grid.targets, grid.field)
    if path:
        print("Path length:", len(path))
        visualize(grid.field, path, grid.targets)
for i in range(10):
    main()