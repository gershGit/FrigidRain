cmake_minimum_required(VERSION 3.9)
project(CLion_Proj)

set(CMAKE_CXX_STANDARD 11)

include_directories(BEFORE D:/Documents/Visual Studio 2017/Libraries/stb-master)

add_executable(CLion_Proj main.cpp)