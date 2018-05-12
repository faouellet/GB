cmake_minimum_required(VERSION 3.9)
project(GB LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

enable_testing()
add_subdirectory(core)
add_subdirectory(test)

add_executable(gb main.cpp)
target_link_libraries(gb core)
