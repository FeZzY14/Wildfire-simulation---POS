cmake_minimum_required(VERSION 3.18)
project(Wildfire-simulation---POS)

set(CMAKE_CXX_STANDARD 17)

add_executable(Wildfire-simulation---POS main.cpp
        PoziarnyBiotop.h
        Bunka.h
        Bunka.cpp
        Svet.h
        Svet.cpp
        Generator.h
        Vietor.h)