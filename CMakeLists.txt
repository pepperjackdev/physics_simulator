cmake_minimum_required(VERSION 3.15)
project(physics_simulator CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# TODO: should be avoided;
#       do not provide multiple `main` functions
file(GLOB GAME_SRC_FILES
    "src/**/*.cpp"
    "src/*.cpp"
)

add_executable(physics_simulator ${GAME_SRC_FILES})

find_package(PkgConfig REQUIRED)
pkg_check_modules(raylib REQUIRED IMPORTED_TARGET raylib)
target_link_libraries(physics_simulator PRIVATE PkgConfig::raylib)
target_include_directories(physics_simulator PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_custom_target(run
    COMMAND physics_simulator
    DEPENDS physics_simulator
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running project..."
)