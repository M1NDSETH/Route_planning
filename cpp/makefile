ALL: run clean

build: path.out

path.out: Route.cpp route_test.cpp scenario.cpp
	g++ Route.cpp route_test.cpp scenario.cpp -o my_route_test

run: build
	./my_route_test

clean:
	rm my_route_test
