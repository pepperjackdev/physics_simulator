
ifndef VERBOSE
.SILENT:
endif

# Runs the project
run: build
	./build/physics_simulator

# Builds the sources with build-system
build: generate
	ninja -C build

# Generates the build-system's scripts
generate:
	cmake -B build -G Ninja