#this program tests a Route.py in 3 cases with different pillars location

#to generate new path, you need to close previous window!

import Route as path_planner

path_planner.main(pillars = [(14,6),
                             (3,20),
                             (0, 0)])

path_planner.main(pillars = [(28,22),
                             (13,7),
                             (0, 0)])

path_planner.main(pillars = [(2,10),
                             (29,25),
                             (0, 0)])