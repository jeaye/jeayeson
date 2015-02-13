cmake_minimum_required(VERSION 2.8.4)
project(test_odr)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

set(SOURCE_FILES main.cpp odr.cpp)
add_executable(test_odr ${SOURCE_FILES})

include_directories(../../../include) 
