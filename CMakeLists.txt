cmake_minimum_required(VERSION 3.10)
project(particle VERSION 1.0)

find_package(raylib REQUIRED)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(particle Main.cpp)

target_link_libraries(particle raylib)
