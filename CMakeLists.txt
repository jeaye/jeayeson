project(JeayeSON)

# States that CMake required version must be greater than 2.6
cmake_minimum_required(VERSION 2.6)

# C++11
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall --pedantic")

set(jeayeson_SRCS 
	src/main.cpp
)

find_package(Boost COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(jeayeson ${jeayeson_SRCS})

target_link_libraries(jeayeson ${Boost_LIBRARIES})

