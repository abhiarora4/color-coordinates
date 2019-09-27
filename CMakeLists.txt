cmake_minimum_required(VERSION 3.12)

add_executable(color color.cpp XYData.cpp algo.cpp XYZData.cpp)

target_compile_options(color PRIVATE -g -Wall --std=c++17)
target_link_libraries(color PRIVATE -lstdc++fs)