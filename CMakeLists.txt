cmake_minimum_required(VERSION 3.21)
set(PROJECT_NAME A23DGraphicsProgramming)

project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(entity-component-system)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/glew/build/cmake)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor)


add_executable(${PROJECT_NAME}
        src/Main.cpp
        )

target_include_directories(${PROJECT_NAME} PUBLIC
        entity-component-system
        entity-component-system/include
        entity-component-system/include/systems

        vendor/glm/glm

        vendor/glew/include/GL

        vendor/glfw/include/GLFW

        vendor/imgui

        entity-component-system/src
        )

target_precompile_headers(${PROJECT_NAME} PUBLIC
        <iostream>
        )

target_compile_definitions(${PROJECT_NAME} PUBLIC
        GLEW_STATIC
        )

find_package(OpenGL)  # Glew Requires OpenGL to be added.
find_library(ECS EntityComponentSystem2022 cmake-build-debug/entity-component-system REQUIRED)
find_library(GLEW libglew32d cmake-build-debug/lib REQUIRED)
find_library(GLFW glfw3 cmake-build-debug/vendor/glfw/src)
find_library(IMGUI imgui cmake-build-debug/vendor)

message(STATUS ${ECS})
message(STATUS ${GLEW})
message(STATUS ${GLFW})
message(STATUS ${IMGUI})
target_link_libraries(${PROJECT_NAME}
        ${ECS} ${GLEW} OpenGL::GL ${GLFW} ${IMGUI}
        )