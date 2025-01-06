cmake_minimum_required(VERSION 3.27)
project(badapple)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(Boost REQUIRED COMPONENTS system filesystem)
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -pthread -lboost_system -lboost_thread -lboost_chrono -Ofast -lncurses -ltinfo")
add_executable(badapple main.cpp)
target_link_libraries(badapple ${OpenCV_LIBS})

