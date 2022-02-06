cmake_minimum_required(VERSION 3.21)
set(PROJECT_NAME A23DGraphicsProgramming)

project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(entity-component-system)
add_subdirectory(vendor/glm)


add_executable(${PROJECT_NAME}
        src/Main.cpp
        )

target_include_directories(${PROJECT_NAME} PUBLIC
        entity-component-system
        entity-component-system/include
        entity-component-system/include/systems

        vendor/glm/glm

        entity-component-system/src
        )

target_precompile_headers(${PROJECT_NAME} PUBLIC
        <iostream>
        )


find_library(ECS EntityComponentSystem2022 cmake-build-debug/entity-component-system)

message(STATUS ${ECS})
target_link_libraries(${PROJECT_NAME}
        ${ECS}
        )