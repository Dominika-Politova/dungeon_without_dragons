cmake_minimum_required(VERSION 3.10)
project(Dungeon)

set(CMAKE_CXX_STANDARD 17)

add_executable(Dungeon
    main.cpp
    Parser.cpp
    Bot_simulator.cpp
    Alice_strategy.cpp
    Output.cpp
)