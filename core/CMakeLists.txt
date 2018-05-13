cmake_minimum_required(VERSION 3.9)
project(CoreLib LANGUAGES CXX)

add_library(core emulator.cpp cpu.cpp memory.cpp utils.cpp)

target_include_directories(core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)