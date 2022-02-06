cmake_minimum_required(VERSION 3.21)
set(PROJECT_NAME A23DGraphicsProgramming)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
        src/Main.cpp
        )

target_include_directories(${PROJECT_NAME} PUBLIC

        )

target_precompile_headers(${PROJECT_NAME} PUBLIC
        <iostream>
        )