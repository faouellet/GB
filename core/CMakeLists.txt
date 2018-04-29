cmake_minimum_required(VERSION 3.10)
project(CoreLib LANGUAGES CXX)

add_library(core cpu.cpp)

target_include_directories(core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)