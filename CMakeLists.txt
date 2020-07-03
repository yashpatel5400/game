cmake_minimum_required(VERSION 3.0.0)
project(game VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

add_executable(game
    main.cpp
)
target_link_libraries(
    game 
    glfw
    GLEW::GLEW
)