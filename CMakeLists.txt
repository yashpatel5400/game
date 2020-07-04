cmake_minimum_required(VERSION 3.0.0)
project(game VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

find_package(folly CONFIG REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(gflags CONFIG REQUIRED)
find_package(gflags CONFIG REQUIRED)
find_package(glog CONFIG REQUIRED)

# Basic SDF example
add_executable(
    simple
    app/simple.cpp
)
target_link_libraries(
    simple 

    glfw
    gflags_static 
    GLEW::GLEW
    glog::glog
)

# Heightmapped water example
add_executable(
    heightmap
    app/heightmap.cpp
)
target_link_libraries(
    heightmap 

    glfw
    gflags_static 
    GLEW::GLEW
    glog::glog
)