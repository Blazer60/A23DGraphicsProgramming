cmake_minimum_required(VERSION 3.21)
set(PROJECT_NAME A23DGraphicsProgramming)

project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 17)

message(STATUS "Using Cmake:    " ${CMAKE_VERSION})
message(STATUS "Compiling with: " ${CMAKE_CXX_COMPILER_ID})
message(STATUS "CXX Standard:   " ${CMAKE_CXX_STANDARD})

add_subdirectory(entity-component-system)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/glew/build/cmake)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor)


add_executable(${PROJECT_NAME}
        src/Main.cpp                                src/core/Core.cpp                           src/core/Core.h
        src/core/Scene.cpp                          src/core/Scene.h                            include/core/DebugLogger.h
        src/core/DebugLogger.cpp                    include/components/Components.h             src/systems/BasicBinderSystem.cpp
        include/systems/BasicBinderSystem.h         src/helpers/Primitives.cpp                  include/helpers/Primitives.h
        src/core/Shader.cpp                         include/core/Shader.h                       src/systems/BasicShaderSystem.cpp
        include/systems/BasicShaderSystem.h         src/core/MainCamera.cpp                     include/core/MainCamera.h
        src/systems/BasicUniformUpdaterSystem.cpp   include/systems/BasicUniformUpdaterSystem.h src/helpers/Timers.cpp
        include/helpers/Timers.h                    src/systems/RotatorSystem.cpp               include/systems/RotatorSystem.h
        src/systems/UvBinderSystem.cpp              include/systems/UvBinderSystem.h            src/systems/UvShaderSystem.cpp
        include/systems/UvShaderSystem.h)

target_include_directories(${PROJECT_NAME} PUBLIC
# Project Includes
        include                                     include/core
        include/systems                             include/components
        include/helpers

# ECS Includes
        entity-component-system                     entity-component-system/include
        entity-component-system/include/systems     entity-component-system/src

# Vendor Includes
        vendor/glm/glm                              vendor/glew/include/GL
        vendor/glfw/include/GLFW                    vendor/imgui
        )

target_precompile_headers(${PROJECT_NAME} PUBLIC
        <iostream>  <vector>        <unordered_map>
        <string>    <string_view>   <algorithm>
        <memory>    <numeric>       <cstdint>
        <glm.hpp>   [["DebugLogger.h"]]
        )

target_compile_definitions(${PROJECT_NAME} PUBLIC
        GLEW_STATIC
        )


find_package(OpenGL)  # Glew Requires OpenGL to be added.
find_library(ECS   NAMES EntityComponentSystem2022 PATHS ${CMAKE_CURRENT_BINARY_DIR}/entity-component-system REQUIRED)
find_library(GLEW  NAMES libglew32d                PATHS ${CMAKE_CURRENT_BINARY_DIR}/lib                     REQUIRED)
find_library(GLFW  NAMES glfw3                     PATHS ${CMAKE_CURRENT_BINARY_DIR}/vendor/glfw/src         REQUIRED)
find_library(IMGUI NAMES imgui                     PATHS ${CMAKE_CURRENT_BINARY_DIR}/vendor                  REQUIRED)

target_link_libraries(${PROJECT_NAME}
        ${ECS}  ${GLEW}
        ${GLFW} ${IMGUI}
        OpenGL::GL
        )