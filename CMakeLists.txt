cmake_minimum_required(VERSION 3.26)
project(lbp_parking_lot)

set(CMAKE_CXX_STANDARD 23)
IF (WIN32)
    set(ENV{OPENCV_DIR} "C:\\tools\\opencv\\build")
ENDIF()


find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(lbp_parking_lot main.cpp
        Objetcs/PixelSet/PixelSet.cpp
        Objetcs/PixelSet/PixelSet.h
        Objetcs/ImageReader/PixelDataset.cpp
        Objetcs/ImageReader/PixelDataset.h
        Objetcs/LbpResult/LbpResult.cpp
        Objetcs/LbpResult/LbpResult.h)
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)
target_link_libraries(lbp_parking_lot ${OpenCV_LIBS})
