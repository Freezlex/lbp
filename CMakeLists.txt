cmake_minimum_required(VERSION 3.26)
project(lbp_parking_lot)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 23)

add_executable(lbp_parking_lot main.cpp)

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(lbp_parking_lot ${OpenCV_LIBS})
