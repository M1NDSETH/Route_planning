ALL: run clean

build: path.out

path.out: Route.cpp route_test.cpp
	g++ Route.cpp route_test.cpp -o my_route_test

run: build
	./my_route_test

clean:
	rm my_route_test