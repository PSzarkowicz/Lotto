cmake_minimum_required(VERSION 4.0)
project(Lotto)

set(CMAKE_CXX_STANDARD 20)

add_executable(Lotto main.cpp
        defs.h
        Print.cpp
        Print.h
        Data.cpp
        Data.h)
