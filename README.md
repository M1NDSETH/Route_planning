This program builds the shortest route to the goals with its subsequent visualization and the output of the route length in the coordinates of the program grid.
All object coordinates are specified in the program grid, which is not tied to real coordinates.

## GOALS

There are two types of targets: pillars and gates.
1) PILLARS: designed to be shot down by the device. They are set in the *pillars* list using two coordinates in the program grid. In the list, the pillars are arranged in the order of their passage by the device.
2) GATES: designed to pass the device through them. They are set by the coordinates in the program grid of two pillars in the *gates* list (the order in the list does not matter). When moving towards the gate, the device will come to the middle of the line connecting the gate posts (*gate_centre*)
The order of targets for the device: pillars in the order of the *pillars* list, then gates.

## START OF PROGRAM

Before starting the program, the user sets the coordinates of the starting point of the device (*start*), the size of the grid of the program (*grid_size_x,grid_size_y*), the coordinates of the pillars and gates.

## VISUALIZATION

The red line on the graph shows the route of the device, the green line shows the goal line, the black squares are the walls or pillars of the gate.




