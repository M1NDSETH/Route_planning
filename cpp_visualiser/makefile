all: run clean

run:
	mkdir -p build
	cd build && cmake .. && cmake --build . -j
	./build/cpp_visualiser
clean:
	rm -rf build
