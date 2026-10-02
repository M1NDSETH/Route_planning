all: test vis clean

test: simple_test clean_test

vis: simple_vis clean_vis

simple_vis:
	mkdir -p cpp_visualiser/build
	cd cpp_visualiser/build && cmake .. && cmake --build . -j
	./cpp_visualiser/build/cpp_visualiser

simple_test:
	g++ cpp/Route.cpp cpp/scenario.cpp cpp/route_test.cpp -o cpp/my_route_test
	./cpp/my_route_test

clean_vis:
	rm -rf cpp_visualiser/build

clean_test:
	rm -rf cpp/my_route_test

clean: clean_vis clean_test
