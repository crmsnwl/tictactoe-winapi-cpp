cmake_minimum_required(VERSION 3.21)
project(TTT)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS " -static -static-libgcc -static-libstdc++ -std=c++11")
add_executable(TTT main.cpp ttt.rc)
